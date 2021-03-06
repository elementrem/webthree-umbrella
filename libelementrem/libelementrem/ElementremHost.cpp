/*
	This file is part of cpp-elementrem.

	cpp-elementrem is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-elementrem is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-elementrem.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file ElementremHost.cpp
 * 
 * 
 */

#include "ElementremHost.h"

#include <chrono>
#include <thread>
#include <libdevcore/Common.h>
#include <libp2p/Host.h>
#include <libp2p/Session.h>
#include <libelecore/Exceptions.h>
#include "BlockChain.h"
#include "TransactionQueue.h"
#include "BlockQueue.h"
#include "ElementremPeer.h"
#include "BlockChainSync.h"

using namespace std;
using namespace dev;
using namespace dev::ele;
using namespace p2p;

unsigned const ElementremHost::c_oldProtocolVersion = 62; //TODO: remove this once v63+ is common
static unsigned const c_maxSendTransactions = 256;

char const* const ElementremHost::s_stateNames[static_cast<int>(SyncState::Size)] = {"NotSynced", "Idle", "Waiting", "Blocks", "State", "NewBlocks" };

#if defined(_WIN32)
const char* ElementremHostTrace::name() { return ElePurple "^" EleGray "  "; }
#else
const char* ElementremHostTrace::name() { return ElePurple "⧫" EleGray " "; }
#endif

ElementremHost::ElementremHost(BlockChain const& _ch, OverlayDB const& _db, TransactionQueue& _tq, BlockQueue& _bq, u256 _networkId):
	HostCapability<ElementremPeer>(),
	Worker		("elesync"),
	m_chain		(_ch),
	m_db(_db),
	m_tq		(_tq),
	m_bq		(_bq),
	m_networkId	(_networkId)
{
	m_latestBlockSent = _ch.currentHash();
	m_tq.onImport([this](ImportResult _ir, h256 const& _h, h512 const& _nodeId) { onTransactionImported(_ir, _h, _nodeId); });
}

ElementremHost::~ElementremHost()
{
}

bool ElementremHost::ensureInitialised()
{
	if (!m_latestBlockSent)
	{
		// First time - just initialise.
		m_latestBlockSent = m_chain.currentHash();
		clog(ElementremHostTrace) << "Initialising: latest=" << m_latestBlockSent;

		Guard l(x_transactions);
		m_transactionsSent = m_tq.knownTransactions();
		return true;
	}
	return false;
}

void ElementremHost::reset()
{
	RecursiveGuard l(x_sync);
	if (m_sync)
		m_sync->abortSync();
	m_sync.reset();
	m_syncStart = 0;

	m_latestBlockSent = h256();
	Guard tl(x_transactions);
	m_transactionsSent.clear();
}

void ElementremHost::doWork()
{
	bool netChange = ensureInitialised();
	auto h = m_chain.currentHash();
	// If we've finished our initial sync (including getting all the blocks into the chain so as to reduce invalid transactions), start trading transactions & blocks
	if (!isSyncing() && m_chain.isKnown(m_latestBlockSent))
	{
		if (m_newTransactions)
		{
			m_newTransactions = false;
			maintainTransactions();
		}
		if (m_newBlocks)
		{
			m_newBlocks = false;
			maintainBlocks(h);
		}
	}

	time_t  now = std::chrono::system_clock::to_time_t(chrono::system_clock::now());
	if (now - m_lastTick >= 1)
	{
		m_lastTick = now;
		foreachPeer([](std::shared_ptr<ElementremPeer> _p) { _p->tick(); return true; });
	}

	if (m_syncStart)
	{
		DEV_RECURSIVE_GUARDED(x_sync)
			if (!m_sync)
			{
				time_t now = std::chrono::system_clock::to_time_t(chrono::system_clock::now());
				if (now - m_syncStart > 10)
				{
					m_sync.reset(new BlockChainSync(*this));
					m_syncStart = 0;
					m_sync->restartSync();
				}
			}
	}

//	return netChange;
	// TODO: Figure out what to do with netChange.
	(void)netChange;
}

void ElementremHost::maintainTransactions()
{
	// Send any new transactions.
	unordered_map<std::shared_ptr<ElementremPeer>, std::vector<size_t>> peerTransactions;
	auto ts = m_tq.topTransactions(c_maxSendTransactions);
	{
		Guard l(x_transactions);
		for (size_t i = 0; i < ts.size(); ++i)
		{
			auto const& t = ts[i];
			bool unsent = !m_transactionsSent.count(t.sha3());
			auto peers = get<1>(randomSelection(0, [&](ElementremPeer* p) { return p->m_requireTransactions || (unsent && !p->m_knownTransactions.count(t.sha3())); }));
			for (auto const& p: peers)
				peerTransactions[p].push_back(i);
		}
		for (auto const& t: ts)
			m_transactionsSent.insert(t.sha3());
	}
	foreachPeer([&](shared_ptr<ElementremPeer> _p)
	{
		bytes b;
		unsigned n = 0;
		for (auto const& i: peerTransactions[_p])
		{
			_p->m_knownTransactions.insert(ts[i].sha3());
			b += ts[i].rlp();
			++n;
		}

		_p->clearKnownTransactions();

		if (n || _p->m_requireTransactions)
		{
			RLPStream ts;
			_p->prep(ts, TransactionsPacket, n).appendRaw(b, n);
			_p->sealAndSend(ts);
			clog(ElementremHostTrace) << "Sent" << n << "transactions to " << _p->session()->info().clientVersion;
		}
		_p->m_requireTransactions = false;
		return true;
	});
}

void ElementremHost::foreachPeer(std::function<bool(std::shared_ptr<ElementremPeer>)> const& _f) const
{
	//order peers by protocol, rating, connection age
	auto sessions = peerSessions();
	auto sessionLess = [](std::pair<std::shared_ptr<Session>, std::shared_ptr<Peer>> const& _left, std::pair<std::shared_ptr<Session>, std::shared_ptr<Peer>> const& _right)
		{ return _left.first->rating() == _right.first->rating() ? _left.first->connectionTime() < _right.first->connectionTime() : _left.first->rating() > _right.first->rating(); };

	std::sort(sessions.begin(), sessions.end(), sessionLess);
	for (auto s: sessions)
		if (!_f(s.first->cap<ElementremPeer>()))
			return;

	sessions = peerSessions(c_oldProtocolVersion); //TODO: remove once v61+ is common
	std::sort(sessions.begin(), sessions.end(), sessionLess);
	for (auto s: sessions)
		if (!_f(s.first->cap<ElementremPeer>(c_oldProtocolVersion)))
			return;
}

tuple<vector<shared_ptr<ElementremPeer>>, vector<shared_ptr<ElementremPeer>>, vector<shared_ptr<Session>>> ElementremHost::randomSelection(unsigned _percent, std::function<bool(ElementremPeer*)> const& _allow)
{
	vector<shared_ptr<ElementremPeer>> chosen;
	vector<shared_ptr<ElementremPeer>> allowed;
	vector<shared_ptr<Session>> sessions;
	
	size_t peerCount = 0;
	foreachPeer([&](std::shared_ptr<ElementremPeer> _p)
	{
		if (_allow(_p.get()))
		{
			allowed.push_back(_p);
			sessions.push_back(_p->session());
		}
		++peerCount;
		return true;
	});

	size_t chosenSize = (peerCount * _percent + 99) / 100;
	chosen.reserve(chosenSize);
	for (unsigned i = chosenSize; i && allowed.size(); i--)
	{
		unsigned n = rand() % allowed.size();
		chosen.push_back(std::move(allowed[n]));
		allowed.erase(allowed.begin() + n);
	}
	return make_tuple(move(chosen), move(allowed), move(sessions));
}

void ElementremHost::maintainBlocks(h256 const& _currentHash)
{
	// Send any new blocks.
	auto detailsFrom = m_chain.details(m_latestBlockSent);
	auto detailsTo = m_chain.details(_currentHash);
	if (detailsFrom.totalDifficulty < detailsTo.totalDifficulty)
	{
		if (diff(detailsFrom.number, detailsTo.number) < 20)
		{
			// don't be sending more than 20 "new" blocks. if there are any more we were probably waaaay behind.
			clog(ElementremHostTrace) << "Sending a new block (current is" << _currentHash << ", was" << m_latestBlockSent << ")";

			h256s blocks = get<0>(m_chain.treeRoute(m_latestBlockSent, _currentHash, false, false, true));

			auto s = randomSelection(25, [&](ElementremPeer* p){
				DEV_GUARDED(p->x_knownBlocks)
					return !p->m_knownBlocks.count(_currentHash);
				return false;
			});
			for (shared_ptr<ElementremPeer> const& p: get<0>(s))
				for (auto const& b: blocks)
				{
					RLPStream ts;
					p->prep(ts, NewBlockPacket, 2).appendRaw(m_chain.block(b), 1).append(m_chain.details(b).totalDifficulty);

					Guard l(p->x_knownBlocks);
					p->sealAndSend(ts);
					p->m_knownBlocks.clear();
				}
			for (shared_ptr<ElementremPeer> const& p: get<1>(s))
			{
				RLPStream ts;
				p->prep(ts, NewBlockHashesPacket, blocks.size());
				for (auto const& b: blocks)
				{
					ts.appendList(2);
					ts.append(b);
					ts.append(m_chain.number(b));
				}

				Guard l(p->x_knownBlocks);
				p->sealAndSend(ts);
				p->m_knownBlocks.clear();
			}
		}
		m_latestBlockSent = _currentHash;
	}
}

BlockChainSync* ElementremHost::sync()
{
	if (m_sync)
		return m_sync.get(); // We only chose sync strategy once

	bool pv63 = false;
	foreachPeer([&](std::shared_ptr<ElementremPeer> _p)
	{
		if (_p->m_protocolVersion == protocolVersion())
			pv63 = true;
		return !pv63;
	});
	if (pv63)
	{
		m_syncStart = 0;
		m_sync.reset(new BlockChainSync(*this));
	}
	else if (!m_syncStart)
		m_syncStart = std::chrono::system_clock::to_time_t(chrono::system_clock::now());

	return m_sync.get();
}

void ElementremHost::onPeerStatus(std::shared_ptr<ElementremPeer> _peer)
{
	RecursiveGuard l(x_sync);
	if (sync())
	{
		try
		{
			sync()->onPeerStatus(_peer);
		}
		catch (FailedInvariant const&)
		{
			// "fix" for https://github.com/elementrem/webthree-umbrella/issues/300
			clog(NetWarn) << "Failed invariant during sync, restarting sync";
			sync()->restartSync();
		}
	}
}

void ElementremHost::onPeerBlockHeaders(std::shared_ptr<ElementremPeer> _peer, RLP const& _headers)
{
	RecursiveGuard l(x_sync);
	if (sync())
	{
		try
		{
			sync()->onPeerBlockHeaders(_peer, _headers);
		}
		catch (FailedInvariant const&)
		{
			// "fix" for https://github.com/elementrem/webthree-umbrella/issues/300
			clog(NetWarn) << "Failed invariant during sync, restarting sync";
			sync()->restartSync();
		}
	}
}

void ElementremHost::onPeerBlockBodies(std::shared_ptr<ElementremPeer> _peer, RLP const& _r)
{
	RecursiveGuard l(x_sync);
	if (sync())
	{
		try
		{
			sync()->onPeerBlockBodies(_peer, _r);
		}
		catch (FailedInvariant const&)
		{
			// "fix" for https://github.com/elementrem/webthree-umbrella/issues/300
			clog(NetWarn) << "Failed invariant during sync, restarting sync";
			sync()->restartSync();
		}
	}
}

void ElementremHost::onPeerNewHashes(std::shared_ptr<ElementremPeer> _peer, std::vector<std::pair<h256, u256>> const& _hashes)
{
	RecursiveGuard l(x_sync);
	if (sync())
	{
		try
		{
			sync()->onPeerNewHashes(_peer, _hashes);
		}
		catch (FailedInvariant const&)
		{
			// "fix" for https://github.com/elementrem/webthree-umbrella/issues/300
			clog(NetWarn) << "Failed invariant during sync, restarting sync";
			sync()->restartSync();
		}
	}
}

void ElementremHost::onPeerNewBlock(std::shared_ptr<ElementremPeer> _peer, RLP const& _r)
{
	RecursiveGuard l(x_sync);
	if (sync())
	{
		try
		{
			sync()->onPeerNewBlock(_peer, _r);
		}
		catch (FailedInvariant const&)
		{
			// "fix" for https://github.com/elementrem/webthree-umbrella/issues/300
			clog(NetWarn) << "Failed invariant during sync, restarting sync";
			sync()->restartSync();
		}
	}
}

void ElementremHost::onPeerTransactions(std::shared_ptr<ElementremPeer> _peer, RLP const& _r)
{
	unsigned itemCount = _r.itemCount();
	clog(ElementremHostTrace) << "Transactions (" << dec << itemCount << "entries)";
	m_tq.enqueue(_r, _peer->session()->id());
}

void ElementremHost::onPeerAborting()
{
	RecursiveGuard l(x_sync);
	try
	{
		if (m_sync)
			m_sync->onPeerAborting();
	}
	catch (Exception&)
	{
		cwarn << "Exception on peer destruciton: " << boost::current_exception_diagnostic_information();
	}
}

bool ElementremHost::isSyncing() const
{
	if (!m_sync)
		return false;
	return m_sync->isSyncing();
}

SyncStatus ElementremHost::status() const
{
	RecursiveGuard l(x_sync);
	if (!m_sync)
		return SyncStatus();
	return m_sync->status();
}

void ElementremHost::onTransactionImported(ImportResult _ir, h256 const& _h, h512 const& _nodeId)
{
	auto session = host()->peerSession(_nodeId);
	if (!session)
		return;

	std::shared_ptr<ElementremPeer> peer = session->cap<ElementremPeer>();
	if (!peer)
		peer = session->cap<ElementremPeer>(c_oldProtocolVersion);
	if (!peer)
		return;

	Guard l(peer->x_knownTransactions);
	peer->m_knownTransactions.insert(_h);
	switch (_ir)
	{
	case ImportResult::Malformed:
		peer->addRating(-100);
		break;
	case ImportResult::AlreadyKnown:
		// if we already had the transaction, then don't bother sending it on.
		DEV_GUARDED(x_transactions)
			m_transactionsSent.insert(_h);
		peer->addRating(0);
		break;
	case ImportResult::Success:
		peer->addRating(100);
		break;
	default:;
	}
}

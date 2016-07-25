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
/** @file WebThree.cpp
 * 
 * 
 */

#include "WebThree.h"
#include <chrono>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <libdevcore/Log.h>
#include <libelementrem/Defaults.h>
#include <libelementrem/ElementremHost.h>
#include <libwhisper/WhisperHost.h>
#include <libelementrem/ClientTest.h>
#include <libelhashseal/ElhashClient.h>
#if ELE_AFTER_REPOSITORY_MERGE
#include "cpp-elementrem/BuildInfo.h"
#else
#include <elementrem/BuildInfo.h>
#endif // ELE_AFTER_REPOSITORY_MERGE
#include <libelhashseal/Elhash.h>
#include "Swarm.h"
#include "Support.h"
using namespace std;
using namespace dev;
using namespace dev::p2p;
using namespace dev::ele;
using namespace dev::shh;

WebThreeDirect::WebThreeDirect(
	std::string const& _clientVersion,
	std::string const& _dbPath,
	ele::ChainParams const& _params,
	WithExisting _we,
	std::set<std::string> const& _interfaces,
	NetworkPreferences const& _n,
	bytesConstRef _network,
	bool _testing
):
	m_clientVersion(_clientVersion),
	m_net(_clientVersion, _n, _network)
{
	if (_dbPath.size())
		Defaults::setDBPath(_dbPath);
	if (_interfaces.count("ele"))
	{
		Elhash::init();
		NoProof::init();
		if (_params.sealEngineName == "Elhash")
			m_elementrem.reset(new ele::ElhashClient(_params, (int)_params.u256Param("networkID"), &m_net, shared_ptr<GasPricer>(), _dbPath, _we));
		else if (_params.sealEngineName == "NoProof" && _testing)
			m_elementrem.reset(new ele::ClientTest(_params, (int)_params.u256Param("networkID"), &m_net, shared_ptr<GasPricer>(), _dbPath, _we));
		else
			m_elementrem.reset(new ele::Client(_params, (int)_params.u256Param("networkID"), &m_net, shared_ptr<GasPricer>(), _dbPath, _we));
		string bp = DEV_QUOTED(ELE_BUILD_PLATFORM);
		vector<string> bps;
		boost::split(bps, bp, boost::is_any_of("/"));
		bps[0] = bps[0].substr(0, 5);
		bps[1] = bps[1].substr(0, 3);
		bps.back() = bps.back().substr(0, 3);
		m_elementrem->setExtraData(rlpList(0, string(dev::Version) + "++" + string(DEV_QUOTED(ELE_COMMIT_HASH)).substr(0, 4) + (ELE_CLEAN_REPO ? "-" : "*") + string(DEV_QUOTED(ELE_BUILD_TYPE)).substr(0, 1) + boost::join(bps, "/")));
	}

	if (_interfaces.count("shh"))
		m_whisper = m_net.registerCapability(make_shared<WhisperHost>());

	if (_interfaces.count("bzz"))
	{
		m_swarm.reset(new bzz::Client(this));
	}

	m_support = make_shared<Support>(this);
}

WebThreeDirect::~WebThreeDirect()
{
	// Utterly horrible right now - WebThree owns everything (good), but:
	// m_net (Host) owns the ele::ElementremHost via a shared_ptr.
	// The ele::ElementremHost depends on ele::Client (it maintains a reference to the BlockChain field of Client).
	// ele::Client (owned by us via a unique_ptr) uses ele::ElementremHost (via a weak_ptr).
	// Really need to work out a clean way of organising ownership and guaranteeing startup/shutdown is perfect.

	// Have to call stop here to get the Host to kill its io_service otherwise we end up with left-over reads,
	// still referencing Sessions getting deleted *after* m_elementrem is reset, causing bad things to happen, since
	// the guarantee is that m_elementrem is only reset *after* all sessions have ended (sessions are allowed to
	// use bits of data owned by m_elementrem).
	m_net.stop();
	m_elementrem.reset();
}

bzz::Interface* WebThreeDirect::swarm() const
{
	if (!m_swarm)
		BOOST_THROW_EXCEPTION(InterfaceNotSupported("bzz"));
	return m_swarm.get();
}

std::string WebThreeDirect::composeClientVersion(std::string const& _client)
{
	return _client + "/" + \
		"v" + dev::Version + "/" + \
		DEV_QUOTED(ELE_BUILD_OS) + "/" + \
		DEV_QUOTED(ELE_BUILD_COMPILER) + "/" + \
		DEV_QUOTED(ELE_BUILD_JIT_MODE) + "/" + \
		DEV_QUOTED(ELE_BUILD_TYPE) + "/" + \
		string(DEV_QUOTED(ELE_COMMIT_HASH)).substr(0, 8) + \
		(ELE_CLEAN_REPO ? "" : "*") + "/";
}

p2p::NetworkPreferences const& WebThreeDirect::networkPreferences() const
{
	return m_net.networkPreferences();
}

void WebThreeDirect::setNetworkPreferences(p2p::NetworkPreferences const& _n, bool _dropPeers)
{
	auto had = isNetworkStarted();
	if (had)
		stopNetwork();
	m_net.setNetworkPreferences(_n, _dropPeers);
	if (had)
		startNetwork();
}

std::vector<PeerSessionInfo> WebThreeDirect::peers()
{
	return m_net.peerSessionInfo();
}

size_t WebThreeDirect::peerCount() const
{
	return m_net.peerCount();
}

void WebThreeDirect::setIdealPeerCount(size_t _n)
{
	return m_net.setIdealPeerCount(_n);
}

void WebThreeDirect::setPeerStretch(size_t _n)
{
	return m_net.setPeerStretch(_n);
}

bytes WebThreeDirect::saveNetwork()
{
	return m_net.saveNetwork();
}

void WebThreeDirect::addNode(NodeID const& _node, bi::tcp::endpoint const& _host)
{
	m_net.addNode(_node, NodeIPEndpoint(_host.address(), _host.port(), _host.port()));
}

void WebThreeDirect::requirePeer(NodeID const& _node, bi::tcp::endpoint const& _host)
{
	m_net.requirePeer(_node, NodeIPEndpoint(_host.address(), _host.port(), _host.port()));
}

void WebThreeDirect::addPeer(NodeSpec const& _s, PeerType _t)
{
	m_net.addPeer(_s, _t);
}


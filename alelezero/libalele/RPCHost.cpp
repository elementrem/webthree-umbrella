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
/** @file RPCHost.h
 * 
 * 
 */

#include <libdevcore/Log.h>
#include <libweb3jsonrpc/ModularServer.h>
#include <libweb3jsonrpc/MemoryDB.h>
#include <libweb3jsonrpc/SafeHttpServer.h>
#include <libweb3jsonrpc/Net.h>
#include <libweb3jsonrpc/Bzz.h>
#include <libweb3jsonrpc/Web3.h>
#include <libweb3jsonrpc/Ele.h>
#include <libweb3jsonrpc/SessionManager.h>
#include <libweb3jsonrpc/Personal.h>
#include <libweb3jsonrpc/AdminEle.h>
#include <libweb3jsonrpc/AdminNet.h>
#include <libweb3jsonrpc/AdminUtils.h>
#include <libweb3jsonrpc/Debug.h>
#include <libwebthree/WebThree.h>
#include <libelementrem/GasPricer.h>
#include "RPCHost.h"
#include "AleleWhisper.h"
#include "AleleFace.h"
#include "AccountHolder.h"

using namespace std;
using namespace dev;
using namespace ele;
using namespace alele;

void RPCHost::init(AleleFace* _alele)
{
	m_accountHolder.reset(new AccountHolder(_alele));
	m_sm.reset(new rpc::SessionManager());
	m_eleFace = new rpc::Ele(*_alele->web3()->elementrem(), *m_accountHolder.get());
	m_whisperFace = new AleleWhisper(*_alele->web3(), {});
	auto adminEle = new rpc::AdminEle(*_alele->web3()->elementrem(), *static_cast<TrivialGasPricer*>(_alele->elementrem()->gasPricer().get()), _alele->keyManager(), *m_sm.get());
	m_rpcServer.reset(new ModularServer<
		rpc::EleFace, rpc::DBFace, rpc::WhisperFace,
		rpc::NetFace, rpc::BzzFace, rpc::Web3Face,
		rpc::PersonalFace, rpc::AdminEleFace, rpc::AdminNetFace,
		rpc::AdminUtilsFace, rpc::DebugFace
	>(
		m_eleFace, new rpc::MemoryDB(), m_whisperFace,
		new rpc::Net(*_alele->web3()), new rpc::Bzz(*_alele->web3()->swarm()), new rpc::Web3(_alele->web3()->clientVersion()),
		new rpc::Personal(_alele->keyManager(), *m_accountHolder), adminEle, new rpc::AdminNet(*_alele->web3(), *m_sm.get()),
		new rpc::AdminUtils(*m_sm.get()), new rpc::Debug(*_alele->web3()->elementrem())
	));
	m_httpConnectorId = m_rpcServer->addConnector(new dev::SafeHttpServer(7075, "", "", 4));
	m_ipcConnectorId = m_rpcServer->addConnector(new dev::IpcServer("gele"));
	m_rpcServer->StartListening();
}

SafeHttpServer* RPCHost::httpConnector() const
{
	return static_cast<dev::SafeHttpServer*>(m_rpcServer->connector(m_httpConnectorId));
}

IpcServer* RPCHost::ipcConnector() const
{
	return static_cast<dev::IpcServer*>(m_rpcServer->connector(m_ipcConnectorId));
}

alele::AccountHolder* RPCHost::accountHolder() const
{
	return m_accountHolder.get();
}

rpc::SessionManager* RPCHost::sessionManager() const
{
	return m_sm.get();
}

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

#pragma once

#include <memory>
#include <libdevcore/Common.h>
#include <libweb3jsonrpc/IpcServer.h>

template <class... Is> class ModularServer;

namespace dev
{

class SafeHttpServer;

namespace rpc
{
class EleFace;
class DBFace;
class WhisperFace;
class NetFace;
class BzzFace;
class Web3Face;
class SessionManager;
class PersonalFace;
class AdminEleFace;
class AdminNetFace;
class AdminUtilsFace;
class DebugFace;
}

namespace alele
{

class AleleFace;
class AleleWhisper;
class AccountHolder;

class RPCHost
{
public:
	RPCHost() = default;
	RPCHost(AleleFace* _alele) { init(_alele); }

	void init(AleleFace* _alele);
	rpc::EleFace* eleFace() const { return m_eleFace; }
	AleleWhisper* whisperFace() const { return m_whisperFace; }
	SafeHttpServer* httpConnector() const;
	IpcServer* ipcConnector() const;
	AccountHolder* accountHolder() const;
	rpc::SessionManager* sessionManager() const;

private:
	rpc::EleFace* m_eleFace;
	AleleWhisper* m_whisperFace;
	std::shared_ptr<ModularServer<rpc::EleFace, rpc::DBFace, rpc::WhisperFace,
	rpc::NetFace, rpc::BzzFace, rpc::Web3Face, rpc::PersonalFace,
	rpc::AdminEleFace, rpc::AdminNetFace, rpc::AdminUtilsFace,
	rpc::DebugFace>> m_rpcServer;
	unsigned m_httpConnectorId;
	unsigned m_ipcConnectorId;
	std::shared_ptr<AccountHolder> m_accountHolder;
	std::shared_ptr<rpc::SessionManager> m_sm;
};

}
}

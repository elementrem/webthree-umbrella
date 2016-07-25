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
/** @file Web3Server.h.cpp
 * @
 * 
 * Elementrem IDE client.
 */

#include <libdevcore/Exceptions.h>
#include <libdevcore/Log.h>
#include <libelementrem/Interface.h>
#include <libwebthree/WebThree.h>
#include <libweb3jsonrpc/AccountHolder.h>
#include "Web3Server.h"

using namespace dev::mix;
using namespace dev;
using namespace std;

namespace
{
class EmptyNetwork : public dev::NetworkFace
{
	vector<p2p::PeerSessionInfo> peers() override
	{
		return vector<p2p::PeerSessionInfo>();
	}

	size_t peerCount() const override
	{
		return 0;
	}

	void addNode(p2p::NodeID const& _node, bi::tcp::endpoint const& _hostEndpoint) override
	{
		(void)_node;
		(void)_hostEndpoint;
	}

	void addPeer(p2p::NodeSpec const& _node, p2p::PeerType _t) override
	{
		(void)_node;
		(void)_t;
	}


	void requirePeer(p2p::NodeID const& _node, bi::tcp::endpoint const& _endpoint) override
	{
		(void)_node;
		(void)_endpoint;
	}

	dev::bytes saveNetwork() override
	{
		return dev::bytes();
	}

	void setIdealPeerCount(size_t _n) override
	{
		(void)_n;
	}

	bool haveNetwork() const override
	{
		return false;
	}

	p2p::NetworkPreferences const& networkPreferences() const override
	{
		static const p2p::NetworkPreferences c_ret;
		return c_ret;
	}

	void setNetworkPreferences(p2p::NetworkPreferences const& _n, bool _dropPeers) override
	{
		(void)_n;
		(void)_dropPeers;
	}

	p2p::NodeInfo nodeInfo() const override { return p2p::NodeInfo(); }

	string enode() const override { return ""; }

	p2p::NodeID id() const override
	{
		return p2p::NodeID();
	}

	p2p::Peers nodes() const override
	{
		return p2p::Peers();
	}

	void startNetwork() override
	{
	}

	void stopNetwork() override
	{
	}

	bool isNetworkStarted() const override
	{
		return false;
	}
};

}

Web3Server::Web3Server(ele::Interface& _client, ele::AccountHolder& _eleAccounts):
	rpc::Ele(_client, _eleAccounts)
{
}

Web3Server::~Web3Server()
{
}

Json::Value Web3Server::ele_getFilterChanges(string const& _filterId)
{
	return rpc::Ele::ele_getFilterChanges(_filterId);
}

string Web3Server::ele_sendTransaction(Json::Value const& _json)
{
	string ret = rpc::Ele::ele_sendTransaction(_json);
	emit newTransaction();
	return ret;
}

string Web3Server::ele_call(Json::Value const& _json, string const& _blockNumber)
{
	string ret = rpc::Ele::ele_call(_json, _blockNumber);
	emit newTransaction();
	return ret;
}

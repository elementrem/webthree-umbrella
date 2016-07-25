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
/** @file Web3Server.h
 * @
 * 
 * Elementrem IDE client.
 */

#pragma once

#include <map>
#include <string>
#include <QObject>
#include <libweb3jsonrpc/AccountHolder.h>
#include <libweb3jsonrpc/Ele.h>

namespace dev
{

namespace mix
{

class Web3Server: public QObject, public rpc::Ele
{
	Q_OBJECT

public:
	Web3Server(ele::Interface& _client, ele::AccountHolder& _eleAccounts);
	virtual ~Web3Server();

signals:
	void newTransaction();

protected:
	virtual Json::Value ele_getFilterChanges(std::string const& _filterId) override;
	virtual std::string ele_sendTransaction(Json::Value const& _json) override;
	virtual std::string ele_call(Json::Value const& _json, std::string const& _blockNumber) override;
};

}
}

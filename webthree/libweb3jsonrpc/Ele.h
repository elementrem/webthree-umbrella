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
/** @file Ele.h
 * @authors:
 *   
 *   
 * 
 */

#pragma once

#include <memory>
#include <iostream>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/common/exception.h>
#include <libdevcrypto/Common.h>
#include "SessionManager.h"
#include "EleFace.h"


namespace dev
{
class NetworkFace;
class KeyPair;
namespace ele
{
class AccountHolder;
struct TransactionSkeleton;
class Interface;
}
namespace shh
{
class Interface;
}

extern const unsigned SensibleHttpThreads;
extern const unsigned SensibleHttpPort;

}

namespace dev
{

namespace rpc
{

/**
 * @brief JSON-RPC api implementation
 */
class Ele: public dev::rpc::EleFace
{
public:
	Ele(ele::Interface& _ele, ele::AccountHolder& _eleAccounts);

	virtual RPCModules implementedModules() const override
	{
		return RPCModules{RPCModule{"ele", "1.0"}};
	}

	ele::AccountHolder const& eleAccounts() const { return m_eleAccounts; }

	virtual std::string ele_protocolVersion() override;
	virtual std::string ele_hashrate() override;
	virtual std::string ele_coinbase() override;
	virtual bool ele_mining() override;
	virtual std::string ele_gasPrice() override;
	virtual Json::Value ele_accounts() override;
	virtual std::string ele_blockNumber()override;
	virtual std::string ele_getBalance(std::string const& _address, std::string const& _blockNumber) override;
	virtual std::string ele_getStorageAt(std::string const& _address, std::string const& _position, std::string const& _blockNumber) override;
	virtual std::string ele_getTransactionCount(std::string const& _address, std::string const& _blockNumber) override;
	virtual std::string ele_pendingTransactions() override;
	virtual Json::Value ele_getBlockTransactionCountByHash(std::string const& _blockHash) override;
	virtual Json::Value ele_getBlockTransactionCountByNumber(std::string const& _blockNumber) override;
	virtual Json::Value ele_getUncleCountByBlockHash(std::string const& _blockHash) override;
	virtual Json::Value ele_getUncleCountByBlockNumber(std::string const& _blockNumber) override;
	virtual std::string ele_getCode(std::string const& _address, std::string const& _blockNumber) override;
	virtual std::string ele_sendTransaction(Json::Value const& _json) override;
	virtual std::string ele_call(Json::Value const& _json, std::string const& _blockNumber) override;
	virtual std::string ele_estimateGas(Json::Value const& _json) override;
	virtual bool ele_flush() override;
	virtual Json::Value ele_getBlockByHash(std::string const& _blockHash, bool _includeTransactions) override;
	virtual Json::Value ele_getBlockByNumber(std::string const& _blockNumber, bool _includeTransactions) override;
	virtual Json::Value ele_getTransactionByHash(std::string const& _transactionHash) override;
	virtual Json::Value ele_getTransactionByBlockHashAndIndex(std::string const& _blockHash, std::string const& _transactionIndex) override;
	virtual Json::Value ele_getTransactionByBlockNumberAndIndex(std::string const& _blockNumber, std::string const& _transactionIndex) override;
	virtual Json::Value ele_getTransactionReceipt(std::string const& _transactionHash) override;
	virtual Json::Value ele_getUncleByBlockHashAndIndex(std::string const& _blockHash, std::string const& _uncleIndex) override;
	virtual Json::Value ele_getUncleByBlockNumberAndIndex(std::string const& _blockNumber, std::string const& _uncleIndex) override;
	virtual std::string ele_newFilter(Json::Value const& _json) override;
	virtual std::string ele_newFilterEx(Json::Value const& _json) override;
	virtual std::string ele_newBlockFilter() override;
	virtual std::string ele_newPendingTransactionFilter() override;
	virtual bool ele_uninstallFilter(std::string const& _filterId) override;
	virtual Json::Value ele_getFilterChanges(std::string const& _filterId) override;
	virtual Json::Value ele_getFilterChangesEx(std::string const& _filterId) override;
	virtual Json::Value ele_getFilterLogs(std::string const& _filterId) override;
	virtual Json::Value ele_getFilterLogsEx(std::string const& _filterId) override;
	virtual Json::Value ele_getLogs(Json::Value const& _json) override;
	virtual Json::Value ele_getLogsEx(Json::Value const& _json) override;
	virtual Json::Value ele_getWork() override;
	virtual bool ele_submitWork(std::string const& _nonce, std::string const&, std::string const& _mixHash) override;
	virtual bool ele_submitHashrate(std::string const& _hashes, std::string const& _id) override;
	virtual std::string ele_register(std::string const& _address) override;
	virtual bool ele_unregister(std::string const& _accountId) override;
	virtual Json::Value ele_fetchQueuedTransactions(std::string const& _accountId) override;
	virtual std::string ele_signTransaction(Json::Value const& _transaction) override;
	virtual Json::Value ele_inspectTransaction(std::string const& _rlp) override;
	virtual std::string ele_sendRawTransaction(std::string const& _rlp) override;
	virtual bool ele_notePassword(std::string const&) override { return false; }
	virtual Json::Value ele_syncing() override;
	
	void setTransactionDefaults(ele::TransactionSkeleton& _t);
protected:

	ele::Interface* client() { return &m_ele; }
	
	ele::Interface& m_ele;
	ele::AccountHolder& m_eleAccounts;

};

}
} //namespace dev

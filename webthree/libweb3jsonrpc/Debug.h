#pragma once
#include <libelementrem/Executive.h>
#include "DebugFace.h"

namespace dev
{

namespace ele
{
class Client;
}

namespace rpc
{

class SessionManager;

class Debug: public DebugFace
{
public:
	explicit Debug(ele::Client& _ele);

	virtual RPCModules implementedModules() const override
	{
		return RPCModules{RPCModule{"debug", "1.0"}};
	}


	virtual Json::Value debug_traceTransaction(std::string const& _txHash, Json::Value const& _json) override;
	virtual Json::Value debug_traceBlockByNumber(int _blockNumber, Json::Value const& _json) override;
	virtual Json::Value debug_traceBlockByHash(std::string const& _blockHash, Json::Value const& _json) override;
	virtual Json::Value debug_storageAt(std::string const& _blockHashOrNumber, int _txIndex, std::string const& _address) override;

private:

	ele::Client& m_ele;
	h256 blockHash(std::string const& _blockHashOrNumber) const;
	Json::Value traceTransaction(dev::ele::Executive& _e, dev::ele::Transaction const& _t, Json::Value const& _json);
	Json::Value traceBlock(dev::ele::Block const& _block, Json::Value const& _json);
};

}
}

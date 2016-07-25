#pragma once
#include "AdminEleFace.h"

namespace dev
{

namespace ele
{
class Client;
class TrivialGasPricer;
class KeyManager;
}

namespace rpc
{

class SessionManager;

class AdminEle: public AdminEleFace
{
public:
	AdminEle(ele::Client& _ele, ele::TrivialGasPricer& _gp, ele::KeyManager& _keyManager, SessionManager& _sm);

	virtual RPCModules implementedModules() const override
	{
		return RPCModules{RPCModule{"admin", "1.0"}, RPCModule{"miner", "1.0"}};
	}

	virtual bool admin_ele_setMining(bool _on, std::string const& _session) override;
	virtual Json::Value admin_ele_blockQueueStatus(std::string const& _session) override;
	virtual bool admin_ele_setAskPrice(std::string const& _mey, std::string const& _session) override;
	virtual bool admin_ele_setBidPrice(std::string const& _mey, std::string const& _session) override;
	virtual Json::Value admin_ele_findBlock(std::string const& _blockHash, std::string const& _session) override;
	virtual std::string admin_ele_blockQueueFirstUnknown(std::string const& _session) override;
	virtual bool admin_ele_blockQueueRetryUnknown(std::string const& _session) override;
	virtual Json::Value admin_ele_allAccounts(std::string const& _session) override;
	virtual Json::Value admin_ele_newAccount(const Json::Value& _info, std::string const& _session) override;
	virtual bool admin_ele_setMiningBenefactor(std::string const& _uuidOrAddress, std::string const& _session) override;
	virtual Json::Value admin_ele_inspect(std::string const& _address, std::string const& _session) override;
	virtual Json::Value admin_ele_reprocess(std::string const& _blockNumberOrHash, std::string const& _session) override;
	virtual Json::Value admin_ele_vmTrace(std::string const& _blockNumberOrHash, int _txIndex, std::string const& _session) override;
	virtual Json::Value admin_ele_getReceiptByHashAndIndex(std::string const& _blockNumberOrHash, int _txIndex, std::string const& _session) override;
	virtual bool miner_start(int _threads) override;
	virtual bool miner_stop() override;
	virtual bool miner_setElementbase(std::string const& _uuidOrAddress) override;
	virtual bool miner_setExtra(std::string const& _extraData) override;
	virtual bool miner_setGasPrice(std::string const& _gasPrice) override;
	virtual std::string miner_hashrate() override;

	virtual void setMiningBenefactorChanger(std::function<void(Address const&)> const& _f) { m_setMiningBenefactor = _f; }
private:
	ele::Client& m_ele;
	ele::TrivialGasPricer& m_gp;
	ele::KeyManager& m_keyManager;
	SessionManager& m_sm;
	std::function<void(Address const&)> m_setMiningBenefactor;

	h256 blockHash(std::string const& _blockNumberOrHash) const;
};

}
}

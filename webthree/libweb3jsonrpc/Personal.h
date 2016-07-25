#pragma once
#include "PersonalFace.h"

namespace dev
{
	
namespace ele
{
class KeyManager;
class AccountHolder;
}

namespace rpc
{

class Personal: public dev::rpc::PersonalFace
{
public:
	Personal(dev::ele::KeyManager& _keyManager, dev::ele::AccountHolder& _accountHolder);
	virtual RPCModules implementedModules() const override
	{
		return RPCModules{RPCModule{"personal", "1.0"}};
	}
	virtual std::string personal_newAccount(std::string const& _password) override;
	virtual bool personal_unlockAccount(std::string const& _address, std::string const& _password, int _duration) override;
	virtual Json::Value personal_listAccounts() override;

private:
	dev::ele::KeyManager& m_keyManager;
	dev::ele::AccountHolder& m_accountHolder;
};

}
}

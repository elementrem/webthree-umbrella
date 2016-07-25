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
/** @file AccountHolder.h
 * 
 * 
 */

#pragma once

#include <queue>
#include <unordered_map>
#include <QObject>
#include <libdevcore/Guards.h>
#include <libweb3jsonrpc/AccountHolder.h>

namespace dev
{
namespace alele
{

class AleleFace;

class AccountHolder: public QObject, public ele::AccountHolder
{
	Q_OBJECT

public:
	AccountHolder(AleleFace* _alele);

	void setEnabled(bool _e) { m_isEnabled = _e; }
	bool isEnabled() const { return m_isEnabled; }

private slots:
	void doValidations();

protected:
	// easiest to return keyManager.addresses();
	virtual dev::AddressHash realAccounts() const override;
	// use web3 to submit a signed transaction to accept
	virtual ele::TransactionNotification authenticate(ele::TransactionSkeleton const& _t) override;

	void timerEvent(QTimerEvent*) override;

private:
	std::pair<bool, std::string> getUserNotice(ele::TransactionSkeleton const& _t);
	bool showAuthenticationPopup(std::string const& _title, std::string const& _text);
	bool validateTransaction(ele::TransactionSkeleton const& _t, bool _toProxy);

	bool m_isEnabled = true;

	std::queue<std::pair<ele::TransactionSkeleton, unsigned>> m_queued;
	std::unordered_map<unsigned, ele::TransactionNotification> m_queueOutput;
	unsigned m_nextQueueID = 0;
	Mutex x_queued;
	std::condition_variable m_queueCondition;

	AleleFace* m_alele;
};

}
}

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

#include "AccountHolder.h"
#include <QMessageBox>
#include <QAbstractButton>
#include <libdevcore/Log.h>
#include <libelecore/KeyManager.h>
#include <libwebthree/WebThree.h>
#include "AleleFace.h"
using namespace std;
using namespace dev;
using namespace ele;
using namespace alele;

alele::AccountHolder::AccountHolder(AleleFace* _alele):
	ele::AccountHolder([=](){ return _alele->elementrem(); }),
	m_alele(_alele)
{
	startTimer(500);
}

bool alele::AccountHolder::showAuthenticationPopup(string const& _title, string const& _text)
{
	QMessageBox userInput;
	userInput.setText(QString::fromStdString(_title));
	userInput.setInformativeText(QString::fromStdString(_text));
	userInput.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	userInput.button(QMessageBox::Ok)->setText("Allow");
	userInput.button(QMessageBox::Cancel)->setText("Reject");
	userInput.setDefaultButton(QMessageBox::Cancel);
	return userInput.exec() == QMessageBox::Ok;
}

TransactionNotification alele::AccountHolder::authenticate(TransactionSkeleton const& _t)
{
	UniqueGuard l(x_queued);
	auto id = m_nextQueueID++;
	m_queued.push(make_pair(_t, id));
	while (!m_queueOutput.count(id))
		m_queueCondition.wait(l);
	TransactionNotification ret = m_queueOutput[id];
	m_queueOutput.erase(id);
	return ret;
}

void alele::AccountHolder::doValidations()
{
	Guard l(x_queued);
	while (!m_queued.empty())
	{
		TransactionSkeleton t;
		unsigned id;
		tie(t, id) = m_queued.front();
		m_queued.pop();

		TransactionNotification n;
		bool proxy = isProxyAccount(t.from);
		if (!proxy && !isRealAccount(t.from))
		{
			cwarn << "Trying to send from non-existant account" << t.from;
			n.r = TransactionRepercussion::UnknownAccount;
		}
		else
		{
			// TODO: determine gas price.
			if (validateTransaction(t, proxy))
			{
				if (proxy)
				{
					queueTransaction(t);
					n.r = TransactionRepercussion::ProxySuccess;
				}
				else
				{
					// sign and submit.
					if (Secret s = m_alele->retrieveSecret(t.from))
					{
						tie(n.hash, n.created) = m_alele->elementrem()->submitTransaction(t, s);
						n.r = TransactionRepercussion::Success;
					}
					else
						n.r = TransactionRepercussion::Locked;
				}
			}
			else
			    n.r = TransactionRepercussion::Refused;
		}

		m_queueOutput[id] = n;
	}
	m_queueCondition.notify_all();
}

AddressHash alele::AccountHolder::realAccounts() const
{
	return m_alele->keyManager().accountsHash();
}

bool alele::AccountHolder::validateTransaction(TransactionSkeleton const& _t, bool _toProxy)
{
	if (!m_isEnabled)
		return true;

	return showAuthenticationPopup("Transaction", _t.userReadable(_toProxy,
		[&](TransactionSkeleton const& _t) -> pair<bool, string>
		{
			h256 contractCodeHash = m_alele->elementrem()->postState().codeHash(_t.to);
			if (contractCodeHash == EmptySHA3)
				return make_pair(false, std::string());
			string userNotice = m_alele->natSpec().userNotice(contractCodeHash, _t.data);
			return std::make_pair(true, userNotice);
		},
		[&](Address const& _a) -> string { return m_alele->toReadable(_a); }
	));
}


void alele::AccountHolder::timerEvent(QTimerEvent*)
{
	doValidations();
}

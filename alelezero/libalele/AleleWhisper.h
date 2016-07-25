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
/** @file WebThreeServer.h
 * 
 * 
 */

#include <queue>
#include <QtCore/QObject>
#include <libdevcore/Guards.h>
#include <libelecore/CommonJS.h>
#include <libdevcrypto/Common.h>
#include <libweb3jsonrpc/Whisper.h>
#include "AccountHolder.h"

namespace dev
{

namespace alele
{

class AccountHolder;

class AleleWhisper: public QObject, public rpc::Whisper
{
	Q_OBJECT

public:
	AleleWhisper(WebThreeDirect& _web3, std::vector<dev::KeyPair> const& _accounts): rpc::Whisper(_web3, _accounts) {}
	virtual std::string shh_newIdentity() override;

signals:
	void onNewId(QString _s);
};

}
}

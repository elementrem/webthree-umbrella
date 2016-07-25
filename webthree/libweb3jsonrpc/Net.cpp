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
/** @file Net.cpp
 * @authors:
 *   
 *   
 * 
 */

#include <libwebthree/WebThree.h>
#include <libdevcore/CommonJS.h>
#include <libelecore/Common.h>
#include "Net.h"

using namespace dev;
using namespace dev::rpc;

Net::Net(NetworkFace& _network): m_network(_network) {}

std::string Net::net_version()
{
	return toString(m_network.networkId());
}

std::string Net::net_peerCount()
{
	return toJS(m_network.peerCount());
}

bool Net::net_listening()
{
	return m_network.isNetworkStarted();
}

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
/** @file Bzz.cpp
 * @authors:
 *   
 *   
 * 
 */

#include <libdevcore/SHA3.h>
#include <libdevcore/CommonData.h>
#include <libwebthree/Swarm.h>
#include <libdevcore/CommonIO.h>
#include <libelecore/CommonJS.h>
#include <libdevcore/TrieCommon.h>	// TODO: bytes ostream operator doesn't work without this import
#include "Bzz.h"

using namespace std;
using namespace dev;
using namespace dev::rpc;

Bzz::Bzz(dev::bzz::Interface& _bzz): m_bzz(_bzz){}

std::string Bzz::bzz_put(std::string const& _data)
{
	bytes b = jsToBytes(_data);
	m_bzz.put(b);
	return toJS(sha3(b));
}

std::string Bzz::bzz_get(std::string const& _hash)
{
	return toJS(static_cast<bytesConstRef>(m_bzz.get(jsToFixed<32>(_hash))));
}
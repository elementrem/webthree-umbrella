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
/** @file Debugger.h
 * 
 * 
 */

#pragma once

#include <string>
#include <vector>
#include <libelecore/Common.h>

namespace dev
{

namespace ele { struct StateDiff; }

namespace alele
{

class Context
{
public:
	virtual ~Context();

	std::string toHTML(u256 const& _n) const;

	virtual Address toAddress(std::string const&) const = 0;
	virtual std::string toName(Address const&) const = 0;
	std::string toReadable(Address const& _a) const;
	virtual Addresses allKnownAddresses() const = 0;
};

}
}

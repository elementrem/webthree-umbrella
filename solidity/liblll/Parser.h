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
/** @file Parser.h
 * 
 * 
 */

#pragma once

#include <string>
#include <vector>
#include <libdevcore/Common.h>

namespace boost { namespace spirit { class utree; } }
namespace sp = boost::spirit;

namespace dev
{
namespace ele
{

void killBigints(sp::utree const& _this);
void parseTreeLLL(std::string const& _s, sp::utree& o_out);
void debugOutAST(std::ostream& _out, sp::utree const& _this);

}
}

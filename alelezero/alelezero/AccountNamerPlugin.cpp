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
/** @file AccountNamerPlugin.h
 * 
 * 
 */

#include "AccountNamerPlugin.h"
#include <libdevcore/Log.h>
#include <libalele/AleleFace.h>
using namespace std;
using namespace dev;
using namespace alele;
using namespace zero;

AccountNamerPlugin::AccountNamerPlugin(ZeroFace* _z, std::string const& _name):
	Plugin(_z, _name)
{
	alele()->install(this);
}

AccountNamerPlugin::~AccountNamerPlugin()
{
	alele()->uninstall(this);
}

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
/** @file TestHelper.cpp
 * @author Marko Simovic <markobarko@gmail.com>
 * 
 */

#include "TestHelper.h"

using namespace std;

namespace dev
{
namespace test
{

namespace
{
	Listener* g_listener;
}

void Listener::registerListener(Listener& _listener)
{
	g_listener = &_listener;
}

void Listener::notifySuiteStarted(std::string const& _name)
{
	if (g_listener)
		g_listener->suiteStarted(_name);
}

void Listener::notifyTestStarted(std::string const& _name)
{
	if (g_listener)
		g_listener->testStarted(_name);
}

void Listener::notifyTestFinished()
{
	if (g_listener)
		g_listener->testFinished();
}

} } // namespaces

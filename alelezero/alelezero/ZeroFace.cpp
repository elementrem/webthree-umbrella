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
/** @file ZeroFace.cpp
 * 
 * 
 */

#include "ZeroFace.h"
#include <QMenu>
#include <libdevcore/Log.h>
#include <libelecore/ICAP.h>
#include <libelementrem/Client.h>
#include <libalele/AleleFace.h>
#include "Plugin.h"
using namespace std;
using namespace dev;
using namespace shh;
using namespace ele;
using namespace alele;
using namespace zero;

unordered_map<string, PluginFactory>* ZeroFace::s_linkedPlugins = nullptr;

void ZeroFace::notePlugin(string const& _name, PluginFactory const& _new)
{
	if (!s_linkedPlugins)
		s_linkedPlugins = new std::unordered_map<string, PluginFactory>();
	s_linkedPlugins->insert(make_pair(_name, _new));
}

void ZeroFace::unnotePlugin(string const& _name)
{
	if (s_linkedPlugins)
		s_linkedPlugins->erase(_name);
}

void ZeroFace::adoptPlugin(Plugin* _p)
{
	m_plugins[_p->name()] = shared_ptr<Plugin>(_p);
}

void ZeroFace::killPlugins()
{
	m_plugins.clear();
}

void ZeroFace::noteAllChange()
{
	for (auto const& p: m_plugins)
		p.second->onAllChange();
}

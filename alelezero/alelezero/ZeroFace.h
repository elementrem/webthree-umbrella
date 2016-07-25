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
/** @file AleleFace.h
 * 
 * 
 */

#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <QtWidgets/QMainWindow>
#include <libalele/Common.h>

class QSettings;

namespace dev
{

class SafeHttpServer;

namespace rpc { class SessionManager; }

namespace alele
{

class AleleFace;
class WebThreeServer;
class AleleWhisper;

namespace zero
{

class Plugin;
class ZeroFace;
class SettingsPage;

using PluginFactory = std::function<Plugin*(ZeroFace*)>;

class ZeroFace: public QMainWindow
{
	Q_OBJECT

	friend class Plugin; // Plugin::noteSettingsChanged() calls ZeroFace::writeSettings().

public:
	explicit ZeroFace(QWidget* _parent = nullptr): QMainWindow(_parent) {}
	virtual ~ZeroFace()
	{
		killPlugins();
	}

	// Plugin API
	static void notePlugin(std::string const& _name, PluginFactory const& _new);
	static void unnotePlugin(std::string const& _name);
	void adoptPlugin(Plugin* _p);
	void killPlugins();
	void noteAllChange();
	virtual void addSettingsPage(int _index, QString const& _categoryName, std::function<SettingsPage*()> const& _pageFactory) = 0;
	std::shared_ptr<Plugin> findPlugin(std::string const& _name) { auto it = m_plugins.find(_name); return it != m_plugins.end() ? it->second : nullptr; }

	virtual AleleFace const* alele() const = 0;
	virtual AleleFace* alele() = 0;

	virtual SafeHttpServer* web3ServerConnector() const = 0;
	virtual AleleWhisper* whisperFace() const = 0;
	virtual rpc::SessionManager* sessionManager() const = 0;

	virtual void allStop() = 0;
	virtual void carryOn() = 0;
	virtual void writeSettings() = 0;

protected:
	template <class F> void forEach(F const& _f) { for (auto const& p: m_plugins) _f(p.second); }
	std::shared_ptr<Plugin> takePlugin(std::string const& _name) { auto it = m_plugins.find(_name); std::shared_ptr<Plugin> ret; if (it != m_plugins.end()) { ret = it->second; m_plugins.erase(it); } return ret; }

	static std::unordered_map<std::string, PluginFactory>* s_linkedPlugins;


private:
	std::unordered_map<std::string, std::shared_ptr<Plugin>> m_plugins;
};

}
}
}

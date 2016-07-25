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

/** @file NetworkSettings.h
 * 
 * 
 */

#pragma once

#include "Plugin.h"
#include <libp2p/Network.h>

namespace Ui { class NetworkSettings; }

namespace dev
{
namespace alele
{
namespace zero
{

struct NetworkSettings
{
	p2p::NetworkPreferences p2pSettings;
	QString clientName;
	bool hermitMode;
	bool dropPeers;
	int idealPeers;

	void write(QSettings& _settings) const;
	void read(QSettings const& _settings);
};

class NetworkSettingsPage: public SettingsPage
{
public:
	NetworkSettingsPage();
	NetworkSettings prefs() const;
	void setPrefs(NetworkSettings const& _settings);

private:
	Ui::NetworkSettings* m_ui;
};

}
}
}

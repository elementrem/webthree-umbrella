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
/** @file CopyButton.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.1
import Qt.labs.settings 1.0

DefaultButton
{
	property var getContent;
	Image
	{
		width: 20
		height: 20
		anchors.centerIn: parent
		source: "qrc:/qml/img/copyiconactive.png"
	}
	tooltip: qsTr("Copy to clipboard")
	width: 35
	height: 28
	onClicked:
	{
		clipboard.text = getContent()
	}
}


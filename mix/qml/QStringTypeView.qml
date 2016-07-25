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
/** @file QStringTypeView.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.0
import QtQuick.Controls 1.1

Item
{
	property alias value: textinput.text
	property alias readOnly: textinput.readOnly
	id: editRoot
	width: 350

	function init()
	{
		textinput.cursorPosition = 0
	}

	DefaultTextField {
		anchors.verticalCenter: parent.verticalCenter
		id: textinput
		selectByMouse: true
		text: value
		width: 350
		MouseArea {
			id: mouseArea
			anchors.fill: parent
			hoverEnabled: true
			onClicked: textinput.forceActiveFocus()
		}
	}
}




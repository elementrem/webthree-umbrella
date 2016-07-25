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
/** @file ScenarioButton.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.1
import QtGraphicalEffects 1.0

Rectangle {
	id: buttonActionContainer
	property string text
	property string buttonShortcut
	property string sourceImg
	property alias roundLeft: left.visible
	property alias roundRight: right.visible
	property alias direction: blinkTimer.direction
	property alias index: blinkTimer.index
	property string color: "white"
	property string disabledColor: "#e7e7e7"
	property alias isBlinking: blinkTimer.running
	property alias displayLabel: labelCont.visible
	signal clicked
	onEnabledChanged:
	{
		if (enabled)
			updateColor(color)
		else
			updateColor(disabledColor)
	}

	function startBlinking()
	{
		if (!blinkTimer.running)
			blinkTimer.start()
	}

	function stopBlinking()
	{
		blinkTimer.stop()
	}

	function setBlinking(_index, _direction, _color)
	{
		blinkTimer.index = _index
		blinkTimer.direction = _direction
		updateColor(_color)
	}

	function updateColor(_color)
	{
		left.color = _color
		right.color = _color
		contentRectangle.color = _color
	}

	onColorChanged:
	{
		updateColor(buttonActionContainer.color)
	}

	Rectangle
	{
		id: left
		width: 10
		height: parent.height
		anchors.left: contentRectangle.left
		anchors.leftMargin: -4
		radius: 15
	}

	Rectangle {
		id: contentRectangle
		anchors.fill: parent
		color: "white"
		property variant colorGradient: ["#FFFFFF", "#FFFEFC", "#FFFDF9", "#FFFCF7", "#FFFBF4", "#FFFAF2", "#FFF9EF", "#FFF8EC", "#FFF7EA", "#FFF6E7", "#FFF5E5", "#FFF5E2", "#FFF4E0", "#FFF3DD", "#FFF2DA", "#FFF1D8", "#FFF0D5", "#FFEFD3", "#FFEED0", "#FFEDCE", "#FFECCB", "#FFEBC8", "#FFEBC6", "#FFEAC3", "#FFE9C1", "#FFE8BE", "#FFE7BC", "#FFE6B9", "#FFE5B6", "#FFE4B4", "#FFE3B1", "#FFE2AF", "#FFE1AC", "#FFE1AA", "#FFE0A7", "#FFDFA4", "#FFDEA2", "#FFDD9F", "#FFDC9D", "#FFDB9A", "#FFDA97", "#FFD995", "#FFD892", "#FFD790", "#FFD78D", "#FFD68B", "#FFD588", "#FFD485", "#FFD383", "#FFD280", "#FFD17E", "#FFD07B", "#FFCF79", "#FFCE76", "#FFCD73", "#FFCD71", "#FFCC6E", "#FFCB6C", "#FFCA69", "#FFC967", "#FFC864", "#FFC761", "#FFC65F", "#FFC55C", "#FFC45A", "#FFC357", "#FFC355", "#FFC252", "#FFC14F", "#FFC04D", "#FFBF4A", "#FFBE48", "#FFBD45", "#FFBC42", "#FFBB40", "#FFBA3D", "#FFB93B", "#FFB938", "#FFB836", "#FFB733", "#FFB630", "#FFB52E", "#FFB42B", "#FFB329", "#FFB226", "#FFB124", "#FFB021", "#FFAF1E", "#FFAF1C", "#FFAE19", "#FFAD17", "#FFAC14", "#FFAB12", "#FFAA0F", "#FFA90C", "#FFA80A", "#FFA707", "#FFA605", "#FFA502", "#FFA500"]

		Timer
		{
			id: blinkTimer
			repeat: true
			interval: 40
			running: false
			property int index: 0
			property int direction: 1
			onTriggered: {
				if (!buttonActionContainer.enabled)
					return
				index = index + direction
				color = parent.colorGradient[index]
				updateColor(color)
				if (index >= parent.colorGradient.length - 1)
					direction = -1
				else if (index <= 0)
					direction = 1
			}
			onRunningChanged: {
				if (!running)
				{
					left.color = "white"
					right.color = "white"
					parent.color = "white"
					index = 0
					direction = 1
				}
			}
		}

		Image {
			id: debugImage
			anchors {
				left: parent.left
				right: parent.right
				top: parent.top
				bottom: parent.bottom
				bottomMargin: debugImg.pressed ? -2 : 0;
				topMargin: debugImg.pressed ? 2 : 0;
			}
			source: sourceImg
			fillMode: Image.PreserveAspectFit
			height: 32
			width: 32
		}

		DefaultButton {
			anchors.fill: parent
			id: debugImg
			text: sourceImg ? "" : buttonActionContainer.text
			action: buttonAction
			style: ButtonStyle {
				background: Rectangle {
					color: "transparent"
				}
			}
			tooltip: buttonActionContainer.text
		}

		Action {
			id: buttonAction
			shortcut: buttonShortcut
			onTriggered: {
				buttonActionContainer.clicked();
			}
		}
	}

	Rectangle
	{
		id: right
		width: 10
		height: parent.height
		anchors.right: contentRectangle.right
		anchors.rightMargin: -4
		radius: 15
	}

	Rectangle
	{
		id: labelCont
		anchors.top: contentRectangle.bottom
		anchors.topMargin: 10
		width: parent.width
		DefaultLabel
		{
			text: buttonActionContainer.text
			visible: false
			anchors.centerIn: parent
			id: btnLabel
		}
	}
}


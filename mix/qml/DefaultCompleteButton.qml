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
/**
 * @file DefaultCompleteButton.qml
 * 
 * 
 * Mix: Elementrem IDE client.
 */

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0

DefaultImgButton {
	id: completeButton
	property var imageSource
	property var labelText
	style: ButtonStyle {
		background: Rectangle {
			color: welcomeViewStyleId.bgColor
			implicitWidth: 48
			implicitHeight: 48
		}
		label: RowLayout {
			Image {
			   source: imageSource
			   width: 48
			   height: 48
			   id: openProjectImg
			   anchors.top: parent.top
			}
			DefaultLabel
			{
			   text: labelText
			   font.pixelSize: 22
			   font.italic: true
			   color: welcomeViewStyleId.fontColor
			}
		}
	}
}

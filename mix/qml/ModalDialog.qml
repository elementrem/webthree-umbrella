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
/** @file ModalDialog.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.2
import QtQuick.Window 2.0

Window
{
	id: modalDialog
	title: ""
	modality: Qt.WindowModal
	height: 400
	width: 700
	visible: false
	Loader
	{
		focus: true
		id: modalDialogContent
		objectName: "modalDialogContent"
		anchors.fill: parent
	}
	function open()
	{
		visible = true;
	}
	function close()
	{
		visible = false;
		modalDialogContent.source = "";
		modalDialogContent.sourceComponent = undefined;
		modalDialog.destroy();
	}
}

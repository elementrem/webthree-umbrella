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
/** @file Element.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

/*
 * Display a row containing :
 *  - The amount of Element.
 *  - The unit used.
 *  - User-friendly string representation of the amout of Element (if displayFormattedValue == true).
 * 'value' has to be a QElement obj.
*/
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1

RowLayout {
	id: elementEdition
	property bool displayFormattedValue
	property bool edit
	property bool readOnly
	property variant value;
	property bool displayUnitSelection
	onValueChanged: update()
	Component.onCompleted: update()
	signal amountChanged
	signal unitChanged
	property int inputWidth

	onReadOnlyChanged:
	{
		readonlytxt.visible = readOnly
		elementValueEdit.visible = !readOnly
		readonlytxt.text = elementValueEdit.text
		units.enabled = !readOnly
	}

	function toHexMey()
	{
		var mey = value.toMey()
		if (mey !== null)
			return "0x" + mey.hexValue()
		else
			return "0x"
	}

	function update()
	{
		if (value)
		{
			elementValueEdit.text = value.value;
			readonlytxt.text = value.value
			selectUnit(value.unit);
			units.updateCombobox()
		}
	}

	function selectUnit(unit)
	{
		units.currentIndex = unit;
		units.updateCombobox()
	}

	function formatInput()
	{
		if (value !== undefined)
		{
			var v = value.format()
			var element = v.split(" ")
			elementValueEdit.text = element[0]
			readonlytxt.text = element[0]
			for (var k = 0; k < unitsModel.count; k++)
			{
				if (unitsModel.get(k).text === element[1])
				{
					units.currentIndex = k
					break;
				}
			}
		}
	}

	Rectangle
	{
		anchors.left: parent.left
		Layout.preferredWidth:
		{
			if (inputWidth)
				return inputWidth
			else
			{
				var w = parent.width
				if (displayUnitSelection)
					w = w - units.width - 5
				if (displayFormattedValue)
					w = w - formattedValue.width - 5
				return w
			}
		}
		visible: edit
		DefaultTextField
		{
			anchors.verticalCenter: parent.verticalCenter
			width: parent.width
			anchors.left: parent.left
			onTextChanged:
			{
				if (value !== undefined)
				{
					value.setValue(text)
					formattedValue.text = value.format();
					amountChanged()
				}
			}
			readOnly: elementEdition.readOnly
			visible: edit
			id: elementValueEdit;
		}

		DefaultTextField
		{
			anchors.verticalCenter: parent.verticalCenter
			width: parent.width
			anchors.left: parent.left
			id: readonlytxt
			readOnly: true
			visible: false
			style: TextFieldStyle {
				background: Rectangle
				{
					color: "#cccccc"
					radius: 4
					width: elementValueEdit.width
				}
			}
		}
	}

	DefaultCombobox
	{
		rootItem: elementEdition
		anchors.verticalCenter: parent.verticalCenter
		Layout.minimumWidth: 100
		id: units
		visible: displayUnitSelection;
		onCurrentTextChanged:
		{
			if (value)
			{
				value.setUnit(currentText);
				formattedValue.text = value.format();
				unitChanged()
			}
			updateCombobox()
		}

		onModelChanged:
		{
			updateCombobox()
		}

		model: ListModel {
			id: unitsModel
			ListElement { text: "Uelement"; }
			ListElement { text: "Velement"; }
			ListElement { text: "Delement"; }
			ListElement { text: "Nelement"; }
			ListElement { text: "Yelement"; }
			ListElement { text: "Zelement"; }
			ListElement { text: "Eelement"; }
			ListElement { text: "Pelement"; }
			ListElement { text: "Telement"; }
			ListElement { text: "Gelement"; }
			ListElement { text: "Melement"; }
			ListElement { text: "grand"; }
			ListElement { text: "element"; }
			ListElement { text: "finney"; }
			ListElement { text: "szabo"; }
			ListElement { text: "Gmey"; }
			ListElement { text: "Mmey"; }
			ListElement { text: "Kmey"; }
			ListElement { text: "mey"; }
		}
	}

	DefaultLabel
	{
		Layout.minimumWidth: 100
		visible: displayFormattedValue
		id: formattedValue
	}
}

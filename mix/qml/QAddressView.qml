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
/** @file QAddressView.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import "js/InputValidator.js" as InputValidator

ColumnLayout
{
	property alias value: textinput.text
	property alias accountRef: ctrModel
	property string subType
	property bool readOnly
	property alias currentIndex: trCombobox.currentIndex
	property alias displayInput: textInputRect.visible
	property variant accounts
	signal indexChanged()
	spacing: 0
	id: editRoot
	height: getHeight()
	width: 350

	SourceSansProBold
	{
		id: boldFont
	}

	function isArray()
	{
		InputValidator.init()
		return InputValidator.isArray(subType)
	}

	function currentValue() {
		return value;
	}

	function currentType()
	{
		return accountRef.get(trCombobox.currentIndex).type;
	}

	function current()
	{
		return accountRef.get(trCombobox.currentIndex);
	}

	function getHeight()
	{
		if (isArray() && !readOnly)
			return 60
		else
			return 30
	}

	function getAddress()
	{
		var value = ctrModel.get(trCombobox.currentIndex).value
		if (value.indexOf("0x") === 0)
			return value
		else if (value.indexOf("<") === 0)
			return clientModel.contractAddresses[value]
		else
			return value
	}

	function load()
	{
		accountRef.clear();
		if (subType.indexOf("contract") !== -1 || subType.indexOf("address") !== -1)
		{
			var trCr = 0;
			if (blockChainPanel)
				for (var k = 0; k < blockChainPanel.model.blocks.length; k++)
				{
					if (k > blockIndex)
						break;
					for (var i = 0; i < blockChainPanel.model.blocks[k].transactions.length; i++)
					{
						if (i > transactionIndex)
							break;
						var tr = blockChainPanel.model.blocks[k].transactions[i]
						if (tr.functionId === tr.contractId && !codeModel.contracts[tr.contractId].contract.isLibrary())
						{
							accountRef.append({ "itemid": tr.contractId + " - " + trCr, "value": "<" + tr.contractId + " - " + trCr + ">", "type": "contract" });
							trCr++;
						}
					}
				}
		}
		if (subType.indexOf("address") !== -1)
		{
			for (k = 0; k < accounts.length; k++)
			{
				if (accounts[k].address === undefined)
					accounts[k].address = clientModel.address(accounts[k].secret);
				accountRef.append({ "itemid": accounts[k].name, "value": "0x" + accounts[k].address, "type": "address" });
			}
		}
		editRoot.height = editRoot.getHeight()
	}

	function init()
	{
		btnAdd.visible = isArray()
		textinput.readOnly = readOnly
		if (isArray() || readOnly)
			displayInput = true
		else
			displayInput = false

		var ctrVisible = isArray() || !readOnly

		rowCombobox.visible = ctrVisible
		trCombobox.visible = ctrVisible
		if (!ctrVisible)
		{
			rowCombobox.height = 0
			trCombobox.height = 0
			textinput.anchors.top = textinput.parent.top
		}

		if (!readOnly)
		{
			trCombobox.currentIndex = 0
			for (var k = 0; k < ctrModel.count; k++)
			{
				if (ctrModel.get(k).value === value)
				{
					trCombobox.currentIndex = k
					break
				}
			}
		}
		if (!isArray())
			trCombobox.update()
		else if (value === "")
			textinput.text = "[]"
	}

	function select(address)
	{
		for (var k = 0; k < accountRef.count; k++)
		{
			if (accountRef.get(k).value === address)
			{
				trCombobox.currentIndex = k;
				break;
			}
		}
	}

	function selectFirst()
	{
		trCombobox.currentIndex = 0
	}

	ListModel
	{
		id: ctrModel
	}

	Row
	{
		anchors.top: parent.top
		height: 30
		id: rowCombobox
		DefaultCombobox
		{
			property bool selected: false
			id: trCombobox
			model: ctrModel
			rootItem: editRoot
			width: editRoot.width
			textRole: "itemid"
			function update()
			{
				trCombobox.selected = false;
				if (currentText === "")
					return;
				else if (currentText !== " - ")
				{
					if (model.get(currentIndex))
					{
						if (model.get(currentIndex).type === "contract")
							textinput.text = "<" + currentText + ">";
						else
							textinput.text = model.get(currentIndex).value; //address
						trCombobox.selected = true;
					}
				}
				else if (textinput.text.indexOf("<") === 0)
					textinput.text = ""
				indexChanged()
				updateCombobox()
			}

			onCurrentIndexChanged: {
				if (!isArray())
					update()
			}
		}

		DefaultButton
		{
			id: btnAdd
			text: qsTr("Add")
			visible: false
			height: trCombobox.height
			onClicked:
			{
				var ar = JSON.parse(textinput.text)
				ar.push(trCombobox.model.get(currentIndex).value)
				textinput.text = JSON.stringify(ar)
			}
		}
	}

	Rectangle {
		radius: 4
		width: editRoot.width
		height: trCombobox.height
		id: textInputRect
		DefaultTextField {
			id: textinput
			text: value
			anchors.fill: parent
			clip: true
			font.family: boldFont.name
			MouseArea {
				id: mouseArea
				anchors.fill: parent
				hoverEnabled: true
				onClicked: textinput.forceActiveFocus()
			}
			onTextChanged:
			{
				if (trCombobox.selected)
				{
					trCombobox.currentIndex = 0;
					trCombobox.selected = false;
				}
			}
		}
		CopyButton
		{
			visible: textinput.visible && textinput.readOnly
			anchors.left: textinput.right
			anchors.verticalCenter: parent.verticalCenter
			getContent: function()
			{
				return textinput.text
			}
		}
	}
}

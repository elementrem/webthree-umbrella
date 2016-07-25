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
/** @file StateDialog.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Controls.Styles 1.3
import org.elementrem.qml.QElement 1.0
import org.elementrem.qml.InverseMouseArea 1.0
import "js/QElementHelper.js" as QElementHelper
import "js/TransactionHelper.js" as TransactionHelper
import "."

Dialog {
	id: modalStateDialog
	modality: Qt.ApplicationModal

	width: 630
	height: 480
	title: qsTr("Edit Starting Parameters")
	visible: false

	property alias minerComboBox: comboMiner
	property int minerIndex
	property int stateIndex
	property var stateTransactions: []
	property var stateAccounts: []
	property var stateContracts: []
	signal accepted
	signal closed

	StateDialogStyle {
		id: stateDialogStyle
	}

	function open(index, item) {
		if (mainApplication.systemPointSize >= appSettings.systemPointSize)
		{
			width = 630
			height = 480
		}
		else
		{
			width = 630 + (11 * appSettings.systemPointSize)
			height = 480 + (7 * appSettings.systemPointSize)
		}
		stateIndex = index
		accountsModel.clear()
		stateAccounts = []
		minerIndex = 0
		for (var k = 0; k < item.accounts.length; k++) {
			accountsModel.append(item.accounts[k])
			stateAccounts.push(item.accounts[k])
			if (item.miner && item.accounts[k].name === item.miner.name)
				minerIndex = k
		}
		contractsModel.clear()
		stateContracts = []
		if (item.contracts) {
			for (k = 0; k < item.contracts.length; k++) {
				contractsModel.append(item.contracts[k])
				stateContracts.push(item.contracts[k])
			}
		}

		visible = true
		updateMinerList()
		forceActiveFocus()
	}

	function updateMinerList()
	{
		comboMiner.model = stateAccounts
		comboMiner.currentIndex = minerIndex
	}

	function acceptAndClose() {
		close()
		accepted()
	}

	function close() {
		visible = false
		closed()
	}

	function getItem() {
		var item = {
			accounts: stateAccounts,
			contracts: stateContracts
		}
		for (var k = 0; k < stateAccounts.length; k++) {
			if (stateAccounts[k].name === comboMiner.currentText) {
				item.miner = stateAccounts[k]
				break
			}
		}
		return item
	}

	contentItem: ColumnLayout {
		implicitHeight: modalStateDialog.height
		implicitWidth: modalStateDialog.width
		anchors.fill: parent
		anchors.margins: 10
		ListModel {
			id: contractsModel

			function removeContract(_i) {
				contractsModel.remove(_i)
				stateContracts.splice(_i, 1)
			}
		}

		ListModel {
			id: accountsModel

			function removeAccount(_i) {
				accountsModel.remove(_i)
				stateAccounts.splice(_i, 1)
			}
		}

		ColumnLayout {
			id: dialogContent
			anchors.top: parent.top
			width: parent.width

			ColumnLayout {
				Layout.fillWidth: true

				RowLayout {
					Layout.preferredWidth: parent.width
					DefaultLabel {
						id: contractsLabel
						text: qsTr("Genesis Contracts")
					}

					DefaultButton {
						Layout.alignment: Qt.AlignRight
						id: importStateButton
						tooltip: qsTr("Import genesis state from JSON file")
						text: qsTr("Import...")
						onClicked: {
							importJsonFileDialog.open()
						}
					}

					QFileDialog {
						id: importJsonFileDialog
						visible: false
						title: qsTr("Select State File")
						nameFilters: Qt.platform.os === "osx" ? [] : [qsTr("JSON files (*.json)", "All files (*)")] //qt 5.4 segfaults with filter string on OSX
						onAccepted: {
							var path = importJsonFileDialog.fileUrl.toString()
							var jsonData = fileIo.readFile(path)
							if (jsonData) {
								var json = JSON.parse(jsonData)
								for (var address in json) {
									var account = {
										address: address,
										name: (json[address].name ? json[address].name : address),
										balance: QElementHelper.createElement(json[address].mey, QElement.Mey),
										code: json[address].code,
										storage: json[address].storage
									}
									if (account.code) {
										contractsModel.append(account)
										stateContracts.push(account)
									} else {
										accountsModel.append(account)
										stateAccounts.push(account)
									}
								}
							}
						}
					}
				}

				TableView {

					id: genesisContractsView
					Layout.fillWidth: true
					Layout.fillHeight: true
					model: contractsModel
					headerVisible: false
					TableViewColumn {
						role: "name"
						title: qsTr("Name")
						width: 260
						delegate: Item {
							RowLayout {
								height: 25
								anchors.verticalCenter: parent.verticalCenter
								anchors.horizontalCenter: parent.horizontalCenter
								width: parent.width - 20
								Button {
									height: 25
									width: 25
									anchors.verticalCenter: parent.verticalCenter
									tooltip: qsTr("Delete Contract")
									onClicked: {
										stateContracts.splice(styleData.row, 1)
										contractsModel.remove(styleData.row)
									}
									Image {
										height: 25
										width: 25
										anchors.centerIn: parent
										source: "qrc:/qml/img/delete-block-icon@2x.png"
									}
								}

								DefaultTextField {
									Layout.fillWidth: true
									anchors.verticalCenter: parent.verticalCenter
									onTextChanged: {
										if (styleData.row > -1)
											stateContracts[styleData.row].name = text
									}
									text: styleData.value
								}
							}
						}
					}

					TableViewColumn {
						role: "balance"
						title: qsTr("Balance")
						width: 230
						delegate: Item {
							Element {
								width: parent.width
								anchors.verticalCenter: parent.verticalCenter
								edit: true
								displayFormattedValue: false
								value: styleData.value
							}
						}
					}
					rowDelegate: Rectangle {
						color: styleData.alternate ? "transparent" : "#f0f0f0"
						height: 60
					}
				}
			}

			ColumnLayout {
				Layout.fillWidth: true
				RowLayout {
					Layout.preferredWidth: parent.width
					DefaultLabel {
						id: accountsLabel
						Layout.preferredWidth: 85
						text: qsTr("Accounts")
					}

					Row
					{
						Layout.alignment: Qt.AlignRight
						spacing: 5
						DefaultButton
						{
							id: addAccount
							Image
							{
								anchors.centerIn: parent
								source: "qrc:/qml/img/newaccounticon@2x.png"
							}
							width: 35
							height: 28
							anchors.verticalCenter: parent.verticalCenter
							tooltip: qsTr("Add new account")
							onClicked:
							{
								newAddressWin.accounts = stateAccounts
								newAddressWin.open()
							}
						}

						CopyButton
						{
							id: cpBtn
							tooltip: qsTr("Copy All Users Details to Clipboard")
							getContent: function() {
								return JSON.stringify(stateAccounts, null, "\t");
							}
						}
					}
				}

				MessageDialog {
					id: alertAlreadyUsed
					text: qsTr("This account is in use. You cannot remove it. The first account is used to deploy config contract and cannot be removed.")
					icon: StandardIcon.Warning
					standardButtons: StandardButton.Ok
				}

				TableView {
					Layout.fillWidth: true
					Layout.fillHeight: true

					id: accountsView
					model: accountsModel
					headerVisible: false
					TableViewColumn {
						role: "name"
						title: qsTr("Name")
						delegate: Item {
							Rectangle
							{
								anchors.fill: parent
								color: "transparent"
								RowLayout {
									anchors.top: parent.top
									anchors.topMargin: 10
									anchors.horizontalCenter: parent.horizontalCenter
									anchors.verticalCenter: parent.verticalCenter
									width: parent.width - 20
									MessageDialog
									{
										id: deleteAccountMsg
										text: qsTr("Are you sure you want to delete this account?")
										onYes:
										{
											stateAccounts.splice(styleData.row, 1)
											accountsView.model.remove(styleData.row)
											updateMinerList()
										}
										standardButtons: StandardButton.Yes | StandardButton.No
									}

									Component.onCompleted:
									{
										addressCopy.originalText = stateAccounts[styleData.row].address
									}

									Layout.preferredWidth: parent.width
									Layout.minimumHeight: 20
									spacing: 5

									Rectangle
									{
										Layout.fillWidth: true
										Rectangle
										{
											border.color: "#cccccc"
											border.width: 1
											width: parent.width
											color: "transparent"
											radius: 4
											height: labelUser.height + 5
											anchors.verticalCenter: parent.verticalCenter
											DefaultText
											{
												anchors.verticalCenter: parent.verticalCenter
												anchors.left: parent.left
												anchors.leftMargin: 2
												id: labelUser
												text: styleData.value
												width: parent.width
												elide: Text.ElideRight
												MouseArea
												{
													anchors.fill: parent
													onDoubleClicked:
													{
														parent.visible = false
														parent.parent.visible = false
														addressField.visible = true
														inverseMouseArea.active = true
														addressField.forceActiveFocus()
													}
												}
											}
										}

										DefaultTextField
										{
											id: addressField
											property bool first: true
											visible: false
											anchors.verticalCenter: parent.verticalCenter
											width: parent.width
											onTextChanged: {
												if (styleData.row > -1 && stateAccounts[styleData.row])
												{
													stateAccounts[styleData.row].name = text
													var index = comboMiner.currentIndex
													comboMiner.model = stateAccounts
													comboMiner.currentIndex = index
												}
												if (first)
												{
													addressField.cursorPosition = 0
													first = false
												}
											}
											text: {
												return styleData.value
											}
											Keys.onEnterPressed: {
												hide()
											}

											Keys.onReturnPressed: {
												hide()
											}

											function hide()
											{
												labelUser.visible = true
												labelUser.parent.visible = true
												addressField.visible = false
												labelUser.text = addressField.text
												inverseMouseArea.active = false
											}

											InverseMouseArea
											{
												id: inverseMouseArea
												anchors.fill: parent
												active: false
												onClickedOutside: {
													parent.hide()
												}
											}
										}
									}

									Element {
										anchors.verticalCenter: parent.verticalCenter
										inputWidth: 100
										Layout.minimumWidth: 210
										edit: true
										displayFormattedValue: false
										value: {
											if (stateAccounts[styleData.row])
												return stateAccounts[styleData.row].balance
										}
										displayUnitSelection: true
										Component.onCompleted: {
											formatInput()
										}
									}


									DisableInput
									{
										anchors.verticalCenter: parent.verticalCenter
										id: addressCopy
										Layout.preferredWidth: 150
										tooltip: qsTr("Copy User Address to Clipboard")

										DefaultButton {
											width: 35
											height: parent.height
											anchors.verticalCenter: parent.verticalCenter
											anchors.left: parent.right
											anchors.leftMargin: 5
											tooltip: qsTr("Delete Account")
											onClicked: if (stateAccounts.length > 1) { deleteAccountMsg.open() }
											enabled: accountsView.model.count > 1
											Image {
												width: 25
												height: 25
												anchors.centerIn: parent
												source: "qrc:/qml/img/delete-block-icon@2x.png"
											}
										}
									}

									Rectangle
									{
										Layout.preferredWidth: 85
									}
								}
							}
						}
					}

					rowDelegate: Rectangle {
						color: styleData.alternate ? "transparent" : "#f0f0f0"
						height: 60
					}
				}
			}

			NewAccount
			{
				id: newAddressWin
				onAccepted:
				{
					accountsModel.append(ac)
					stateAccounts.push(ac)
					clientModel.addAccount(ac.secret);
					projectModel.saveProject()
				}
			}

			ColumnLayout {
				DefaultLabel {
					Layout.preferredWidth: 50
					text: qsTr("Miner")
				}

				DefaultCombobox {
					id: comboMiner
					textRole: "name"
					Layout.fillWidth: true
					rootItem: modalStateDialog
				}
			}
		}

		Row {
			id: validationRow
			anchors.bottom: parent.bottom
			layoutDirection: Qt.RightToLeft
			width: parent.width
			anchors.right: parent.right
			spacing: 10
			height: 40

			DefaultButton {
				anchors.top: parent.top
				text: qsTr("Cancel")
				onClicked: close()
			}

			DefaultButton {
				anchors.top: parent.top
				text: qsTr("OK")
				onClicked: {
					close()
					accepted()
				}
			}
		}
	}
}

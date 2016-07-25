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
/** @file ScenarioLoader.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.1
import Qt.labs.settings 1.0
import org.elementrem.qml.InverseMouseArea 1.0
import "js/Debugger.js" as Debugger
import "js/ErrorLocationFormater.js" as ErrorLocationFormater
import "."

ColumnLayout
{
	id: blockChainSelector
	signal restored(variant scenario)
	signal saved(variant scenario)
	signal duplicated(variant scenario)
	signal loaded(variant scenario)
	signal renamed(variant scenario)
	signal deleted()
	signal closed()
	property alias selectedScenarioIndex: scenarioList.currentIndex
	property bool panelLoaded: false
	property int btnWidth: 32
	property int btnHeight: 32

	function init()
	{
		scenarioList.model = projectModel.stateListModel
		scenarioList.load()
		panelLoaded = true
	}

	function clear()
	{
		scenarioList.model = []
		closed()
		panelLoaded = false
	}

	function needSaveOrReload()
	{
	}

	function updateWidth(_width)
	{
		var w;
		if (_width < btnRowContainer.minimalWidth)
			w = (_width - btnRowContainer.comboboxWidth) / 6
		else
			w = btnWidth

		updatebtnWidth((rowBtn.width - 20) / 6 < btnWidth ? (rowBtn.width - 20) / 6 : btnWidth)
	}

	function updatebtnWidth(w)
	{
		rowBtn.width = 6 * w
	}

	function createScenario(defaultScenario)
	{
		dropBtns.createScenario(defaultScenario)
	}

	RowLayout
	{
		spacing: 15
		anchors.top: parent.top
		anchors.topMargin: 7
		id: btnRowContainer
		property int comboboxWidth: 160
		property int minimalWidth: 100 + 6 * btnRowContainer.comboboxWidth
		Layout.minimumHeight: btnHeight

		Item
		{
			Layout.preferredWidth: parent.comboboxWidth
			Layout.minimumHeight: 30
			id: scenarioCont
			anchors.top: parent.top

			Connections
			{
				target: projectModel.stateListModel
				onStateDeleted: {
					scenarioList.init()
				}
			}

			Rectangle
			{
				id: left
				width: 10
				height: parent.height
				anchors.left: scenarioList.left
				anchors.leftMargin: -4
				radius: 15
			}

			ComboBox
			{
				id: scenarioList
				anchors.left: parent.left
				model: projectModel.stateListModel
				anchors.top: parent.top
				textRole: "title"
				height: scenarioCont.height
				width: btnRowContainer.comboboxWidth
				signal updateView()

				onCurrentIndexChanged:
				{
					dropBtns.restore()
				}

				function init()
				{
					scenarioList.currentIndex = 0
					deleted()
				}

				function load()
				{
					var state = projectModel.stateListModel.getState(currentIndex)
					if (state)
						loaded(state)
				}

				style: ComboBoxStyle {
					id: style
					background: Rectangle {
						color: "white"
						anchors.fill: parent
					}
					label: Rectangle {
						property alias label: comboLabel
						anchors.fill: parent
						color: "white"
						DefaultLabel {
							Image {
								id: up
								anchors.top: parent.top
								anchors.right: parent.right
								anchors.topMargin: -2
								source: "qrc:/qml/img/up.png"
								width: 10
								height: 10
							}

							Image {
								id: down
								anchors.bottom: parent.bottom
								anchors.bottomMargin: -3
								anchors.right: parent.right
								source: "qrc:/qml/img/down.png"
								width: 10
								height: 10
							}
							id: comboLabel
							maximumLineCount: 1
							elide: Text.ElideRight
							width: parent.width
							anchors.verticalCenter: parent.verticalCenter
							anchors.left: parent.left
							anchors.leftMargin: -4
							Component.onCompleted:
							{
								comboLabel.updateLabel()
							}

							function updateLabel()
							{
								comboLabel.text = ""
								if (scenarioList.currentIndex > - 1 && scenarioList.currentIndex < projectModel.stateListModel.count)
									comboLabel.text = projectModel.stateListModel.getState(scenarioList.currentIndex).title
							}

							Connections {
								target: blockChainSelector
								onLoaded: {
									if (projectModel.stateListModel.count > 0)
										comboLabel.text = projectModel.stateListModel.getState(scenarioList.currentIndex).title
									else
										return ""
								}
								onRenamed: {
									comboLabel.text = scenario.title
									scenarioNameEdit.text = scenario.title
								}
								onDeleted: {
									comboLabel.updateLabel()
								}
								onClosed: {
									comboLabel.text = ""
								}
							}
						}
					}
				}
			}

			DefaultTextField
			{
				id: scenarioNameEdit
				anchors.left: scenarioCont.left
				anchors.top: parent.top
				anchors.leftMargin: -4
				height: parent.height
				z: 5
				visible: false
				width: btnRowContainer.comboboxWidth
				Keys.onEnterPressed:
				{
					toggleEdit()
				}

				Keys.onReturnPressed:
				{
					toggleEdit()
				}

				function toggleEdit()
				{
					scenarioList.visible = !scenarioList.visible
					scenarioNameEdit.visible = !scenarioNameEdit.visible
					if (!scenarioNameEdit.visible)
						scenarioNameEdit.save()
					else
					{
						scenarioNameEdit.text = projectModel.stateListModel.getState(scenarioList.currentIndex).title
						scenarioNameEdit.forceActiveFocus()
						outsideClick.active = true
					}
				}

				function save()
				{
					outsideClick.active = false

					for (var k = 0; k < projectModel.stateListModel.count; ++k)
					{
						if (projectModel.stateListModel.get(k).title === scenarioNameEdit.text)
							return //title already exists
					}

					projectModel.stateListModel.getState(scenarioList.currentIndex).title = scenarioNameEdit.text
					projectModel.saveProjectFile()
					saved(state)
					scenarioList.model.get(scenarioList.currentIndex).title = scenarioNameEdit.text
					scenarioList.currentIndex = scenarioList.currentIndex
					renamed(projectModel.stateListModel.getState(scenarioList.currentIndex))
				}

				InverseMouseArea {
					id: outsideClick
					anchors.fill: parent
					active: false
					onClickedOutside: {
						scenarioNameEdit.toggleEdit()
					}
				}
			}

			Rectangle
			{
				width: 10
				height: parent.height
				anchors.right: parent.right
				anchors.rightMargin: -6
				color: "white"
				radius: 15
			}
		}

		Row
		{
			anchors.top: parent.top
			width: btnWidth * 6
			Layout.preferredWidth: btnWidth * 6
			Layout.minimumHeight: 30
			id: rowBtn

			DropdownButton
			{
				width: btnWidth
				height: btnHeight
				id: dropBtns
				tooltip: qsTr("Manage Scenario")
				Component.onCompleted:
				{
					actions.push({ label: qsTr("Edit Title") , action: editTitle })
					actions.push({ label: qsTr("Delete Scenario") , action: deleteSce })
					actions.push({ label: qsTr("New Scenario") , action: newSce })
					actions.push({ label: qsTr("Duplicate Scenario") , action: duplicate })
					actions.push({ label: qsTr("Create Scenario") , action: addSce })
					init()
				}

				function editTitle()
				{
					scenarioNameEdit.toggleEdit()
				}

				function deleteSce()
				{
					if (projectModel.stateListModel.count > 1)
						deleteWarning.open()
				}

				function newSce()
				{
					createScenario(false)
				}

				function duplicate()
				{
					projectModel.stateListModel.duplicateState(scenarioList.currentIndex)
					duplicated(state)
					scenarioList.currentIndex = projectModel.stateListModel.count - 1
					scenarioNameEdit.toggleEdit()
				}

				function addSce()
				{
					createScenario(false)
				}

				function createScenario(defaultScenario)
				{
					var item = projectModel.stateListModel.createDefaultState();
					if (defaultScenario)
					{
						//initiate input parameters
						item.blocks[0].transactions[0].parameters = { "v": 12 }
					}

					item.title = defaultScenario ? qsTr("Default") : qsTr("New Scenario")
					item.title = projectModel.stateListModel.correctTitle(item.title)
					projectModel.stateListModel.appendState(item)
					projectModel.stateListModel.save()
					scenarioList.currentIndex = defaultScenario ? 0 : projectModel.stateListModel.count - 1
					if (defaultScenario)
						scenarioList.load()
					else
						scenarioNameEdit.toggleEdit()
				}

				function restore()
				{
					var state = projectModel.stateListModel.reloadStateFromProject(scenarioList.currentIndex)
					if (state)
					{
						restored(state)
						loaded(state)
					}
				}
			}

			MessageDialog
			{
				id: deleteWarning
				text: qsTr("Are you sure you want to delete this scenario?")
				onYes:
				{
					projectModel.stateListModel.deleteState(scenarioList.currentIndex)
					scenarioList.init()
				}
				standardButtons: StandardButton.Yes | StandardButton.No
			}
		}
	}
}

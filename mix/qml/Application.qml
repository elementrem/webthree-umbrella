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
/** @file Application.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import QtQuick.PrivateWidgets 1.1
import Qt.labs.settings 1.0
import org.elementrem.qml.QElement 1.0
import org.elementrem.qml.CodeModel 1.0
import org.elementrem.qml.ClientModel 1.0
import org.elementrem.qml.FileIo 1.0
import org.elementrem.qml.Clipboard 1.0
import org.elementrem.qml.ApplicationService 1.0

ApplicationWindow {

	id: mainApplication
	signal loaded;
	visible: true
	width: 1200
	height: 800
	minimumWidth: 400
	minimumHeight: 300
	title: qsTr("Mix")
	property alias systemPointSize: appService.systemPointSize;
	property alias mainContent: mainContent;
	property alias codeModel: codeModel;
	property alias clientModel: clientModel;
	property alias projectModel: projectModel;
	property alias appService: appService;
	property alias mainSettings: appSettings
	property bool trackLastProject: true;
	WelcomeViewStyle { id: welcomeViewStyleId }

	ApplicationService {
		id: appService
	}

	CodeModel {
		id: codeModel
	}

	ClientModel {
		id: clientModel
		codeModel: codeModel
		Component.onCompleted:
		{
			init("/mixdebug")
		}
	}

	ProjectModel {
		id: projectModel
	}

	FileIo {
		id: fileIo
	}

	Clipboard {
		id: clipboard
	}

	Style {
		id: appStyle
	}

	Connections {
		target: mainApplication
		onClosing:
		{
			mainApplication.close();
			close.accepted = false;
		}
	}

	Component.onCompleted:  {
		loaded();
	}

	function close() {
		projectModel.appIsClosing = true;
		if (projectModel.projectPath !== "")
			projectModel.closeProject(function() { Qt.quit(); })
		else
			Qt.quit();
	}

	menuBar: MenuBar {
		Menu {
			title: qsTr("File")
			MenuItem { action: createProjectAction }
			MenuItem { action: openProjectAction }
			MenuSeparator {}
			MenuItem { action: saveAllFilesAction }
			MenuItem { action: saveCurrentDocument }
			MenuSeparator {}
			MenuItem { action: newFolderAction }
			MenuItem { action: addNewJsFileAction }
			MenuItem { action: addNewHtmlFileAction }
			MenuItem { action: addNewCssFileAction }
			MenuSeparator {}
			MenuItem { action: addNewContractAction }
			MenuItem { action: closeProjectAction }
			MenuSeparator {}
			MenuItem { action: exitAppAction }
		}
		Menu {
			title: qsTr("Deploy")
			MenuItem { action: mineAction }
			MenuSeparator {}
			MenuItem { action: getCompilatonResultAction }
			MenuItem { action: deployViaRpcAction }
			MenuSeparator {}
			MenuItem { action: toggleRunOnLoadAction }
		}
		Menu {
			title: qsTr("Scenario")
			MenuItem { action: editStatesAction }
			MenuItem { action: setAsDefaultAction }
			MenuItem { action: displayCallsAction }
		}
		Menu {
			title: qsTr("Debug")
			MenuItem { action: debugRunAction }
			MenuSeparator {}
			MenuItem { action: toggleAssemblyDebuggingAction }
		}
		Menu {
			title: qsTr("Tools")
			MenuItem { action: gasEstimationAction }
			MenuItem { action: optimizeCodeAction }
		}
		Menu {
			title: qsTr("Windows")
			MenuItem { action: openNextDocumentAction }
			MenuItem { action: openPrevDocumentAction }
			MenuSeparator {}
			MenuItem { action: toggleProjectNavigatorAction }
			MenuItem { action: showHideRightPanelAction }
			MenuItem { action: toggleWebPreviewAction }
			MenuItem { action: toggleWebPreviewOrientationAction }
			MenuSeparator {}
			MenuItem { action: increaseAppFontSize }
			MenuItem { action: decreaseAppFontSize }
			MenuItem { action: setEnvFontSize }
		}
		Menu
		{
			title: qsTr("Help")
			MenuItem { action: aboutMixAction }
		}
	}

	MainContent {
		id: mainContent;
		anchors.fill: parent
	}

	ModalDialog {
		objectName: "dialog"
		id: dialog
	}

	AlertMessageDialog {
		objectName: "alertMessageDialog"
		id: messageDialog
	}

	Settings {
		id: mainWindowSettings
		property alias mainWidth: mainApplication.width
		property alias mainHeight: mainApplication.height
		property alias mainX: mainApplication.x
		property alias mainY: mainApplication.y
	}

	Action {
		id: increaseAppFontSize
		text: qsTr("Increase Application Fontsize")
		shortcut: "Ctrl+Shift+I"
		onTriggered:
		{
			if (appSettings.systemPointSize < 23)
				appSettings.systemPointSize = appSettings.systemPointSize + 1
		}
	}

	Action {
		id: decreaseAppFontSize
		text: qsTr("Decrease Application Fontsize")
		shortcut: "Ctrl+Shift+L"
		onTriggered:
		{
			if (appSettings.systemPointSize > 9)
				appSettings.systemPointSize = appSettings.systemPointSize - 1
		}
	}

	Action {
		id: setEnvFontSize
		text: qsTr("Set to OS Font Size")
		shortcut: ""
		onTriggered:
		{
			appSettings.systemPointSize = appService.systemPointSize
		}
	}

	Action {
		id: exitAppAction
		text: qsTr("Exit")
		shortcut: "Ctrl+Q"
		onTriggered:
		{
			mainApplication.close();
		}
	}

	Action {
		id: mineAction
		text: qsTr("New Block")
		shortcut: "Ctrl+M"
		onTriggered: clientModel.mine();
		enabled: codeModel.hasContract && !clientModel.running && !clientModel.mining
	}

	Action {
		id: displayCallsAction
		text: qsTr("Display Calls")
		checkable: true
		checked: mainContent.displayCalls
		onTriggered: mainContent.displayCalls = !mainContent.displayCalls
		Component.onCompleted:
		{
			mainContent.displayCalls = checked
		}
	}

	StateList {
		id: stateList
	}

	Action {
		id: editStatesAction
		text: qsTr("Edit Scenarios")
		shortcut: "Ctrl+Alt+E"
		onTriggered: if (!projectModel.isEmpty) { stateList.open() }
	}

	Action {
		id: setAsDefaultAction
		text: qsTr("Set Current Scenario as Default")
		shortcut: "Ctrl+Alt+D"
		onTriggered:
		{
			projectModel.stateListModel.setDefaultState(mainContent.rightPane.bc.scenarioIndex)
			projectModel.saveProjectFile()
		}
	}

	Connections {
		target: projectModel.stateListModel

		function updateRunLabel()
		{
			debugRunAction.text = qsTr("Deploy") + " \"" + projectModel.stateListModel.defaultStateName() + "\"";
		}

		onDefaultStateChanged: updateRunLabel()
		onStateListModelReady: updateRunLabel()
	}

	Action {
		id: debugRunAction
		text: qsTr("Deploy")
		shortcut: "F5"
		onTriggered: mainContent.startQuickDebugging()
		enabled: codeModel.hasContract && !clientModel.running && projectModel.stateListModel.defaultStateName() !== ""
	}

	Action {
		id: toggleAssemblyDebuggingAction
		text: qsTr("Show VM Code")
		shortcut: "Ctrl+Alt+V"
		onTriggered: mainContent.debuggerPanel.assemblyMode = !mainContent.debuggerPanel.assemblyMode;
		checked:  mainContent.debuggerPanel.assemblyMode;
		checkable: true
		enabled: true
	}

	Action {
		id: toggleWebPreviewAction
		text: qsTr("Show Web View")
		shortcut: "F2"
		checkable: true
		enabled: !projectModel.isEmpty
		checked: mainContent.webViewVisible
		onTriggered: mainContent.toggleWebPreview();
	}

	Action {
		id: toggleProjectNavigatorAction
		text: qsTr("Show Project Navigator")
		shortcut: "Alt+0"
		checkable: true
		enabled: !projectModel.isEmpty
		checked: mainContent.projectViewVisible
		onTriggered: mainContent.toggleProjectView();
	}

	Action {
		id: toggleWebPreviewOrientationAction
		text: qsTr("Horizontal Web View")
		shortcut: "Alt+W"
		checkable: true
		enabled: !projectModel.isEmpty
		checked: mainContent.webViewHorizontal
		onTriggered: mainContent.toggleWebPreviewOrientation();
	}

	Action {
		id: toggleRunOnLoadAction
		text: qsTr("Load Scenario on Startup")
		shortcut: ""
		checkable: true
		checked: mainContent.runOnProjectLoad
		onTriggered: mainContent.runOnProjectLoad = !mainContent.runOnProjectLoad
	}

	Action {
		id: showHideRightPanelAction
		text: qsTr("Show Right View")
		shortcut: "F7"
		checkable: true
		enabled: !projectModel.isEmpty
		checked: mainContent.rightViewVisible
		onTriggered: mainContent.toggleRightView();
	}

	Action {
		id: createProjectAction
		text: qsTr("&New Project")
		shortcut: "Ctrl+N"
		enabled: true
		onTriggered: projectModel.createProject();
	}

	Action {
		id: openProjectAction
		text: qsTr("&Open Project")
		shortcut: "Ctrl+O"
		enabled: true;
		onTriggered: openProjectFileDialog.open()
	}

	QFileDialog {
		id: openProjectFileDialog
		visible: false
		title: qsTr("Open a Project")
		selectFolder: true
		selectExisting: true
		onAccepted: {
			var path = openProjectFileDialog.fileUrl.toString();
			projectModel.loadProject(path);
		}
	}

	Action {
		id: addNewJsFileAction
		text: qsTr("New JavaScript File")
		shortcut: "Ctrl+Alt+J"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.newJsFile();
	}

	Action {
		id: addNewHtmlFileAction
		text: qsTr("New HTML File")
		shortcut: "Ctrl+Alt+H"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.newHtmlFile();
	}

	Action {
		id: addNewCssFileAction
		text: qsTr("New CSS File")
		shortcut: "Ctrl+Alt+S"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.newCssFile();
	}

	Action {
		id: addNewContractAction
		text: qsTr("New Contract")
		shortcut: "Ctrl+Alt+C"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.newContract();
	}

	Action {
		id: newFolderAction
		text: qsTr("New Folder")
		shortcut: "Ctrl+Alt+F"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.newFolder();
	}

	QFileDialog {
		id: addExistingFileDialog
		visible: false
		title: qsTr("Add a File")
		selectFolder: false
		selectExisting: true
		onAccepted: {
			var paths = addExistingFileDialog.fileUrls;
			projectModel.addExistingFiles(paths);
		}
	}

	Action {
		id: saveAllFilesAction
		text: qsTr("Save All")
		shortcut: "Ctrl+Shift+A"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.saveAll();
	}

	Action {
		id: saveCurrentDocument
		text: qsTr("Save Current Document")
		shortcut: "Ctrl+S"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.saveCurrentDocument();
	}

	Action {
		id: closeProjectAction
		text: qsTr("Close Project")
		shortcut: "Ctrl+W"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.closeProject();
	}

	Action {
		id: openNextDocumentAction
		text: qsTr("Next Document")
		shortcut: "Ctrl+Tab"
		enabled: !projectModel.isEmpty
		onTriggered: mainContent.projectNavigator.openNextOpenedDocument();
	}

	Action {
		id: aboutMixAction
		text: qsTr("About Mix")
		onTriggered: aboutMix.open()
	}

	AboutMix
	{
		id: aboutMix
	}

	Action {
		id: openPrevDocumentAction
		text: qsTr("Previous Document")
		shortcut: "Ctrl+Shift+Tab"
		enabled: !projectModel.isEmpty
		onTriggered: mainContent.projectNavigator.openPrevOpenedDocument();
	}

	Action {
		id: toggleBreakpointAction
		text: qsTr("Toggle Breakpoint")
		shortcut: "F9"
		enabled: mainContent.codeEditor.editingContract();
		onTriggered: mainContent.toggleBreakpoint();
	}

	Action {
		id: getCompilatonResultAction
		text: qsTr("Open Compilation Result")
		shortcut: "Ctrl+Shift+C"
		enabled: !projectModel.isEmpty
		onTriggered: Qt.openUrlExternally(projectModel.projectPath + "/package/compilation")
	}

	Action {
		id: deployViaRpcAction
		text: qsTr("Deploy to Network")
		shortcut: "Ctrl+Shift+D"
		enabled: !projectModel.isEmpty && codeModel.hasContract
		onTriggered: projectModel.deployProject();
	}

	Action {
		id: goToCompilationError
		text: qsTr("Go to Compilation Error")
		shortcut: "F4"
		onTriggered:
		{
			mainContent.codeEditor.goToCompilationError();
		}
	}

	Action {
		id: gasEstimationAction
		text: qsTr("Display Gas Estimation")
		shortcut: "Ctrl+G"
		checkable: true
		onTriggered: mainContent.codeEditor.displayGasEstimation(checked);
	}

	Action {
		id: optimizeCodeAction
		text: qsTr("Enable Optimized Compilation")
		shortcut: "Ctrl+Shift+O"
		checkable: true
		onTriggered: codeModel.setOptimizeCode(checked)
		Component.onCompleted: codeModel.setOptimizeCode(checked)
	}


	MessageDialog {
		id: alertMessageDialog
		icon: StandardIcon.Warning
		function pop(text)
		{
			this.text = text
			this.open()
		}
		onAccepted:
		{
			this.text = ""
		}
	}

	Settings {
		id: appSettings
		property alias gasEstimation: gasEstimationAction.checked
		property alias optimizeCode: optimizeCodeAction.checked
		property string nodeAddress: "http://localhost:7075"
		property alias displayCalls: displayCallsAction.checked
		property int systemPointSize: 11
		function getFormattedPointSize()
		{
			return systemPointSize
		}
	}
}

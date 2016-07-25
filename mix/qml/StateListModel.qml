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
/** @file StateListModel.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import org.elementrem.qml.QElement 1.0
import "js/QElementHelper.js" as QElementHelper
import "js/TransactionHelper.js" as TransactionHelper

Item {

	property alias model: stateListModel
	property var stateList: []
	property alias stateDialog: stateDialog
	property string defaultAccount: "cb73d9408c4720e230387d956eb0f829d8a4dd2c1055f96257167e14e7169074" //support for old project
	property string user2Secret: "db95780d2f30ca8211eec37cc110cc58a2d4f9308dea00ede6717a585efa2eb7"
	property string user3Secret: "090f51d202f847d2468fc87fb06f92c469b8a1927b4aecea796b6831f1331382"
	property string user4Secret: "e9bf58b523b4c94255cad51b3f7efdc85a0c970bf42f1d191c126bb6f49bfb9d"

	function fromPlainStateItem(s) {
		if (!s.accounts)
			s.accounts = [stateListModel.newAccount("1000000", QElement.Element, defaultAccount)]; //support for old project
		if (!s.contracts)
			s.contracts = [];

		var ret = {};
		ret.title = s.title;
		ret.transactions = s.transactions.filter(function(t) { return !t.stdContract; }).map(fromPlainTransactionItem); //support old projects by filtering std contracts
		if (s.blocks)
			ret.blocks = s.blocks.map(fromPlainBlockItem);
		ret.accounts = s.accounts.map(fromPlainAccountItem);
		ret.contracts = s.contracts.map(fromPlainAccountItem);
		ret.miner = s.miner;

		// support old projects
		if (!ret.blocks)
		{
			ret.blocks = [{
							  hash: "",
							  number: -1,
							  transactions: [],
							  status: "pending"
						  }]
			for (var j in ret.transactions)
				ret.blocks[0].transactions.push(fromPlainTransactionItem(toPlainTransactionItem(ret.transactions[j])))
		}
		return ret;
	}

	function fromPlainAccountItem(t)
	{
		return {
			name: t.name,
			address: t.address,
			secret: t.secret,
			balance: QElementHelper.createElement(t.balance.value, t.balance.unit),
			storage: t.storage,
			code: t.code,
		};
	}

	function fromPlainTransactionItem(t) {
		if (!t.sender)
			t.sender = defaultAccount; //support for old project

		var r = {
			type: t.type,
			contractId: t.contractId,
			functionId: t.functionId,
			url: t.url,
			value: QElementHelper.createElement(t.value.value, t.value.unit),
			gas: QElementHelper.createBigInt(t.gas.value),
			gasPrice: QElementHelper.createElement(t.gasPrice.value, t.gasPrice.unit),
			gasAuto: t.gasAuto,
			parameters: {},
			sender: t.sender,
			isContractCreation: t.isContractCreation,
			label: t.label,
			isFunctionCall: t.isFunctionCall,
			saveStatus: t.saveStatus
		};

		if (r.saveStatus === undefined)
			r.saveStatus = true

		if (r.isFunctionCall === undefined)
			r.isFunctionCall = true;

		if (!r.label)
			r.label = r.contractId + " - " + r.functionId;

		if (r.isContractCreation === undefined)
			r.isContractCreation = r.functionId === r.contractId;

		for (var key in t.parameters)
			r.parameters[key] = t.parameters[key];

		r.isCall = false
		return r;
	}

	function fromPlainBlockItem(b)
	{
		var r = {
			hash: b.hash,
			number: b.number,
			transactions: b.transactions.filter(function(t) { return !t.stdContract; }).map(fromPlainTransactionItem), //support old projects by filtering std contracts
			status: b.status
		}
		return r;
	}

	function toPlainStateItem(s) {
		return {
			title: s.title,
			blocks: s.blocks.map(toPlainBlockItem),
			transactions: s.transactions.filter(function(t) { return t.saveStatus; }).map(toPlainTransactionItem),
			accounts: s.accounts.map(toPlainAccountItem),
			contracts: s.contracts.map(toPlainAccountItem),
			miner: s.miner
		};
	}

	function getParamType(param, params)
	{
		for (var k in params)
		{
			if (params[k].declaration.name === param)
				return params[k].declaration.type;
		}
		return '';
	}

	function toPlainBlockItem(b)
	{
		var r = {
			hash: b.hash,
			number: b.number,
			transactions: b.transactions.filter(function(t) { return t.saveStatus; }).map(toPlainTransactionItem),
			status: b.status
		}
		return r;
	}

	function toPlainAccountItem(t)
	{
		return {
			name: t.name,
			secret: t.secret,
			balance: {
				value: t.balance.value,
				unit: t.balance.unit
			},
			address: t.address,
			storage: t.storage,
			code: t.code,
		};
	}

	function toPlainTransactionItem(t) {
		var r = {
			type: t.type,
			contractId: t.contractId,
			functionId: t.functionId,
			url: t.url,
			value: { value: t.value.value, unit: t.value.unit },
			gas: { value: t.gas.value() },
			gasAuto: t.gasAuto,
			gasPrice: { value: t.gasPrice.value, unit: t.gasPrice.unit },
			sender: t.sender,
			parameters: {},
			isContractCreation: t.isContractCreation,
			label: t.label,
			isFunctionCall: t.isFunctionCall,
			saveStatus: t.saveStatus
		};
		for (var key in t.parameters)
			r.parameters[key] = t.parameters[key];
		return r;
	}

	Connections {
		id: projectConnection
		property bool creating: false
		target: projectModel
		onProjectClosed: {
			stateListModel.clear();
			stateList = [];
			codeModel.reset();
		}
		onProjectLoading: stateListModel.loadStatesFromProject(projectData);
		onProjectFileSaving: {
			projectData.states = []
			for(var i = 0; i < stateListModel.count; i++)
			{
				projectData.states.push(toPlainStateItem(stateList[i]));
				stateListModel.set(i, stateList[i]);
			}
			projectData.defaultStateIndex = stateListModel.defaultStateIndex;
			stateListModel.data = projectData
		}
		onNewProject:
		{
			projectData.states = [];
			projectData.defaultStateIndex = 0;
			stateListModel.loadStatesFromProject(projectData);
			creating = true
		}
	}

	Connections
	{
		target: codeModel
		onNewContractCompiled:
		{
			if (projectConnection.creating)
			{
				projectConnection.creating = false
				mainContent.rightPane.bcLoader.createScenario(true)
			}
		}
	}

	Connections {
		target: codeModel
		onNewContractCompiled: {
			stateListModel.addNewContracts();
		}
		onContractRenamed: {
			stateListModel.renameContracts(_oldName, _newName);
		}
	}

	StateDialog {
		id: stateDialog
		onAccepted: {
			var item = stateDialog.getItem();
			saveState(item);
		}

		function saveState(item)
		{
			stateList[stateDialog.stateIndex].accounts = item.accounts
			stateList[stateDialog.stateIndex].contracts = item.contracts
			stateListModel.get(stateDialog.stateIndex).accounts = item.accounts
			stateListModel.get(stateDialog.stateIndex).contracts = item.contracts
			stateListModel.accountsValidated(item.accounts)
			stateListModel.contractsValidated(item.contracts)
			stateListModel.get(stateDialog.stateIndex).miner = item.miner
			stateList[stateDialog.stateIndex].miner = item.miner
			projectModel.saveProject()
		}
	}

	ListModel {
		id: stateListModel
		property int defaultStateIndex: 0
		property variant data
		signal accountsValidated(var _accounts)
		signal contractsValidated(var _contracts)
		signal defaultStateChanged;
		signal stateListModelReady;
		signal stateRun(int index)
		signal stateDeleted(int index)

		function setDefaultState(index)
		{
			defaultStateIndex = index
			stateListModel.defaultStateChanged()
		}

		function defaultTransactionItem()
		{
			return TransactionHelper.defaultTransaction();
		}

		function newAccount(_balance, _unit, _secret, _nickName)
		{
			if (!_secret)
				_secret = clientModel.newSecret();
			var address = clientModel.address(_secret);
			var name
			if (_nickName && _nickName !== "")
				name = _nickName
			else
				name = address
			var amount = QElementHelper.createElement(_balance, _unit)			
			return { name: name, secret: _secret, balance: amount, address: address };
		}

		function titleExists(stateList, title)
		{
			for (var k in stateList)
			{
				if (stateList[k].title === title)
					return true;
			}
			return false;
		}

		function correctTitle(title)
		{
			var copyIndex = 1;
			while (titleExists(stateList, title + "_" + copyIndex))
				copyIndex++;

			return title + "_" + copyIndex;
		}

		function duplicateState(index)
		{
			var state = stateList[index]
			var item = copyScenario(state)
			item.title = correctTitle(item.title)
			appendState(item)
			save()
		}

		function copyScenario(scenario)
		{
			return fromPlainStateItem(toPlainStateItem(scenario))
		}

		function createEmptyBlock()
		{
			return {
				hash: "",
				number: -1,
				transactions: [],
				status: "pending"
			}
		}

		function createDefaultState() {
			var item = {
				title: "",
				transactions: [],
				accounts: [],
				contracts: [],
				blocks: [{ status: "pending", number: 1, hash: "", transactions: []}]
			};

			var account = newAccount("1000000", QElement.Element, defaultAccount, "user1")
			item.accounts.push(account);
			item.miner = account;
			item.accounts.push(newAccount("1000000", QElement.Element, user2Secret, "user2"));
			item.accounts.push(newAccount("1000000", QElement.Element, user3Secret, "user3"));
			item.accounts.push(newAccount("1000000", QElement.Element, user4Secret, "user4"));

			//add constructors, //TODO: order by dependencies
			for(var c in codeModel.contracts) {
				var ctorTr = defaultTransactionItem();
				ctorTr.functionId = c;
				ctorTr.contractId = c;
				ctorTr.label = ctorTr.contractId + "." + ctorTr.contractId + "()"
				ctorTr.sender = item.accounts[0].secret;
				item.transactions.push(ctorTr);
				item.blocks[0].transactions.push(ctorTr)
			}
			return item;
		}

		function renameContracts(oldName, newName) {
			var changed = false;
			for(var c in codeModel.contracts) {
				for (var s = 0; s < stateListModel.count; s++) {
					var state = stateList[s];
					for (var t = 0; t < state.transactions.length; t++) {
						var transaction = state.transactions[t];
						if (transaction.contractId === oldName) {
							transaction.contractId = newName;
							if (transaction.functionId === oldName)
								transaction.functionId = newName;
							changed = true;
							state.transactions[t] = transaction;
						}
					}
					stateListModel.set(s, state);
					stateList[s] = state;
				}
			}
			if (changed)
				save();
		}

		function addNewContracts() {
			//add new contracts to empty states
			var changed = false;
			for (var c in codeModel.contracts) {
				for (var s = 0; s < stateListModel.count; s++) {
					var state = stateList[s];
					if (state.transactions.length === 0) {
						//append this contract
						var ctorTr = defaultTransactionItem();
						ctorTr.functionId = c;
						ctorTr.contractId = c;
						ctorTr.label = ctorTr.contractId + "." + ctorTr.contractId + "()";
						ctorTr.sender = state.accounts[0].secret;
						state.transactions.push(ctorTr);
						changed = true;
						stateListModel.set(s, state);
						stateList[s] = state;
					}
				}
			}
			if (changed)
				save();
		}

		function addState() {
			var item = createDefaultState();
			stateDialog.open(stateListModel.count, item);
		}

		function appendState(item)
		{
			stateListModel.append(item);
			stateList.push(item);
		}

		function reset()
		{
			stateListModel.clear()
			stateList = []
		}

		function editState(index) {
			stateDialog.open(index, stateList[index]);
		}

		function getState(index) {
			return stateList[index];
		}

		function debugDefaultState() {
			if (defaultStateIndex >= 0 && defaultStateIndex < stateList.length)
				runState(defaultStateIndex);
		}

		function runState(index) {
			var item = stateList[index];
			clientModel.setupScenario(item);
			stateRun(index);
		}

		function deleteState(index) {
			stateListModel.remove(index);
			stateList.splice(index, 1);
			if (index === defaultStateIndex)
			{
				defaultStateIndex = 0;
				defaultStateChanged();
			}
			else if (defaultStateIndex > index)
				defaultStateIndex--;
			save();
			stateDeleted(index);
		}

		function save() {
			projectModel.saveProject();
		}

		function defaultStateName()
		{
			if (stateList.length > 0)
				return stateList[defaultStateIndex].title;
			else
				return ""
		}

		function reloadStateFromProject(index)
		{
			if (data)
			{
				var item = fromPlainStateItem(data.states[index])
				stateListModel.set(index, item)
				stateList[index] = item
				return item
			}
		}

		function loadStatesFromProject(projectData)
		{
			data = projectData
			if (!projectData.states)
				projectData.states = [];
			if (projectData.defaultStateIndex !== undefined)
				defaultStateIndex = projectData.defaultStateIndex;
			else
				defaultStateIndex = 0;
			var items = projectData.states;
			stateListModel.clear();
			stateList = [];
			for(var i = 0; i < items.length; i++) {
				var item = fromPlainStateItem(items[i]);
				stateListModel.append(item);
				stateList.push(item);
			}
			stateListModelReady();
		}
	}
}

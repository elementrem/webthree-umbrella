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
/** @file DeploymentWorker.qml
 * @
 * 
 * 
 * Elementrem IDE client.
 */

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.3
import org.elementrem.qml.QElement 1.0
import org.elementrem.qml.CodeModel 1.0
import org.elementrem.qml.ClientModel 1.0
import "js/TransactionHelper.js" as TransactionHelper
import "js/NetworkDeployment.js" as NetworkDeploymentCode
import "js/QElementHelper.js" as QElementHelper
import "."

Item
{
	property string currentAccount
	property string gasPrice
	property alias gasPriceInt: gasPriceInt
	property variant balances: ({})
	property variant accounts: []
	property alias pooler: pooler
	property alias clientModelGasEstimation: clientModelGasEstimation
	signal gasPriceLoaded()
	signal nodeUnreachable(string message)
	signal nodeReachable

	function renewCtx()
	{
		accounts = []
		balances = {}
		var requests = [{
							//accounts
							jsonrpc: "2.0",
							method: "ele_accounts",
							params: [],
							id: 0
						}];

		TransactionHelper.rpcCall(requests, function(arg1, arg2)
		{
			nodeReachable()
			var ids = JSON.parse(arg2)[0].result;
			requests = [];
			accounts = []
			for (var k in ids)
			{
				requests.push({
								  //accounts
								  jsonrpc: "2.0",
								  method: "ele_getBalance",
								  params: [ids[k], 'latest'],
								  id: k
							  });
				accounts.push({ "id": ids[k] })
			}

			TransactionHelper.rpcCall(requests, function (request, response){
				var balanceRet = JSON.parse(response);
				balances = {}
				for (var k in balanceRet)
				{
					var element = QElementHelper.createElement(balanceRet[k].result, QElement.Mey);
					balances[accounts[k].id] = element
				}
			}, function(){});
		}, function(message){
			nodeUnreachable(message)
		});

		NetworkDeploymentCode.gasPrice(function(price) {
			gasPrice = price;
			gasPriceInt.setValue(price);
			gasPriceLoaded()
		}, function(){});
	}

	function balance(account)
	{
		for (var k in accounts)
		{
			if (accounts[k].id === account)
				return balances[account]
		}
		return null
	}

	function stopForInputError(inError)
	{
		errorDialog.text = "";
		if (inError.length > 0)
		{
			errorDialog.text = qsTr("The length of a string cannot exceed 32 characters.\nPlease verify the following value(s):\n\n")
			for (var k in inError)
				errorDialog.text += inError[k] + "\n";
			errorDialog.open();
			return true;
		}
		return false;
	}

	function forceStopPooling()
	{
		poolLog.stop()
	}

	function waitForTrReceipt(hash, callback)
	{
		poolLog.callBack = callback;
		poolLog.hash = hash
		poolLog.elapsed = 0;
		poolLog.start();
	}

	function verifyHash(tr, hash, callBack)
	{
		var h = {}
		h[tr] = hash
		verifyHashes(h, function (bn, trLost)
		{
			callBack(bn, trLost)
		});
	}

	function verifyHashes(trHashes, callback)
	{
		//trHashes : { "trLabel": 'hash' }
		var requests = [];
		var req = 0
		requests.push({
						  jsonrpc: "2.0",
						  method: "ele_blockNumber",
						  params: [],
						  id: req
					  });
		var label = []
		for (var k in trHashes)
		{
			req++
			label[req] = k
			requests.push({
							  jsonrpc: "2.0",
							  method: "ele_getTransactionReceipt",
							  params: [trHashes[k]],
							  id: req
						  });
		}

		TransactionHelper.rpcCall(requests, function (httpRequest, response){
			var ret = JSON.parse(response)
			var b = ret[0].result;
			var trLost = []
			for (var k in ret)
			{
				if (!ret[k].result)
					trLost.push(label[ret[k].id])
			}
			callback(parseInt(b, 16), trLost)
		});
	}

	Component.onCompleted:
	{
		renewCtx()
	}

	BigIntValue
	{
		id: gasPriceInt
	}

	function estimateGas(scenario)
	{
		var docs = {}
		for (var c in codeModel.contracts)
		{
			var docId = codeModel.contracts[c].documentId
			if (!docs[docId])
			{
				codeModelGasEstimation.registerCodeChange(docId, fileIo.readFile(docId));
				docs[docId] = ""
			}
		}

		var sce = projectModel.stateListModel.copyScenario(scenario)
		for (var k = 0; k < sce.blocks.length; k++)
		{
			for (var j = 0; j < sce.blocks[k].transactions.length; j++)
			{
				if (!sce.blocks[k].transactions[j].isFunctionCall)
					sce.blocks[k].transactions.splice(j, 1)
			}
		}
		clientModelGasEstimation.setupScenario(sce)
	}

	CodeModel {
		id: codeModelGasEstimation
	}

	ClientModel
	{
		id: clientModelGasEstimation
		codeModel: codeModelGasEstimation
		Component.onCompleted:
		{
			init("/mixbcgas")
		}
	}

	Timer
	{
		id: pooler
		interval: 5000
		repeat: true
		running: false
	}

	Timer
	{
		id: poolLog
		property var callBack
		property int elapsed
		property string hash
		interval: 2000
		running: false
		repeat: true
		onTriggered: {
			elapsed += interval;
			var requests = [];
			var jsonRpcRequestId = 0;
			requests.push({
							  jsonrpc: "2.0",
							  method: "ele_getTransactionReceipt",
							  params: [ hash ],
							  id: jsonRpcRequestId++
						  });
			TransactionHelper.rpcCall(requests, function (httpRequest, response){
				console.log(response)
				var receipt = JSON.parse(response)[0].result
				if (receipt)
				{
					stop();
					callBack(1, receipt);
				}
				else if (elapsed > 2500000)
				{
					stop();
					callBack(-1, null);
				}
			})
		}
	}
}


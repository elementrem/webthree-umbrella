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
/** @file ClientModel.h
 * @
 * 
 * 
 * Elementrem IDE client.
 */

#pragma once

#include <atomic>
#include <map>
#include <QMetaEnum>
#include <QString>
#include <QQmlListProperty>
#include <QVariantMap>
#include <QFuture>
#include <libelementrem/Account.h>
#include "QVariableDeclaration.h"
#include "MachineStates.h"
#include "QElement.h"

template <class... Is> class ModularServer;


namespace dev
{

namespace rpc
{
class EleFace;
class DBFace;
class Web3Face;
}
namespace ele { class FixedAccountHolder; }

namespace mix
{

class Web3Server;
class QElement;
class QDebugData;
class MixClient;
class CodeModel;
class CompiledContract;
struct SolidityType;

/// Backend transaction config class
struct TransactionSettings
{
	TransactionSettings() {}
	TransactionSettings(
		QString const& _contractId,
		QString const& _functionId,
		u256 _value,
		u256 _gas,
		bool _gasAuto,
		u256 _gasPrice,
		Secret const& _sender,
		bool _isContractCreation,
		bool _isFunctionCall
	):
		contractId(_contractId),
		functionId(_functionId),
		value(_value),
		gas(_gas),
		gasAuto(_gasAuto),
		gasPrice(_gasPrice),
		sender(_sender),
		isContractCreation(_isContractCreation),
		isFunctionCall(_isFunctionCall)
	{}
	TransactionSettings(QString const& _stdContractName, QString const& _stdContractUrl):
		contractId(_stdContractName),
		gasAuto(true),
		stdContractUrl(_stdContractUrl),
		isContractCreation(true),
		isFunctionCall(true)
	{}

	/// Contract name
	QString contractId;
	/// Contract function name
	QString functionId;
	/// Transaction value
	u256 value;
	/// Gas
	u256 gas;
	/// Calculate gas automatically
	bool gasAuto = true;
	/// Gas price
	u256 gasPrice;
	/// Mapping from contract function parameter name to value
	QVariantMap parameterValues;
	/// Standard contract url
	QString stdContractUrl;
	/// Sender
	Secret sender;
	/// Tr deploys a contract
	bool isContractCreation;
	/// Tr call a ctr function
	bool isFunctionCall;
};


/// UI Transaction log record
class RecordLogEntry: public QObject
{
	Q_OBJECT
	Q_ENUMS(TransactionException)
	Q_ENUMS(TxSource)
	Q_ENUMS(RecordType)
	/// Recording index
	Q_PROPERTY(unsigned recordIndex MEMBER m_recordIndex CONSTANT)
	/// Human readable transaction block and transaction index
	Q_PROPERTY(QString transactionIndex MEMBER m_transactionIndex CONSTANT)
	/// Contract name if any
	Q_PROPERTY(QString contract MEMBER m_contract CONSTANT)
	/// Function name if any
	Q_PROPERTY(QString function MEMBER m_function CONSTANT)
	/// Transaction value
	Q_PROPERTY(QString value MEMBER m_value CONSTANT)
	/// Receiving address
	Q_PROPERTY(QString address MEMBER m_address CONSTANT)
	/// Returned value or transaction address in case of creation
	Q_PROPERTY(QString returned MEMBER m_returned CONSTANT)
	/// true if call, false if transaction
	Q_PROPERTY(bool call MEMBER m_call CONSTANT)
	/// @returns record type
	Q_PROPERTY(RecordType type MEMBER m_type CONSTANT)
	/// Gas used
	Q_PROPERTY(QString gasUsed MEMBER m_gasUsed CONSTANT)
	/// Gas required
	Q_PROPERTY(QString gasRequired MEMBER m_gasRequired CONSTANT)
	/// Gas refunded
	Q_PROPERTY(QString gasRefunded MEMBER m_gasRefunded CONSTANT)
	/// Sender
	Q_PROPERTY(QString sender MEMBER m_sender CONSTANT)
	/// label
	Q_PROPERTY(QString label MEMBER m_label CONSTANT)
	/// input parameters
	Q_PROPERTY(QVariantMap parameters MEMBER m_inputParameters CONSTANT)
	/// return parameters
	Q_PROPERTY(QVariantMap returnParameters MEMBER m_returnParameters CONSTANT)
	/// logs
	Q_PROPERTY(QVariantList logs MEMBER m_logs CONSTANT)
	/// logs
	Q_PROPERTY(TxSource source MEMBER m_source CONSTANT)
	/// exception
	Q_PROPERTY(QString exception MEMBER m_strException CONSTANT)

public:
	enum RecordType
	{
		Transaction,
		Block
	};

	enum TxSource
	{
		Web3,
		MixGui
	};

	enum TransactionException
	{
		None,
		Unknown,
		BadRLP,
		InvalidFormat,
		OutOfGasIntrinsic,		///< Too little gas to pay for the base transaction cost.
		InvalidSignature,
		InvalidNonce,
		NotEnoughCash,
		OutOfGasBase,			///< Too little gas to pay for the base transaction cost.
		BlockGasLimitReached,
		BadInstruction,
		BadJumpDestination,
		OutOfGas,				///< Ran out of gas executing code of the transaction.
		OutOfStack,				///< Ran out of stack executing code of the transaction.
		StackUnderflow
	};

	RecordLogEntry():
		m_recordIndex(0), m_call(false), m_type(RecordType::Transaction) {}
	RecordLogEntry(
		unsigned _recordIndex,
		QString _transactionIndex,
		QString _contract,
		QString _function,
		QString _value,
		QString _address,
		QString _returned,
		bool _call,
		RecordType _type,
		QString _gasUsed,
		QString _gasRequired,
		QString _gasRefunded,
		QString _sender,
		QString _label,
		QVariantMap _inputParameters,
		QVariantMap _returnParameters,
		QVariantList _logs,
		TxSource _source,
		TransactionException _exception
	):
		m_recordIndex(_recordIndex),
		m_transactionIndex(_transactionIndex),
		m_contract(_contract),
		m_function(_function),
		m_value(_value),
		m_address(_address),
		m_returned(_returned),
		m_call(_call),
		m_type(_type),
		m_gasUsed(_gasUsed),
		m_gasRequired(_gasRequired),
		m_gasRefunded(_gasRefunded),
		m_sender(_sender),
		m_label(_label),
		m_inputParameters(_inputParameters),
		m_returnParameters(_returnParameters),
		m_logs(_logs),
		m_source(_source),
		m_exception(_exception)
	{
		QMetaEnum ex = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("TransactionException"));
		m_strException = QString(ex.valueToKey(m_exception));
	}

private:
	unsigned m_recordIndex;
	QString m_transactionIndex;
	QString m_contract;
	QString m_function;
	QString m_value;
	QString m_address;
	QString m_returned;
	bool m_call;
	RecordType m_type;
	QString m_gasUsed;
	QString m_gasRequired;
	QString m_gasRefunded;
	QString m_sender;
	QString m_label;
	QVariantMap m_inputParameters;
	QVariantMap m_returnParameters;
	QVariantList m_logs;
	TxSource m_source;
	TransactionException m_exception;
	QString m_strException;
};

/**
 * @brief Elementrem state control
 */
class ClientModel: public QObject
{
	Q_OBJECT

	enum ExecutionCtx
	{
		Rebuild,
		ExecuteTx,
		Idle
	};

public:
	ClientModel();
	~ClientModel();
	/// @returns true if currently executing contract code
	Q_PROPERTY(bool running MEMBER m_running NOTIFY runStateChanged)
	/// @returns true if currently mining
	Q_PROPERTY(bool mining MEMBER m_mining NOTIFY miningStateChanged)
	/// @returns deployed contracts addresses
	Q_PROPERTY(QVariantMap contractAddresses READ contractAddresses NOTIFY contractAddressesChanged)
	/// @returns deployed contracts gas costs
	Q_PROPERTY(QVariantList gasCosts READ gasCosts NOTIFY gasCostsChanged)
	/// @returns the last block
	Q_PROPERTY(RecordLogEntry* lastBlock READ lastBlock CONSTANT)
	/// @returns last transaction index
	Q_PROPERTY(QString lastTransactionIndex READ lastTransactionIndex CONSTANT)
	/// elementrem.js RPC request entry point
	/// @param _message RPC request in Json format
	/// @returns RPC response in Json format
	Q_INVOKABLE QString apiCall(QString const& _message);
	/// Simulate mining. Creates a new block
	Q_INVOKABLE void mine();
	/// Get/set code model. Should be set from qml
	Q_PROPERTY(CodeModel* codeModel MEMBER m_codeModel)
	/// Encode parameters
	Q_INVOKABLE QStringList encodeParams(QVariant const& _param, QString const& _contract, QString const& _function);
	/// Encode parameter
	Q_INVOKABLE QString encodeStringParam(QString const& _param);
	/// To Hex number
	Q_INVOKABLE QString toHex(QString const& _int);
	/// Add new account to the model
	Q_INVOKABLE void addAccount(QString const& _secret);
	/// Return the address associated with the current secret
	Q_INVOKABLE QString resolveAddress(QString const& _secret);
	/// Compute required gas for a list of transactions @arg _tr
	QBigInt computeRequiredGas(QVariantList _tr);
	/// init ele client
	Q_INVOKABLE void init(QString _dbpath);

public slots:
	/// Setup scenario, run transaction sequence, show debugger for the last transaction
	/// @param _state JS object with state configuration
	void setupScenario(QVariantMap _scenario);
	/// Execute the given @param _tr on the current state
	void executeTr(QVariantMap _tr);
	/// Show the debugger for a specified record
	Q_INVOKABLE void debugRecord(unsigned _index, QString const& _label);
	/// Show the debugger for an empty record
	Q_INVOKABLE void emptyRecord();
	/// Generate new secret
	Q_INVOKABLE QString newSecret();
	/// retrieve the address of @arg _secret
	Q_INVOKABLE QString address(QString const& _secret);
	/// Encode a string to ABI parameter. Returns a hex string
	Q_INVOKABLE QString encodeAbiString(QString _string);
	/// Clear state
	Q_INVOKABLE void onStateReset();
	/// Get the storage of the execution for the machine state
	Q_INVOKABLE QVariantMap contractStorageByMachineState(MachineState const&, dev::mix::CompiledContract const* _index);
	/// Get the storage of the execution for the given index. will return the storage at the end of the execution
	Q_INVOKABLE QVariantMap contractStorageByIndex(unsigned _index, QString const& _contractAddress);
	/// Get the contract storage
	QVariantMap contractStorage(std::unordered_map<u256, u256> _storage, dev::mix::CompiledContract const* _contract);

private slots:
	/// Update UI with machine states result. Display a modal dialog.
	void showDebugger();

signals:
	/// Internal error
	void internalError(QString _message) const;
	/// Transaction execution started
	void runStarted();
	/// Transaction execution completed successfully
	void runComplete();
	/// Mining has started
	void miningStarted();
	/// Mined a new block
	void miningComplete();
	/// Mining stopped or started
	void miningStateChanged();
	/// Transaction execution completed with error
	/// @param _message Error message
	void runFailed(QString const& _message);
	/// Contract address changed
	void contractAddressesChanged();
	/// Gas costs updated
	void gasCostsChanged();
	/// Execution state changed
	void newBlock();
	/// Execution state changed
	void runStateChanged();
	/// Show debugger window request
	void debugDataReady(QObject* _debugData);
	/// elementrem.js RPC response ready
	/// @param _message RPC response in Json format
	void apiResponse(QString const& _message);
	/// New transaction log entry
	void newRecord(RecordLogEntry* _r);
	/// State (transaction log) cleared
	void stateCleared();
	/// new state has been processed
	void newState(unsigned _record, QVariantMap _accounts);
	/// setupFinished (blockChain has been rebuilt).
	void setupFinished();
	/// setup scenario started
	void setupStarted();

private:
	RecordLogEntry* lastBlock() const;
	QString lastTransactionIndex() const;
	QVariantMap contractAddresses() const;
	QVariantList gasCosts() const;
	void executeSequence(std::vector<TransactionSettings> const& _sequence);
	Address deployContract(bytes const& _code, TransactionSettings const& _tr = TransactionSettings());
	void callAddress(Address const& _contract, bytes const& _data, TransactionSettings const& _tr);
	void onNewTransaction(RecordLogEntry::TxSource _source);
	void showDebuggerForTransaction(ExecutionResult const& _t, QString const& _label = "");
	QString resolveToken(std::pair<QString, int> const& _value);
	std::pair<QString, int> retrieveToken(QString const& _value);
	std::pair<QString, int> resolvePair(QString const& _contractId);
	QString serializeToken(std::pair<QString, int> const& _value) const;
	QVariant formatStorageValue(
		SolidityType const& _type,
		std::unordered_map<dev::u256,
		dev::u256> const& _storage,
		unsigned const& _offset,
		dev::u256 const& _slot
	);
	QVariant formatMemoryValue(SolidityType const& _type, bytes const& _value, u256& _offset);
	void processNextTransactions();
	void finalizeBlock();
	void stopExecution();
	void setupExecutionChain();
	TransactionSettings transaction(QVariant const& _tr) const;
	void manageException() const;

	std::atomic<bool> m_running;
	std::atomic<bool> m_mining;
	QFuture<void> m_runFuture;
	std::unique_ptr<MixClient> m_client;
	unsigned m_rpcConnectorId;
	std::unique_ptr<ModularServer<rpc::EleFace, rpc::DBFace, rpc::Web3Face>> m_web3Server;
	std::shared_ptr<ele::FixedAccountHolder> m_eleAccounts;
	ele::AccountMap m_accounts;
	std::vector<KeyPair> m_accountsSecret;
	QList<u256> m_gasCosts;
	std::map<std::pair<QString, int>, Address> m_contractAddresses;
	std::map<Address, QString> m_contractNames;
	std::map<QString, Address> m_stdContractAddresses;
	std::map<Address, QString> m_stdContractNames;
	CodeModel* m_codeModel = nullptr;
	QList<QVariantList> m_queueTransactions;
	mutable boost::shared_mutex x_queueTransactions;
	QString m_dbpath;
	ExecutionCtx m_executionCtx;
	QString m_lastTransactionIndex;
	QVariantMap m_deployedLibraries;
};

}
}

Q_DECLARE_METATYPE(dev::mix::RecordLogEntry*)

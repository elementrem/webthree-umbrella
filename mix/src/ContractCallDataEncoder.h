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
/** @file ContractCallDataEncoder.h
 * @
 * 
 * Elementrem IDE client.
 */

#pragma once

#include <boost/asio.hpp>
#include "QVariableDeclaration.h"

namespace dev
{
namespace mix
{

class QFunctionDefinition;
class QVariableDeclaration;
class QSolidityType;

/**
 * @brief Encode/Decode data to be sent to a transaction or to be displayed in a view.
 */
class ContractCallDataEncoder
{
public:
	ContractCallDataEncoder() {}
	/// Encode hash of the function to call.
	void encode(QFunctionDefinition const* _function);
	/// Encode data for corresponding type
	void encode(QVariant const& _data, SolidityType const& _type);
	/// Decode variable in order to be sent to QML view.
	QStringList decode(QList<QVariableDeclaration*> const& _dec, bytes _value, u256 const& _offset = 0);
	/// Decode @param _parameter
	QString decode(QVariableDeclaration* const& _param, bytes _value, u256 const& _offset = 0);
	/// Decode single variable
	QVariant decode(SolidityType const& _type, bytes const& _value, u256 const& _offset = 0);
	/// Get all encoded data encoded by encode function.
	bytes encodedData();
	/// Encode a string to bytes (in order to be used as funtion param)
	dev::bytes encodeStringParam(QString const& _str, unsigned _alignSize);
	/// Encode a string to ABI bytes
	dev::bytes encodeBytes(QString const& _str);
	/// Decode bytes from ABI
	dev::bytes decodeBytes(dev::bytes const& _rawValue);
	/// Decode struct
	QVariantList decodeStruct(SolidityType const& _type, dev::bytes const& _rawValue, u256& _offset);
	/// Decode array
	QJsonArray decodeArray(SolidityType const& _type, bytes const& _value, u256& pos);
	/// Decode array items
	QJsonValue decodeArrayContent(SolidityType const& _type, bytes const& _value, u256& pos);
	/// Decode enum
	QString decodeEnum(bytes _value);
	/// Decode type
	QVariant decodeType(SolidityType _type, bytes _value, u256& _readPosition);
	/// Decode int
	bigint decodeInt(dev::bytes const& _rawValue);
	/// Format storage
	QVariant formatStorageValue(SolidityType const& _type, std::unordered_map<u256, u256> const& _storage, unsigned _offset, u256 const& _slot, u256& _endSlot);
	/// Format array from storage
	QVariant formatStorageArray(SolidityType const& _type, std::unordered_map<u256, u256> const& _storage, unsigned _offset, u256 const& _slot, u256& _endSlot);
	/// Decode raw array
	QVariant decodeRawArray(SolidityType const& _type, bytes const& _value, u256& pos);
	/// Decode struct stored in storage
	QVariant formatStorageStruct(SolidityType const& _type, std::unordered_map<u256, u256> const& _storage, u256 _slot, u256& _endSlot);
	/// Decode value stored in memory
	QVariant formatMemoryValue(SolidityType const& _type, bytes const& _value, u256& _offset);

private:
	unsigned encodeSingleItem(QString const& _data, SolidityType const& _type, bytes& _dest);
	dev::bytes encodeInt(QString const& _str);
	QString toString(dev::bigint const& _int);
	dev::bytes encodeBool(QString const& _str);
	bool decodeBool(dev::bytes const& _rawValue);
	QString toString(bool _b);
	QString toString(dev::bytes const& _b);
	bool asString(dev::bytes const& _b, QString& _str);
	void encodeArray(QJsonArray const& _array, SolidityType const& _type, bytes& _content);
	QString toChar(dev::bytes const& _b);

private:
	bytes m_encodedData;
	bytes m_dynamicData;
	std::vector<std::pair<size_t, size_t>> m_dynamicOffsetMap;
	std::vector<std::pair<size_t, size_t>> m_staticOffsetMap;
};

}
}

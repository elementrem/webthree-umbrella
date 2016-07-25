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
/** @file QElement.h
 * @
 * 
 * Represent an amount of Element in QML (mapped to u256 in c++).
 */

#pragma once

#include <QObject>
#include <libelecore/Common.h>
#include "QBigInt.h"

namespace dev
{
namespace mix
{

class QElement: public QBigInt
{
	Q_OBJECT
	Q_ENUMS(ElementUnit)
	Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
	Q_PROPERTY(ElementUnit unit READ unit WRITE setUnit NOTIFY unitChanged)

public:
	enum ElementUnit
	{
		Uelement,
		Velement,
		Delement,
		Nelement,
		Yelement,
		Zelement,
		Eelement,
		Pelement,
		Telement,
		Gelement,
		Melement,
		Grand,
		Element,
		Finney,
		Szabo,
		Gmey,
		Mmey,
		Kmey,
		Mey
	};

	QElement(QObject* _parent = 0): QBigInt(dev::u256(0), _parent), m_currentUnit(ElementUnit::Mey) {}
	QElement(dev::u256 _value, ElementUnit _unit, QObject* _parent = 0): QBigInt(_value, _parent), m_currentUnit(_unit) {}
	~QElement() {}

	/// @returns user-friendly string representation of the amount of element. Invokable from QML.
	Q_INVOKABLE QString format() const;
	/// @returns the current amount of Element in Mey. Invokable from QML.
	Q_INVOKABLE QBigInt* toMey() const;
	/// @returns the current unit used. @returns nullptr if exception. Invokable from QML.
	Q_INVOKABLE ElementUnit unit() const { return m_currentUnit; }
	/// Set the unit to be used. Invokable from QML.
	Q_INVOKABLE void setUnit(ElementUnit const& _unit) { m_currentUnit = _unit; }
	/// Set the unit to be used. Invokable from QML.
	Q_INVOKABLE void setUnit(QString const& _unit);
	/// @returns the u256 value of the current amount of Element in Mey.
	dev::u256 toU256Mey() { return boost::get<dev::u256>(toMey()->internalValue()); }

private:
	ElementUnit m_currentUnit;
	void manageException() const;

signals:
	void valueChanged();
	void unitChanged();
};

}
}


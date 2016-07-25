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
/** @file QElement.cpp
 * @
 * 
 */

#include <QMetaEnum>
#include "QElement.h"

using namespace dev::mix;
using namespace std;

void QElement::manageException() const
{
	try
	{
		throw;
	}
	catch (boost::exception const& _e)
	{
		cerr << boost::diagnostic_information(_e);
	}
	catch (exception const& _e)
	{
		cerr << _e.what();
	}
	catch (...)
	{
		cerr << boost::current_exception_diagnostic_information();
	}
}

QString QElement::format() const
{
	try
	{
		return QString::fromStdString(dev::ele::formatBalance(boost::get<dev::u256>(toMey()->internalValue())));
	}
	catch (...)
	{
		manageException();
		return QString();
	}
}

QBigInt* QElement::toMey() const
{
	try
	{
		QMetaEnum units = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("ElementUnit"));
		const char* key = units.valueToKey(m_currentUnit);
		for (pair<dev::u256, string> rawUnit: dev::ele::units())
		{
			if (QString::fromStdString(rawUnit.second).toLower() == QString(key).toLower())
				return multiply(new QBigInt(rawUnit.first));
		}
	}
	catch (...)
	{
		manageException();
	}
	return nullptr;
}

void QElement::setUnit(QString const& _unit)
{
	try
	{
		QMetaEnum units = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("ElementUnit"));
		for (int k = 0; k < units.keyCount(); k++)
		{
			if (QString(units.key(k)).toLower() == _unit.toLower())
			{
				m_currentUnit = static_cast<ElementUnit>(units.keysToValue(units.key(k)));
				return;
			}
		}
	}
	catch (...)
	{
		manageException();
	}
}

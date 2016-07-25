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
/** @file QBasicNodeDefinition.cpp
 * @
 * 
 */

#include "QBasicNodeDefinition.h"
#include <libsolidity/ast/AST.h>

namespace dev
{
namespace mix
{

QBasicNodeDefinition::QBasicNodeDefinition(QObject* _parent, solidity::Declaration const* _d):
	QObject(_parent), m_name(QString::fromStdString(_d->name())), m_location(_d->location())
{
}

QBasicNodeDefinition::QBasicNodeDefinition(QObject* _parent, std::string const& _name):
	QObject(_parent), m_name(QString::fromStdString(_name))
{
}

}
}

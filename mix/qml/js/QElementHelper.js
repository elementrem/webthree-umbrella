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
/** @file QElementHelper.js
 * @
 * 
 * 
 * Elementrem IDE client.
 */

function createElement(_value, _unit, _parent)
{
	var elementComponent = Qt.createComponent("qrc:/qml/ElementValue.qml");
	var element = elementComponent.createObject();
	element.setValue(_value);
	element.setUnit(_unit);
	return element;
}

function createBigInt(_value)
{
	var bigintComponent = Qt.createComponent("qrc:/qml/BigIntValue.qml");
	var bigint = bigintComponent.createObject();
	bigint.setValue(_value);
	return bigint;
}


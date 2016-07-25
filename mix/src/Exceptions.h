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
/** @file Exceptions.h
 * 
 * 
 * Elementrem IDE client.
 */

#pragma once

#include <iosfwd>
#include <libdevcore/Exceptions.h>

class QTextDocument;
class QQmlError;

namespace dev
{
namespace mix
{

struct QmlLoadException: virtual Exception {};
struct FileIoException: virtual Exception {};
struct InvalidBlockException: virtual Exception {};
struct FunctionNotFoundException: virtual Exception {};
struct ExecutionStateException: virtual Exception {};
struct ParameterChangedException: virtual Exception {};
struct OutOfGasException: virtual Exception {};

using QmlErrorInfo = boost::error_info<struct tagQmlError, QQmlError>;
using FileError = boost::error_info<struct tagFileError, std::string>;
using BlockIndex = boost::error_info<struct tagBlockIndex, unsigned>;
using FunctionName = boost::error_info<struct tagFunctionName, std::string>;

}
}

std::ostream& operator<<(std::ostream& _out, QQmlError const& _error);
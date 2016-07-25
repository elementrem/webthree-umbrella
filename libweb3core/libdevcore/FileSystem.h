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
/** @file FileSystem.h
 * @authors
 *	 Eric Lombrozo <elombrozo@gmail.com>
 *	 
 * 
 */

#pragma once

#include <string>
#include "CommonIO.h"

namespace dev
{

/// Sets the data dir for the default ("elementrem") prefix.
void setDataDir(std::string const& _dir);
/// @returns the path for user data.
std::string getDataDir(std::string _prefix = "elementrem");
/// @returns the default path for user data, ignoring the one set by `setDataDir`.
std::string getDefaultDataDir(std::string _prefix = "elementrem");

}

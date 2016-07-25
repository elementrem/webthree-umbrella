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
/** CommandLineInterface.h
 * @
 * 
 */
#pragma once

#include <json/json.h>
#include <boost/program_options.hpp>

class CommandLineInterface
{
public:
	CommandLineInterface() {}

	/// Parse command line arguments and return false if we should not continue
	bool parseArguments(int argc, char** argv);
	/// Parse input file and check if test exists
	bool processInput();
	/// Start FixedJsonRpcServer
	void actOnInput();

private:

	/// Compiler arguments variable map
	boost::program_options::variables_map m_args;

	/// loaded json test case
	Json::Value m_json;
};


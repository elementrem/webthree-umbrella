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
/** main.cpp
 * @
 * 
 */

#include "CommandLineInterface.h"

int main(int argc, char** argv)
{
	CommandLineInterface cli;
	if (!cli.parseArguments(argc, argv))
		return 1;
	if (!cli.processInput())
		return 1;
	cli.actOnInput();

	return 0;
}

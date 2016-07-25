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
/** @file main.cpp
 * @
 * 
 * Elementrem IDE client.
 */

#include <iostream>
#include <stdlib.h>
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include "MixApplication.h"

using namespace dev::mix;

int main(int _argc, char* _argv[])
{
	try
	{
		MixApplication::initialize();
		MixApplication app(_argc, _argv);
		return app.exec();
	}
	catch (boost::exception const& _e)
	{
		std::cerr << boost::diagnostic_information(_e);
	}
	catch (std::exception const& _e)
	{
		std::cerr << _e.what();
	}
	catch (...)
	{
		std::cerr << boost::current_exception_diagnostic_information();
	}
}

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
/** @file Compiler.cpp
 * 
 * 
 */

#include "Compiler.h"
#include "Parser.h"
#include "CompilerState.h"
#include "CodeFragment.h"

using namespace std;
using namespace dev;
using namespace dev::ele;

bytes dev::ele::compileLLL(string const& _src, bool _opt, vector<string>* _errors)
{
	try
	{
		CompilerState cs;
		cs.populateStandard();
		auto f = CodeFragment::compile(_src, cs);
		bytes ret = f.assembly(cs).optimise(_opt).assemble().bytecode;
		for (auto i: cs.treesToKill)
			killBigints(i);
		return ret;
	}
	catch (Exception const& _e)
	{
		if (_errors)
		{
			_errors->push_back("Parse error.");
			_errors->push_back(diagnostic_information(_e));
		}
	}
	catch (std::exception)
	{
		if (_errors)
			_errors->push_back("Parse error.");
	}
	return bytes();
}

std::string dev::ele::compileLLLToAsm(std::string const& _src, bool _opt, std::vector<std::string>* _errors)
{
	try
	{
		CompilerState cs;
		cs.populateStandard();
		string ret = CodeFragment::compile(_src, cs).assembly(cs).optimise(_opt).out();
		for (auto i: cs.treesToKill)
			killBigints(i);
		return ret;
	}
	catch (Exception const& _e)
	{
		if (_errors)
			_errors->push_back(diagnostic_information(_e));
	}
	catch (std::exception)
	{
		if (_errors)
			_errors->push_back("Parse error.");
	}
	return string();
}

string dev::ele::parseLLL(string const& _src)
{
	sp::utree o;
	try
	{
		parseTreeLLL(_src, o);
	}
	catch (...) {}
	ostringstream ret;
	debugOutAST(ret, o);
	killBigints(o);
	return ret.str();
}

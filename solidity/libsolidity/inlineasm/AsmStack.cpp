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
/**
 * @
 * 
 * Full-stack Solidity inline assember.
 */

#include <libsolidity/inlineasm/AsmStack.h>
#include <memory>
#include <libevmasm/Assembly.h>
#include <libevmasm/SourceLocation.h>
#include <libsolidity/inlineasm/AsmParser.h>
#include <libsolidity/inlineasm/AsmCodeGen.h>

using namespace std;
using namespace dev;
using namespace dev::solidity;
using namespace dev::solidity::assembly;

bool InlineAssemblyStack::parse(const std::shared_ptr<Scanner>& _scanner)
{
	m_parserResult = make_shared<Block>();
	Parser parser(m_errors);
	auto result = parser.parse(_scanner);
	if (!result)
		return false;
	*m_parserResult = std::move(*result);
	return true;
}

ele::Assembly InlineAssemblyStack::assemble()
{
	CodeGenerator codeGen(*m_parserResult, m_errors);
	return codeGen.assemble();
}


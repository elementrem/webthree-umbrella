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
 * Versioning.
 */

#include <libsolidity/interface/Version.h>
#include <string>
#include <libdevcore/CommonData.h>
#include <libdevcore/Common.h>
#include <libsolidity/interface/Utils.h>
#include <solidity/BuildInfo.h>

using namespace dev;
using namespace dev::solidity;
using namespace std;

char const* dev::solidity::VersionNumber = ELE_PROJECT_VERSION;

string const dev::solidity::VersionString =
	string(dev::solidity::VersionNumber) +
	"-" +
	string(DEV_QUOTED(ELE_COMMIT_HASH)).substr(0, 8) +
	(ELE_CLEAN_REPO ? "" : "*") +
	"/" DEV_QUOTED(ELE_BUILD_TYPE) "-" DEV_QUOTED(ELE_BUILD_PLATFORM);


bytes dev::solidity::binaryVersion()
{
	bytes ret{0};
	size_t i = 0;
	auto parseDecimal = [&]()
	{
		size_t ret = 0;
		solAssert('0' <= VersionString[i] && VersionString[i] <= '9', "");
		for (; i < VersionString.size() && '0' <= VersionString[i] && VersionString[i] <= '9'; ++i)
			ret = ret * 10 + (VersionString[i] - '0');
		return ret;
	};
	ret.push_back(byte(parseDecimal()));
	solAssert(i < VersionString.size() && VersionString[i] == '.', "");
	++i;
	ret.push_back(byte(parseDecimal()));
	solAssert(i < VersionString.size() && VersionString[i] == '.', "");
	++i;
	ret.push_back(byte(parseDecimal()));
	solAssert(i < VersionString.size() && VersionString[i] == '-', "");
	++i;
	solAssert(i + 7 < VersionString.size(), "");
	bytes commitHash = fromHex(VersionString.substr(i, 8));
	if (commitHash.empty())
		commitHash = bytes(4, 0);
	ret += commitHash;
	solAssert(ret.size() == 1 + 3 + 4, "");

	return ret;
}


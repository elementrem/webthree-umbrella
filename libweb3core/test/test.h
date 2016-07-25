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
/** @file test.h
 * 
 * 
 * Stub for generating main boost.test module.
 * Original code taken from boost sources.
 */

#pragma once
#include <boost/test/unit_test.hpp>

namespace dev
{
namespace test
{

	std::string getTestPath();

	class Options
	{
	public:
		bool performance = false;
		bool nonetwork = false;

		/// Get reference to options
		/// The first time used, options are parsed
		static Options const& get();

	private:
		Options();
		Options(Options const&) = delete;
	};

}}

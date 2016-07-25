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
/** @file DB.h
 * 
 * 
 */

#pragma once

#pragma warning(push)
#pragma warning(disable: 4100 4267)
#if ELE_ROCKSDB
#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>
namespace ldb = rocksdb;
#else
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
namespace ldb = leveldb;
#endif
#pragma warning(pop)
#define DEV_LDB 1

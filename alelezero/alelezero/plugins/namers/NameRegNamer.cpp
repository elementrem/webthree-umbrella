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
/** @file NameRegNamer.h
 * 
 * 
 */

#include "NameRegNamer.h"
#include <QSettings>
#include <libdevcore/Log.h>
#include <libelementrem/Client.h>
#include <libalele/AleleFace.h>
#include "ZeroFace.h"

using namespace std;
using namespace dev;
using namespace ele;
using namespace alele;
using namespace zero;

ZERO_NOTE_PLUGIN(NameRegNamer);

NameRegNamer::NameRegNamer(ZeroFace* _m):
	AccountNamerPlugin(_m, "NameRegNamer")
{
}

NameRegNamer::~NameRegNamer()
{
}

string NameRegNamer::toName(Address const& _a) const
{
	for (auto const& r: m_registrars)
	{
		string n = abiOut<string>(alele()->elementrem()->call(Address(1), 0, r, abiIn("name(address)", _a), 1000000, DefaultGasPrice, PendingBlock, FudgeFactor::Lenient).output);
		if (!n.empty())
			return n;
	}
	return string();
}

Address NameRegNamer::toAddress(std::string const& _n) const
{
	for (auto const& r: m_registrars)
		if (Address a = abiOut<Address>(alele()->elementrem()->call(r, abiIn("addr(string)", _n)).output))
			return a;
	return Address();
}

Addresses NameRegNamer::knownAddresses() const
{
	return m_knownCache;
}

void NameRegNamer::killRegistrar(Address const& _r)
{
	if (m_filters.count(_r))
	{
		alele()->uninstallWatch(m_filters.at(_r));
		m_filters.erase(_r);
	}
	for (auto i = m_registrars.begin(); i != m_registrars.end();)
		if (*i == _r)
			i = m_registrars.erase(i);
		else
			++i;
}

void NameRegNamer::updateCache()
{
//	m_forwardCache.clear();
//	m_reverseCache.clear();
	m_knownCache.clear();
#if ELE_FATDB
	for (auto const& r: m_registrars)
		for (u256 const& a: keysOf(elementrem()->storageAt(r)))
			if (a > u256(1) << 253 && a < ((u256(1) << 253) + (u256(1) << 160)))
				m_knownCache.push_back(Address((u160)(a - (u256(1) << 253) - 1)));
#endif // ELE_FATDB
}

void NameRegNamer::readSettings(QSettings const& _s)
{
	(void)_s;
	while (!m_registrars.empty())
		killRegistrar(m_registrars.back());

	Address a("5e70c0bbcd5636e0f9f9316e9f8633feb64d4050");
	m_registrars.push_back(a);
	m_filters[a] = alele()->installWatch(LogFilter().address(a), [=](LocalisedLogEntries const&){ updateCache(); });

	noteKnownChanged();
}

void NameRegNamer::writeSettings(QSettings&)
{
}

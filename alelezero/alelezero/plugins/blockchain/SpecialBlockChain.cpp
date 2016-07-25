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
/** @file SpecialBlockChain.cpp
 * 
 * 
 */

#include "SpecialBlockChain.h"
#include <QInputDialog>
#include <libelementrem/Client.h>
#include <libalele/AleleFace.h>
#include "ZeroFace.h"
using namespace std;
using namespace dev;
using namespace ele;
using namespace alele;
using namespace zero;

ZERO_NOTE_PLUGIN(SpecialBlockChain);

SpecialBlockChain::SpecialBlockChain(ZeroFace* _z):
	Plugin(_z, "SpecialBlockChain")
{
	addSeparator("Block Chain", "menuSpecial");
	connect(addMenuItem("Rewind", "menuSpecial"), SIGNAL(triggered()), SLOT(rewind()));
	connect(addMenuItem("Inject", "menuSpecial"), SIGNAL(triggered()), SLOT(inject()));
	connect(addMenuItem("Retry Unknown", "menuSpecial"), SIGNAL(triggered()), SLOT(retryUnknown()));
	connect(addMenuItem("Kill", "menuSpecial"), SIGNAL(triggered()), SLOT(kill()));
}

void SpecialBlockChain::rewind()
{
	bool ok;
	int n = QInputDialog::getInt(zero(), "Rewind Chain", "Enter the number of the new chain head.", elementrem()->number() * 9 / 10, 1, elementrem()->number(), 1, &ok);
	if (ok)
	{
		elementrem()->rewind(n);
		zero()->noteAllChange();
	}
}

void SpecialBlockChain::inject()
{
	QString s = QInputDialog::getText(zero(), "Inject Block", "Enter block dump in hex");
	try
	{
		bytes b = fromHex(s.toStdString(), WhenError::Throw);
		elementrem()->injectBlock(b);
	}
	catch (BadHexCharacter& _e)
	{
		cwarn << "invalid hex character, transaction rejected";
		cwarn << boost::diagnostic_information(_e);
	}
	catch (...)
	{
		cwarn << "block rejected";
	}
}

void SpecialBlockChain::retryUnknown()
{
	elementrem()->retryUnknown();
}

void SpecialBlockChain::kill()
{
	zero()->allStop();
	elementrem()->killChain();
	zero()->carryOn();
}


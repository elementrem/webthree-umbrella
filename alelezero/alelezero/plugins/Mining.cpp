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
/** @file Mining.cpp
 * 
 * 
 */

#include "Mining.h"
#include <QMenuBar>
#include <QTimer>
#include <QActionGroup>
#include <libdevcore/Log.h>
#include <libelhashseal/ElhashAux.h>
#include <libelementrem/Client.h>
#include <libelhashseal/ElhashClient.h>
#include <libalele/AleleFace.h>
#include "ZeroFace.h"
using namespace std;
using namespace dev;
using namespace ele;
using namespace alele;
using namespace zero;

ZERO_NOTE_PLUGIN(Mining);

Mining::Mining(ZeroFace* _m):
	Plugin(_m, "Mining")
{
	QAction* mine = addMenuItem("Mine", "menuMining", false, "&Mining");
	mine->setCheckable(true);
	connect(mine, &QAction::triggered, [=](){
		if (elementrem()->wouldSeal() != mine->isChecked())
		{
			cdebug << "Author" << elementrem()->author() << "(" << alele()->author() << ")";
			if (mine->isChecked())
				elementrem()->startSealing();
			else
				elementrem()->stopSealing();
		}
	});
	connect(addMenuItem("Prepare Next DAG", "menuMining"), &QAction::triggered, [&](){
		ElhashAux::computeFull(
			ElhashAux::seedHash(
				elementrem()->blockChain().number() + ELHASH_EPOCH_LENGTH
			)
		);
	});

	auto g = new QActionGroup(this);
	g->setExclusive(true);
	auto translate = [](string s) { return s == "cpu" ? "CPU Miner" : s == "opencl" ? "OpenCL (GPU) Miner" : s + " Miner"; };
	for (auto i: elementrem()->sealers())
	{
		QAction* a = addMenuItem(QString::fromStdString(translate(i)), "menuMining", i == elementrem()->sealers().front());
		connect(a, &QAction::triggered, [=](){ elementrem()->setSealer(i); });
		g->addAction(a);
		a->setCheckable(true);
		if (i == elementrem()->sealer())
			a->setChecked(true);
	}

	{
		QTimer* t = new QTimer(this);
		connect(t, &QTimer::timeout, [=](){
			mine->setChecked(elementrem()->wouldSeal());
		});
		t->start(1000);
	}
}

Mining::~Mining()
{
}

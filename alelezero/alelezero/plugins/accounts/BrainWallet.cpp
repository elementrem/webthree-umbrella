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
/** @file BrainWallet.h
 * 
 * 
 */

#include "BrainWallet.h"
#include <QMenu>
#include <QDialog>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <libdevcore/Log.h>
#include <libdevcrypto/WordList.h>
#include <libelecore/KeyManager.h>
#include <libelementrem/Client.h>
#include <libalele/AleleFace.h>
#include "ZeroFace.h"
#include "ui_BrainWallet.h"
using namespace std;
using namespace dev;
using namespace ele;
using namespace alele;
using namespace zero;

ZERO_NOTE_PLUGIN(BrainWallet);

BrainWallet::BrainWallet(ZeroFace* _m):
	Plugin(_m, "BrainWallet")
{
	connect(addMenuItem("New Brain Wallet...", "menuAccounts", true), SIGNAL(triggered()), SLOT(create()));
}

BrainWallet::~BrainWallet()
{
}

void BrainWallet::create()
{
	QDialog d;
	Ui_BrainWallet u;
	u.setupUi(&d);
	d.setWindowTitle("New Brain Wallet");
	connect(u.generate, &QPushButton::clicked, [&](){
		boost::random_device d;
		boost::random::uniform_int_distribution<unsigned> pickWord(0, WordList.size() - 1);
		QString t;
		for (int i = 0; i < 13; ++i)
			t += (t.size() ? " " : "") + QString::fromStdString(WordList[pickWord(d)]);
		u.seed->setPlainText(t);
	});

	if (d.exec() == QDialog::Accepted)
	{
		alele()->keyManager().importBrain(u.seed->toPlainText().trimmed().toStdString(), u.name->text().toStdString(), u.hint->toPlainText().toStdString());
		alele()->noteKeysChanged();
	}
}
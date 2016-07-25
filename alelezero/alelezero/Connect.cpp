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
/** @file Connect.cpp
 * @author Alex Leverington <nessence@gmail.com>
 * 
 */

#include "Connect.h"
#include <libp2p/Host.h>
#include "ui_Connect.h"
using namespace dev;
using namespace alele;

Connect::Connect(QWidget* _parent):
	QDialog(_parent),
	ui(new Ui::Connect)
{
	ui->setupUi(this);
}

Connect::~Connect()
{
	delete ui;
}

void Connect::setEnvironment(QStringList const& _nodes)
{
	if (ui->host->count() == 0)
		ui->host->addItems(_nodes);
}

void Connect::reset()
{
	ui->required->setChecked(true);
}

QString Connect::host()
{
	return ui->host->currentText();
}

bool Connect::required()
{
	return ui->required->isChecked();
}

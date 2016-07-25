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
/** @file ExportState.h
 * 
 * 
 */

#pragma once

#if ELE_FATDB

#include <memory>
#include <QDialog>
#include <libelecore/Common.h>
#include "Plugin.h"

namespace Ui { class ExportState; }

namespace dev
{

namespace ele { class Client; }

namespace alele
{
namespace zero
{

class ExportState: public QObject, public Plugin
{
	Q_OBJECT

public:
	ExportState(ZeroFace* _m);

private slots:
	void create();
};

class ExportStateDialog: public QDialog
{
	Q_OBJECT

public:
	ExportStateDialog(ZeroFace* _m);
	virtual ~ExportStateDialog();

private slots:
	void on_block_editTextChanged();
	void on_block_currentIndexChanged(int _index);
	void on_saveButton_clicked();

private:
	void fillBlocks();
	void fillContracts();
	void generateJSON();

	AleleFace* alele() const;

	ZeroFace* m_main;
	std::unique_ptr<Ui::ExportState> m_ui;
	int m_recentBlocks = 0;
	ele::BlockNumber m_block = ele::LatestBlock;
};

}
}
}

#endif //ELE_FATDB

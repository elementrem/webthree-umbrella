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
/** @file SendDialog.cpp
 * 
 * 
 */

#pragma once

#include <QDialog>
#include <libelecore/Common.h>

namespace Ui {
class SendDialog;
}

namespace dev
{
namespace alele
{

class AleleFace;

class SendDialog: public QDialog
{
	Q_OBJECT

public:
	SendDialog(QWidget* _parent, AleleFace* _alele);
	~SendDialog();

private slots:
	void on_send_clicked();
	void on_cancel_clicked();
	void on_value_textChanged(QString _s);
	void on_to_textChanged(QString _s);

private:
	void updateProblem();
	void adjustDialogWidth();

	Ui::SendDialog* m_ui;
	AleleFace* m_alele;

	u256 m_value = Invalid256;
	Address m_to;
	bytes m_data;
};

}
}

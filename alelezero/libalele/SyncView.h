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
/** @file DownloadView.h
 * 
 * 
 */

#pragma once

#ifdef Q_MOC_RUN
#define BOOST_MPL_IF_HPP_INCLUDED
#endif

#include <list>
#include <QtWidgets/QWidget>
#ifndef Q_MOC_RUN
#include <libelementrem/Client.h>
#endif

namespace dev
{

namespace ele { class Client; }

namespace alele
{

class AleleFace;

class SyncView: public QWidget
{
	Q_OBJECT

public:
	SyncView(QWidget* _p = nullptr);

	void setAlele(AleleFace const* _a) { m_alele = _a; }

private:
	virtual void timerEvent(QTimerEvent*);
	virtual void paintEvent(QPaintEvent*);

	AleleFace const* m_alele = nullptr;

	unsigned m_lastSyncFrom = (unsigned)-1;
	unsigned m_lastSyncCount = 0;
	bool m_wasEstimate = false;

	int m_secondTimer;
};

}
}

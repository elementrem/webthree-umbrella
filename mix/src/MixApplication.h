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
/** @file MixApplication.h
 * @
 * 
 * This class will be use instead of QApplication to launch the application. the method 'notify' allows to catch all exceptions.
 * Not use for now: TODO.
 */

#pragma once

#include <memory>
#include <QApplication>

class QQmlApplicationEngine;

namespace dev
{
namespace mix
{

extern char const* VersionNumber;

class ApplicationService: public QObject
{
	Q_OBJECT
	Q_PROPERTY(int systemPointSize READ systemPointSize CONSTANT)
	Q_PROPERTY(bool haveWebEngine READ haveWebEngine CONSTANT)
	Q_PROPERTY(QString solidityVersionNumber READ solidityVersionNumber CONSTANT)
	Q_PROPERTY(QString solidityVersionString READ solidityVersionString CONSTANT)
	Q_PROPERTY(QString qtVersion READ qtVersion CONSTANT)

public:
	ApplicationService();
	int systemPointSize() const { return m_systemPointSize; }
	QString solidityVersionString() { return m_solidityVersionString; }
	QString solidityVersionNumber() { return m_solidityVersionNumber; }
	bool haveWebEngine() const { return true; }
	QString qtVersion() { return m_qtVersion; }

private:
	int m_systemPointSize = 0;
	QString m_solidityVersionString;
	QString m_solidityVersionNumber;
	QString m_qtVersion;
};


class MixApplication: public QApplication
{
	Q_OBJECT

public:
	MixApplication(int& _argc, char* _argv[]);
	static void initialize();
	virtual ~MixApplication();
	QQmlApplicationEngine* engine() { return m_engine.get(); }
	bool notify(QObject* _receiver, QEvent* _event) override;

private:
	std::unique_ptr<QQmlApplicationEngine> m_engine;
};

}
}

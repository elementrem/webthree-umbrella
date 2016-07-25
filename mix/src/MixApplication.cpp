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
/** @file MixApplication.cpp
 * @
 * 
 */

#include "MixApplication.h"
#include <boost/exception/diagnostic_information.hpp>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QIcon>
#include <QFont>
#include <QtWebEngine/QtWebEngine>
#include <libsolidity/interface/Version.h>
#include <mix/BuildInfo.h>
#include "CodeModel.h"
#include "ClientModel.h"
#include "FileIo.h"
#include "QElement.h"
#include "QVariableDeclaration.h"
#include "SortFilterProxyModel.h"
#include "Clipboard.h"
#include "HttpServer.h"
#include "InverseMouseArea.h"

using namespace dev::mix;

char const* dev::mix::VersionNumber = ELE_PROJECT_VERSION;

ApplicationService::ApplicationService()
{
	QtWebEngine::initialize();
	QFont f;

	m_systemPointSize = f.pointSize();
	m_solidityVersionNumber = QString(dev::solidity::VersionNumber);
	m_solidityVersionString = QString::fromStdString(dev::solidity::VersionString);
	m_qtVersion = QString::fromStdString(qVersion());
}

MixApplication::MixApplication(int& _argc, char* _argv[]):
	QApplication(_argc, _argv), m_engine(new QQmlApplicationEngine())
{
	#if __linux
		setWindowIcon(QIcon(":/res/Mix-1024.png"));
	#endif
	m_engine->load(QUrl("qrc:/qml/Application.qml"));
}

void MixApplication::initialize()
{
#if __linux
	//work around ubuntu appmenu-qt5 bug
	//https://bugs.launchpad.net/ubuntu/+source/appmenu-qt5/+bug/1323853
	putenv((char*)"QT_QPA_PLATFORMTHEME=");
	putenv((char*)"QSG_RENDER_LOOP=threaded");
#endif
#if (defined(_WIN32) || defined(_WIN64))
	if (!getenv("OPENSSL_CONF"))
		putenv((char*)"OPENSSL_CONF=c:\\");
#endif

	setOrganizationName(tr("Elementrem"));
	setOrganizationDomain(tr("elementrem.org"));
	setApplicationName(tr("Mix"));
	setApplicationVersion(dev::mix::VersionNumber);

	qmlRegisterType<CodeModel>("org.elementrem.qml.CodeModel", 1, 0, "CodeModel");
	qmlRegisterType<ClientModel>("org.elementrem.qml.ClientModel", 1, 0, "ClientModel");
	qmlRegisterType<ApplicationService>("org.elementrem.qml.ApplicationService", 1, 0, "ApplicationService");
	qmlRegisterType<FileIo>("org.elementrem.qml.FileIo", 1, 0, "FileIo");
	qmlRegisterType<QElement>("org.elementrem.qml.QElement", 1, 0, "QElement");
	qmlRegisterType<QBigInt>("org.elementrem.qml.QBigInt", 1, 0, "QBigInt");
	qmlRegisterType<QVariableDeclaration>("org.elementrem.qml.QVariableDeclaration", 1, 0, "QVariableDeclaration");
	qmlRegisterType<RecordLogEntry>("org.elementrem.qml.RecordLogEntry", 1, 0, "RecordLogEntry");
	qmlRegisterType<SortFilterProxyModel>("org.elementrem.qml.SortFilterProxyModel", 1, 0, "SortFilterProxyModel");
	qmlRegisterType<QSolidityType>("org.elementrem.qml.QSolidityType", 1, 0, "QSolidityType");
	qmlRegisterType<Clipboard>("org.elementrem.qml.Clipboard", 1, 0, "Clipboard");
	qmlRegisterType<HttpServer>("HttpServer", 1, 0, "HttpServer");
	qmlRegisterType<InverseMouseArea>("org.elementrem.qml.InverseMouseArea", 1, 0, "InverseMouseArea");
	qRegisterMetaType<CodeModel*>("CodeModel*");
	qRegisterMetaType<ClientModel*>("ClientModel*");
}

MixApplication::~MixApplication()
{
}

bool MixApplication::notify(QObject* _receiver, QEvent* _event)
{
	try
	{
		return QApplication::notify(_receiver, _event);
	}
	catch (std::exception e)
	{
		std::cerr << e.what();
	}
	catch (...)
	{
		std::cerr << boost::current_exception_diagnostic_information();
	}
	return false;
}

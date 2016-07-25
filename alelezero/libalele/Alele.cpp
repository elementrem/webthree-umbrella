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
/** @file Alele.h
 * 
 * 
 */

#include "Alele.h"
#include <QTimer>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>
#include <libdevcore/Log.h>
#include <libp2p/Host.h>
#include <libelecore/ICAP.h>
#include <libelementrem/Client.h>
#include <libelhashseal/GenesisInfo.h>
#include "ui_GetPassword.h"
using namespace std;
using namespace dev;
using namespace p2p;
using namespace ele;
using namespace alele;

Alele::Alele(QObject* _parent):
	AleleFace(_parent)
{
}

Alele::~Alele()
{
	close();
	m_destructing = true;
}

void Alele::openKeyManager()
{
	if (!getPassword("Enter your MASTER account password.", "Master password", "", [&](string const& s){ return !!keyManager().load(s); }, DefaultPasswordFlags, "The password you entered is incorrect. If you have forgotten your password, and you wish to start afresh, manually remove the file: " + getDataDir("elementrem") + "/keys.info").second)
		exit(-1);
}

void Alele::createKeyManager()
{
	auto p = getPassword("Enter a MASTER password for your key store. Make it strong. You probably want to write it down somewhere and keep it safe and secure; your identity will rely on this - you never want to lose it.", "Master Password", string(), DoNotVerify, NeedConfirm, "You must have a master password to use this program. Exiting.");
	if (!p.second)
		exit(-1);

	keyManager().create(p.first);
	keyManager().import(ICAP::createDirect(), "Default identity");
}

void Alele::init(OnInit _onInit, string const& _clientVersion, string const& _nodeName)
{
	m_clientVersion = _clientVersion;
	m_nodeName = _nodeName;

	// Get options
	m_dbPath = getDataDir();

	ele::Network network = ele::Network::Frontier;

	for (int i = 1; i < qApp->arguments().size(); ++i)
	{
		QString arg = qApp->arguments()[i];
		if (arg == "--frontier")
			network = ele::Network::Frontier;
		else if (arg == "--olympic")
			network = ele::Network::Olympic;
		else if (arg == "--morden" || arg == "--testnet")
			network = ele::Network::Morden;
		else if ((arg == "--genesis-json" || arg == "--genesis" || arg == "--config") && i + 1 < qApp->arguments().size())
		{
			network = ele::Network::Special;
			m_baseParams = ChainParams(contentsString(qApp->arguments()[++i].toStdString()));
		}
		else if ((arg == "--db-path" || arg == "-d") && i + 1 < qApp->arguments().size())
			m_dbPath = qApp->arguments()[++i].toStdString();
	}

	if (!dev::contents(m_dbPath + "/genesis.json").empty())
	{
		m_baseParams = ChainParams(contentsString(m_dbPath + "/genesis.json"));
		network = ele::Network::Special;
	}
	if (network != ele::Network::Special)
		m_baseParams = ChainParams(genesisInfo(network), genesisStateRoot(network));

	// Open Key Store
	if (keyManager().exists())
		openKeyManager();
	else
		createKeyManager();

	if (_onInit >= OpenOnly)
		open(_onInit);
}

bool Alele::open(OnInit _connect)
{
	if (!m_webThree)
	{
		QSettings s("elementrem", "alelezero");
		auto configBytes = s.value("peers").toByteArray();
		bytesConstRef network((byte*)configBytes.data(), configBytes.size());

		try
		{
			m_webThree.reset(new WebThreeDirect(WebThreeDirect::composeClientVersion(m_clientVersion), m_dbPath, m_baseParams, WithExisting::Trust, {"ele", "bzz", "shh"}, p2p::NetworkPreferences(), network));
		}
		catch (DatabaseAlreadyOpen&)
		{
			return false;
		}

		web3()->setClientVersion(WebThreeDirect::composeClientVersion(m_clientVersion));
		if (keyManager().accounts().size() > 0)
			setAuthor(keyManager().accounts().front());
		elementrem()->setDefault(LatestBlock);

		{
			QTimer* t = new QTimer(this);
			connect(t, SIGNAL(timeout()), SLOT(checkHandlers()));
			t->start(200);
		}

		if (_connect >= Bootstrap)
		{
			web3()->startNetwork();
			for (auto const& i: Host::pocHosts())
				web3()->requirePeer(i.first, i.second);
		}
	}
	return true;
}

void Alele::close()
{
	if (!isOpen())
		return;
	QSettings s("elementrem", "alelezero");
	bytes d = web3()->saveNetwork();
	s.setValue("peers", QByteArray((char*)d.data(), (int)d.size()));

	delete findChild<QTimer*>();
	m_webThree.reset(nullptr);
}

void Alele::reopenChain(ChainParams const& _p)
{
	elementrem()->reopenChain(_p);
	m_isTampered = true;
}

void Alele::reopenChain()
{
	elementrem()->reopenChain(m_baseParams);
	m_isTampered = false;
}

unsigned Alele::installWatch(LogFilter const& _tf, WatchHandler const& _f)
{
	if (!isOpen())
		return (unsigned)-1;
	auto ret = elementrem()->installWatch(_tf, Reaping::Manual);
	m_handlers[ret] = _f;
	_f(LocalisedLogEntries());
	return ret;
}

unsigned Alele::installWatch(h256 const& _tf, WatchHandler const& _f)
{
	if (!isOpen())
		return (unsigned)-1;
	auto ret = elementrem()->installWatch(_tf, Reaping::Manual);
	m_handlers[ret] = _f;
	_f(LocalisedLogEntries());
	return ret;
}

void Alele::uninstallWatch(unsigned _w)
{
	if (!isOpen())
		return;
	cdebug << "!!! Main: uninstalling watch" << _w;
	elementrem()->uninstallWatch(_w);
	m_handlers.erase(_w);
}

void Alele::checkHandlers()
{
	if (!isOpen())
		return;
	for (auto const& i: m_handlers)
	{
		auto ls = elementrem()->checkWatchSafe(i.first);
		if (ls.size())
		{
//				cnote << "FIRING WATCH" << i.first << ls.size();
			i.second(ls);
		}
	}
}

void Alele::install(AccountNamer* _adopt)
{
	m_namers.insert(_adopt);
	_adopt->m_alele = this;
	emit knownAddressesChanged();
}

void Alele::uninstall(AccountNamer* _kill)
{
	m_namers.erase(_kill);
	if (!m_destructing)
		emit knownAddressesChanged();
}

void Alele::noteKnownAddressesChanged(AccountNamer*)
{
	emit knownAddressesChanged();
}

void Alele::noteAddressNamesChanged(AccountNamer*)
{
	emit addressNamesChanged();
}

Address Alele::toAddress(string const& _n) const
{
	for (AccountNamer* n: m_namers)
		if (n->toAddress(_n))
			return n->toAddress(_n);
	return Address();
}

string Alele::toName(Address const& _a) const
{
	for (AccountNamer* n: m_namers)
		if (!n->toName(_a).empty())
			return n->toName(_a);
	return string();
}

Addresses Alele::allKnownAddresses() const
{
	Addresses ret;
	for (AccountNamer* i: m_namers)
		ret += i->knownAddresses();
	sort(ret.begin(), ret.end());
	return ret;
}

Secret Alele::retrieveSecret(Address const& _address) const
{
	bool first = true;
	while (true)
	{
		bool ok = false;
		Secret s = m_keyManager.secret(_address, [&](){
			string r;
			tie(r, ok) = getPassword("Enter the password for the account " + m_keyManager.accountName(_address) + " (" + _address.abridged() + "):", "Unlock Account", m_keyManager.passwordHint(_address), DoNotVerify, first ? DefaultPasswordFlags : IsRetry);
			return r;
		});
		if (s)
			return s;
		if (!ok)
			return Secret();
		first = false;
	}
}

pair<string, bool> Alele::getPassword(string const& _prompt, string const& _title, string const& _hint, function<bool(string const&)> const& _verify, int _flags, string const& _failMessage) const
{
	QDialog d;
	Ui_GetPassword gp;
	gp.setupUi(&d);
	d.setWindowTitle(QString::fromStdString(_title));
	gp.label->setText(QString::fromStdString(_prompt));
	if (!_hint.empty())
		gp.entry->setPlaceholderText("Hint: " + QString::fromStdString(_hint));
	gp.confirm->setVisible(!!(_flags & NeedConfirm));

	if (_flags & NeedConfirm)
	{
		connect(gp.entry, &QLineEdit::textChanged, [&](){ gp.ok->setEnabled(gp.confirm->text() == gp.entry->text()); });
		connect(gp.confirm, &QLineEdit::textChanged, [&](){ auto m = gp.confirm->text() == gp.entry->text(); gp.ok->setEnabled(m); gp.error->setText(m ? " " : "Passphrases must match."); gp.error->update(); });
	}

	while (true)
	{
		if (_flags & IsRetry)
			gp.error->setText("The password you gave is incorrect.");
		if (d.exec() == QDialog::Accepted)
		{
			std::string ret = gp.entry->text().toStdString();
			if (_verify(ret))
				return make_pair(ret, true);
			else
				_flags |= IsRetry;
		}
		else
		{
			if (!_failMessage.empty())
				QMessageBox::information(nullptr, "Failed Password Entry", QString::fromStdString(_failMessage));
			return make_pair(string(), false);
		}
	}
}

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
/** @file AleleZero.cpp
 * 
 * 
 */

#include <fstream>
// Make sure boost/asio.hpp is included before windows.h.
#include <boost/asio.hpp>
#include "AleleZero.h"
#pragma GCC diagnostic ignored "-Wpedantic"
//pragma GCC diagnostic ignored "-Werror=pedantic"
#include <QtCore/QtCore>
#include <QtGui/QClipboard>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QListWidgetItem>
#include <libelhashseal/ElhashAux.h>
#include <libelecore/ICAP.h>
#include <libelementrem/Client.h>
#include <libelementrem/ElementremHost.h>
#include <libelhashseal/ElhashClient.h>
#include <libalele/AccountHolder.h>
#include <libalele/SyncView.h>
#include "Connect.h"
#include "alelezero/BuildInfo.h"
#include "SettingsDialog.h"
#include "NetworkSettings.h"
#include "ui_AleleZero.h"
using namespace std;
using namespace dev;
using namespace alele;
using namespace zero;
using namespace p2p;
using namespace ele;

AleleZero::AleleZero():
	m_ui(new Ui::Main),
	m_alele(this)
{
	setWindowFlags(Qt::Window);
	m_ui->setupUi(this);

//	statusBar()->addPermanentWidget(m_ui->cacheUsage);
	m_ui->cacheUsage->hide();
	statusBar()->addPermanentWidget(m_ui->balance);
	statusBar()->addPermanentWidget(m_ui->peerCount);
	statusBar()->addPermanentWidget(m_ui->mineStatus);
	statusBar()->addPermanentWidget(m_ui->syncStatus);
	statusBar()->addPermanentWidget(m_ui->chainStatus);
	statusBar()->addPermanentWidget(m_ui->blockCount);

	connect(&m_alele, SIGNAL(beneficiaryChanged()), SLOT(onBeneficiaryChanged()));
	m_alele.init(Alele::OpenOnly, "AleleZero", "anon");
	m_rpcHost.init(&m_alele);

	cerr << "ele Network protocol version: " << ele::c_protocolVersion << endl;
	cerr << "Client database version: " << c_databaseVersion << endl;

	m_ui->blockCount->setText(QString("PV%1.%2 D%3 %4-%5 v%6").arg(ele::c_protocolVersion).arg(ele::c_minorProtocolVersion).arg(c_databaseVersion).arg(QString::fromStdString(alele()->elementrem()->sealEngine()->name())).arg(alele()->elementrem()->sealEngine()->revision()).arg(dev::Version));

	createSettingsPages();
	readSettings(true, false);
	installWatches();

	{
		QTimer* t = new QTimer(this);
		connect(t, &QTimer::timeout, [&](){ refreshMining(); });
		t->start(100);
	}
	{
		QTimer* t = new QTimer(this);
		connect(t, &QTimer::timeout, [&](){ refreshNetwork(); refreshBlockCount(); });
		t->start(1000);
	}

	for (auto const& i: *s_linkedPlugins)
	{
		cdebug << "Initialising plugin:" << i.first;
		initPlugin(i.second(this));
	}

	connect(alele(), SIGNAL(knownAddressesChanged(AccountNamer*)), SLOT(refreshAll()));
	connect(alele(), SIGNAL(addressNamesChanged(AccountNamer*)), SLOT(refreshAll()));
	connect(&m_alele, &AleleFace::keysChanged, [&](){ refreshBalances(); });

	readSettings(false, true);

	refreshAll();
}

AleleZero::~AleleZero()
{
	// save all settings here so we don't have to explicitly finalise plugins.
	// NOTE: as soon as plugin finalisation means anything more than saving settings, this will
	// need to be rethought into something more like:
	// forEach([&](shared_ptr<Plugin> const& p){ finalisePlugin(p.get()); });
	writeSettings();

	killPlugins();
}

void AleleZero::createSettingsPages()
{
	m_settingsDialog = new SettingsDialog(this);
	m_settingsDialog->addPage(0, "Network", [this](){
		NetworkSettingsPage* network = new NetworkSettingsPage();
		connect(m_settingsDialog, &SettingsDialog::displayed, [this, network]() { network->setPrefs(netPrefs()); });
		connect(m_settingsDialog, &SettingsDialog::applied, [this, network]() { setNetPrefs(network->prefs()); });
		return network;
	});
}

void AleleZero::initPlugin(Plugin* _p)
{
	QSettings s("elementrem", "alelezero");
	_p->readSettings(s);
}

void AleleZero::finalisePlugin(Plugin* _p)
{
	QSettings s("elementrem", "alelezero");
	_p->writeSettings(s);
}

void AleleZero::unloadPlugin(string const& _name)
{
	shared_ptr<Plugin> p = takePlugin(_name);
	if (p)
		finalisePlugin(p.get());
}

void AleleZero::allStop()
{
	writeSettings();
	alele()->elementrem()->stopSealing();
	m_ui->net->setChecked(false);
	alele()->web3()->stopNetwork();
}

void AleleZero::carryOn()
{
	readSettings(true);
	installWatches();
	refreshAll();
}

void AleleZero::installWatches()
{
	auto newBlockId = alele()->installWatch(ChainChangedFilter, [=](LocalisedLogEntries const&){
		cwatch << "Blockchain changed!";

		// update blockchain dependent views.
		refreshBlockCount();

		// We must update balances since we can't filter updates to basic accounts.
		refreshBalances();
	});

	cdebug << "newBlock watch ID: " << newBlockId;
}

QString AleleZero::contents(QString _s)
{
	return QString::fromStdString(dev::asString(dev::contents(_s.toStdString())));
}

void AleleZero::note(QString _s)
{
	cnote << _s.toStdString();
}

void AleleZero::debug(QString _s)
{
	cdebug << _s.toStdString();
}

void AleleZero::warn(QString _s)
{
	cwarn << _s.toStdString();
}

void AleleZero::on_about_triggered()
{
	QString versionString = QString::fromStdString(WebThreeDirect::composeClientVersion("AleleZero"));

	versionString += "\n";
	versionString += "\nBy cpp-elementrem contributors, 2016.";
	versionString += "\nSee the README for contributors and credits.";

	QMessageBox::about(this, "About " + windowTitle(), versionString);
}

void AleleZero::writeSettings()
{
	QSettings s("elementrem", "alelezero");
	s.remove("address");

	forEach([&](std::shared_ptr<Plugin> p)
	{
		p->writeSettings(s);
	});

	s.setValue("geometry", saveGeometry());
	s.setValue("windowState", saveState());
}

void AleleZero::readSettings(bool _skipGeometry, bool _onlyGeometry)
{
	{
		QSettings s("elementrem", "alelezero");

		if (!_skipGeometry)
			restoreGeometry(s.value("geometry").toByteArray());
		restoreState(s.value("windowState").toByteArray());
		if (_onlyGeometry)
			return;

		forEach([&](std::shared_ptr<Plugin> p)
		{
			p->readSettings(s);
		});
	}
	NetworkSettings netSettings = netPrefs();
	if (netSettings.clientName.isEmpty())
		netSettings.clientName = QInputDialog::getText(nullptr, "Enter identity", "Enter a name that will identify you on the peer network");
	setNetPrefs(netSettings);
}

void AleleZero::setNetPrefs(NetworkSettings const& _settings)
{
	alele()->web3()->setIdealPeerCount(_settings.idealPeers);
	auto p = _settings.p2pSettings;
	p.discovery = p.discovery && !alele()->isTampered();
	p.pin = p.pin || alele()->isTampered();
	alele()->web3()->setNetworkPreferences(p, _settings.dropPeers);
	alele()->web3()->setClientVersion(WebThreeDirect::composeClientVersion("AleleZero"));
	QSettings s("elementrem", "alelezero");
	_settings.write(s);
}

NetworkSettings AleleZero::netPrefs() const
{
	NetworkSettings settings;
	QSettings s("elementrem", "alelezero");
	settings.read(s);
	return settings;
}

void AleleZero::onKeysChanged()
{
	// TODO: reinstall balance watchers
}

void AleleZero::onBeneficiaryChanged()
{
	Address b = alele()->author();
	for (int i = 0; i < m_ui->ourAccounts->count(); ++i)
	{
		auto hba = m_ui->ourAccounts->item(i)->data(Qt::UserRole).toByteArray();
		auto h = Address((byte const*)hba.data(), Address::ConstructFromPointer);
		m_ui->ourAccounts->item(i)->setCheckState(h == b ? Qt::Checked : Qt::Unchecked);
	}
}

void AleleZero::on_confirm_triggered()
{
	m_rpcHost.accountHolder()->setEnabled(m_ui->confirm->isChecked());
}

void AleleZero::on_preview_triggered()
{
	alele()->elementrem()->setDefault(m_ui->preview->isChecked() ? PendingBlock : LatestBlock);
	refreshAll();
}

void AleleZero::refreshMining()
{
	pair<uint64_t, unsigned> gp = ElhashAux::fullGeneratingProgress();
	QString t;
	if (gp.first != ElhashAux::NotGenerating)
		t = QString("DAG for #%1-#%2: %3% complete; ").arg(gp.first).arg(gp.first + ELHASH_EPOCH_LENGTH - 1).arg(gp.second);
	try
	{
		ElhashClient* c = asElhashClient(alele()->elementrem());
		WorkingProgress p = c->miningProgress();
		m_ui->mineStatus->setText(t + (c->isMining() ? p.hashes > 0 ? QString("%1s @ %2kH/s").arg(p.ms / 1000).arg(p.ms ? p.hashes / p.ms : 0) : "Awaiting DAG" : "Not mining"));
	}
	catch (...)
	{
		m_ui->mineStatus->setText(t + (alele()->elementrem()->wouldSeal() ? "Sealing..." : "Not sealing"));
	}
}

void AleleZero::refreshBalances()
{
	cwatch << "refreshBalances()";
	// update all the balance-dependent stuff.
	m_ui->ourAccounts->clear();
	u256 totalBalance = 0;
/*	map<Address, tuple<QString, u256, u256>> altCoins;
	Address coinsAddr = getCurrencies();
	for (unsigned i = 0; i < elementrem()->stateAt(coinsAddr, PendingBlock); ++i)
	{
		auto n = elementrem()->stateAt(coinsAddr, i + 1);
		auto addr = right160(elementrem()->stateAt(coinsAddr, n));
		auto denom = elementrem()->stateAt(coinsAddr, sha3(h256(n).asBytes()));
		if (denom == 0)
			denom = 1;
//		cdebug << n << addr << denom << sha3(h256(n).asBytes());
		altCoins[addr] = make_tuple(fromRaw(n), 0, denom);
	}*/

	auto bene = alele()->author();
	for (auto const& address: alele()->keyManager().accounts())
	{
		u256 b = alele()->elementrem()->balanceAt(address);
		QListWidgetItem* li = new QListWidgetItem(
			QString("<%1> %2: %3 [%4]")
				.arg(alele()->keyManager().haveKey(address) ? "KEY" : "BRAIN")
				.arg(QString::fromStdString(alele()->toReadable(address)))
				.arg(formatBalance(b).c_str())
				.arg((unsigned)alele()->elementrem()->countAt(address))
			, m_ui->ourAccounts);
		li->setData(Qt::UserRole, QByteArray((char const*)address.data(), Address::size));
		li->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		li->setCheckState(bene == address ? Qt::Checked : Qt::Unchecked);
		totalBalance += b;

//		for (auto& c: altCoins)
//			get<1>(c.second) += (u256)elementrem()->stateAt(c.first, (u160)i.address());
	}

	QString b;
/*	for (auto const& c: altCoins)
		if (get<1>(c.second))
		{
			stringstream s;
			s << setw(toString(get<2>(c.second) - 1).size()) << setfill('0') << (get<1>(c.second) % get<2>(c.second));
			b += QString::fromStdString(toString(get<1>(c.second) / get<2>(c.second)) + "." + s.str() + " ") + get<0>(c.second).toUpper() + " | ";
		}*/
	m_ui->balance->setText(b + QString::fromStdString(formatBalance(totalBalance)));
}

void AleleZero::refreshNetwork()
{
	auto ps = alele()->web3()->peers();

	m_ui->peerCount->setText(QString::fromStdString(toString(ps.size())) + " peer(s)");
	m_ui->peers->clear();
	m_ui->nodes->clear();

	if (alele()->web3()->haveNetwork())
	{
		map<h512, QString> sessions;
		for (PeerSessionInfo const& i: ps)
			m_ui->peers->addItem(QString("[%8 %7] %3 ms - %1:%2 - %4 %5 %6")
				.arg(QString::fromStdString(i.host))
				.arg(i.port)
				.arg(chrono::duration_cast<chrono::milliseconds>(i.lastPing).count())
				.arg(sessions[i.id] = QString::fromStdString(i.clientVersion))
				.arg(QString::fromStdString(toString(i.caps)))
				.arg(QString::fromStdString(toString(i.notes)))
				.arg(i.socketId)
				.arg(QString::fromStdString(i.id.abridged())));

		auto ns = alele()->web3()->nodes();
		for (p2p::Peer const& i: ns)
			m_ui->nodes->insertItem(sessions.count(i.id) ? 0 : m_ui->nodes->count(), QString("[%1 %3] %2 - ( %4 ) - *%5")
				.arg(QString::fromStdString(i.id.abridged()))
				.arg(QString::fromStdString(i.endpoint.address.to_string()))
				.arg(i.id == alele()->web3()->id() ? "self" : sessions.count(i.id) ? sessions[i.id] : "disconnected")
				.arg(i.isOffline() ? " | " + QString::fromStdString(reasonOf(i.lastDisconnect())) + " | " + QString::number(i.failedAttempts()) + "x" : "")
				.arg(i.rating())
				);
	}
}

void AleleZero::refreshBlockCount()
{
	SyncStatus sync = alele()->elementrem()->syncStatus();
	QString syncStatus = QString("PV%1 %2").arg(sync.protocolVersion).arg(ElementremHost::stateName(sync.state));
	if (sync.state == SyncState::Blocks || sync.state == SyncState::NewBlocks)
		syncStatus += QString(": %1/%2").arg(sync.startBlockNumber + sync.blocksReceived).arg(sync.highestBlockNumber);
	m_ui->syncStatus->setText(syncStatus);
	// TODO: allow a plugin to display this.
//	BlockQueueStatus b = elementrem()->blockQueueStatus();
//	m_ui->chainStatus->setText(QString("%3 importing %4 ready %5 verifying %6 unverified %7 future %8 unknown %9 bad  %1 #%2")
//		.arg(m_privateChain.size() ? "[" + m_privateChain + "] " : c_network == ele::Network::Olympic ? "Olympic" : "Frontier").arg(d.number).arg(b.importing).arg(b.verified).arg(b.verifying).arg(b.unverified).arg(b.future).arg(b.unknown).arg(b.bad));
	// TODO: allow chain description here, probably from a plugin.
	auto const& blockInfo = alele()->elementrem()->blockChain().info();
	char timestamp[64];
	time_t rawTime = (time_t)(uint64_t)blockInfo.timestamp();
	strftime(timestamp, 64, "%c", localtime(&rawTime));
	m_ui->chainStatus->setText(
		QString("#%1 (%2)")
		.arg(unsigned(blockInfo.number()))
		.arg(QString(timestamp))
	);
}

void AleleZero::refreshAll()
{
	refreshBlockCount();
	refreshBalances();
	noteAllChange();
}

void AleleZero::on_refresh_triggered()
{
	refreshAll();
}
/*
static std::string niceUsed(unsigned _n)
{
	static const vector<std::string> c_units = { "bytes", "KB", "MB", "GB", "TB", "PB" };
	unsigned u = 0;
	while (_n > 10240)
	{
		_n /= 1024;
		u++;
	}
	if (_n > 1000)
		return toString(_n / 1000) + "." + toString((min<unsigned>(949, _n % 1000) + 50) / 100) + " " + c_units[u + 1];
	else
		return toString(_n) + " " + c_units[u];
}

void Main::refreshCache()
{
	BlockChain::Statistics s = elementrem()->blockChain().usage();
	QString t;
	auto f = [&](unsigned n, QString l)
	{
		t += ("%1 " + l).arg(QString::fromStdString(niceUsed(n)));
	};
	f(s.memTotal(), "total");
	t += " (";
	f(s.memBlocks, "blocks");
	t += ", ";
	f(s.memReceipts, "receipts");
	t += ", ";
	f(s.memLogBlooms, "blooms");
	t += ", ";
	f(s.memBlockHashes + s.memTransactionAddresses, "hashes");
	t += ", ";
	f(s.memDetails, "family");
	t += ")";
	m_ui->cacheUsage->setText(t);
}
*/


//////////////////////// NETWORK MANAGEMENT /////////////////////////////////

void AleleZero::on_go_triggered()
{
	if (!m_ui->net->isChecked())
	{
		m_ui->net->setChecked(true);
		on_net_triggered();
	}
	for (auto const& i: Host::pocHosts())
		alele()->web3()->requirePeer(i.first, i.second);
}

void AleleZero::on_net_triggered()
{
	if (m_ui->net->isChecked())
	{
		alele()->web3()->startNetwork();
		m_ui->downloadView->setAlele(alele());
		m_ui->enode->setText(QString::fromStdString(alele()->web3()->enode()));
	}
	else
	{
		m_ui->downloadView->setAlele(nullptr);
		writeSettings();
		alele()->web3()->stopNetwork();
	}
}

void AleleZero::on_connect_triggered()
{
	if (!m_ui->net->isChecked())
	{
		m_ui->net->setChecked(true);
		on_net_triggered();
	}

	Connect m_connect(this);
	if (m_connect.exec() == QDialog::Accepted)
	{
		m_connect.reset();
		try
		{
			alele()->web3()->addPeer(NodeSpec(m_connect.host().toStdString()), m_connect.required() ? PeerType::Required : PeerType::Optional);
		}
		catch (...)
		{
			QMessageBox::warning(this, "Connect to Node", "Couldn't interpret that address. Ensure you've typed it in properly and that it's in a reasonable format.", QMessageBox::Ok);
		}
	}
}


//////////////////////// ACCOUNT MANAGEMENT /////////////////////////////////


void AleleZero::on_ourAccounts_doubleClicked()
{
	auto hba = m_ui->ourAccounts->currentItem()->data(Qt::UserRole).toByteArray();
	auto h = Address((byte const*)hba.data(), Address::ConstructFromPointer);
	qApp->clipboard()->setText(QString::fromStdString(ICAP(h).encoded()) + " (" + QString::fromStdString(h.hex()) + ")");
}

void AleleZero::on_ourAccounts_itemClicked(QListWidgetItem* _i)
{
	auto hba = _i->data(Qt::UserRole).toByteArray();
	alele()->setAuthor(Address((byte const*)hba.data(), Address::ConstructFromPointer));
}

void AleleZero::on_exportKey_triggered()
{
	if (m_ui->ourAccounts->currentRow() >= 0)
	{
		auto hba = m_ui->ourAccounts->currentItem()->data(Qt::UserRole).toByteArray();
		Address h((byte const*)hba.data(), Address::ConstructFromPointer);
		Secret s = alele()->retrieveSecret(h);
		QMessageBox::information(this, "Export Account Key", "Secret key to account " + QString::fromStdString(alele()->toReadable(h) + " is:\n" + s.makeInsecure().hex()));
	}
}

void AleleZero::on_killAccount_triggered()
{
	if (m_ui->ourAccounts->currentRow() >= 0)
	{
		auto hba = m_ui->ourAccounts->currentItem()->data(Qt::UserRole).toByteArray();
		Address h((byte const*)hba.data(), Address::ConstructFromPointer);
		QString s = QInputDialog::getText(this, QString::fromStdString("Kill Account " + alele()->keyManager().accountName(h) + "?!"),
			QString::fromStdString("Account " + alele()->keyManager().accountName(h) + " (" + alele()->toReadable(h) + ") has " + formatBalance(alele()->elementrem()->balanceAt(h)) + " in it.\r\nIt, and any contract that this account can access, will be lost forever if you continue. Do NOT continue unless you know what you are doing.\n"
			"Are you sure you want to continue? \r\n If so, type 'YES' to confirm."),
			QLineEdit::Normal, "NO");
		if (s != "YES")
			return;
		alele()->keyManager().kill(h);
		if (alele()->keyManager().accounts().empty())
			alele()->keyManager().import(Secret::random(), "Default account");
		alele()->noteKeysChanged();
		if (alele()->author() == h)
			alele()->setAuthor(alele()->keyManager().accounts().front());
	}
}

void AleleZero::on_reencryptKey_triggered()
{
	if (m_ui->ourAccounts->currentRow() >= 0)
	{
		auto hba = m_ui->ourAccounts->currentItem()->data(Qt::UserRole).toByteArray();
		Address a((byte const*)hba.data(), Address::ConstructFromPointer);
		QStringList kdfs = {"PBKDF2-SHA256", "Scrypt"};
		bool ok = true;
		KDF kdf = (KDF)kdfs.indexOf(QInputDialog::getItem(this, "Re-Encrypt Key", "Select a key derivation function to use for storing your key:", kdfs, kdfs.size() - 1, false, &ok));
		if (!ok)
			return;
		std::string hint;
		std::string password = getPassword("Create Account", "Enter the password you would like to use for this key. Don't forget it!\nEnter nothing to use your Master password.", &hint, &ok);
		if (!ok)
			return;
		try {
			auto pw = [&](){
				auto p = QInputDialog::getText(this, "Re-Encrypt Key", "Enter the original password for this key.\nHint: " + QString::fromStdString(alele()->keyManager().passwordHint(a)), QLineEdit::Password, QString()).toStdString();
				if (p.empty())
					throw PasswordUnknown();
				return p;
			};
			while (!(password.empty() ? alele()->keyManager().recode(a, SemanticPassword::Master, pw, kdf) : alele()->keyManager().recode(a, password, hint, pw, kdf)))
				if (QMessageBox::question(this, "Re-Encrypt Key", "Password given is incorrect. Would you like to try again?", QMessageBox::Retry, QMessageBox::Cancel) == QMessageBox::Cancel)
					return;
		}
		catch (PasswordUnknown&) {}
	}
}

void AleleZero::on_reencryptAll_triggered()
{
	QStringList kdfs = {"PBKDF2-SHA256", "Scrypt"};
	bool ok = false;
	QString kdf = QInputDialog::getItem(this, "Re-Encrypt Key", "Select a key derivation function to use for storing your keys:", kdfs, kdfs.size() - 1, false, &ok);
	if (!ok)
		return;
	try {
		for (Address const& a: alele()->keyManager().accounts())
			while (!alele()->keyManager().recode(a, SemanticPassword::Existing, [&](){
				auto p = QInputDialog::getText(nullptr, "Re-Encrypt Key", QString("Enter the original password for key %1.\nHint: %2").arg(QString::fromStdString(alele()->toReadable(a))).arg(QString::fromStdString(alele()->keyManager().passwordHint(a))), QLineEdit::Password, QString()).toStdString();
				if (p.empty())
					throw PasswordUnknown();
				return p;
			}, (KDF)kdfs.indexOf(kdf)))
				if (QMessageBox::question(this, "Re-Encrypt Key", "Password given is incorrect. Would you like to try again?", QMessageBox::Retry, QMessageBox::Cancel) == QMessageBox::Cancel)
					return;
	}
	catch (PasswordUnknown&) {}
}

std::string AleleZero::getPassword(std::string const& _title, std::string const& _for, std::string* _hint, bool* _ok)
{
	QString password;
	while (true)
	{
		bool ok;
		password = QInputDialog::getText(nullptr, QString::fromStdString(_title), QString::fromStdString(_for), QLineEdit::Password, QString(), &ok);
		if (!ok)
		{
			if (_ok)
				*_ok = false;
			return string();
		}
		if (password.isEmpty())
			break;
		QString confirm = QInputDialog::getText(nullptr, QString::fromStdString(_title), "Confirm this password by typing it again", QLineEdit::Password, QString());
		if (password == confirm)
			break;
		QMessageBox::warning(nullptr, QString::fromStdString(_title), "You entered two different passwords - please enter the same password twice.", QMessageBox::Ok);
	}

	if (!password.isEmpty() && _hint && !alele()->keyManager().haveHint(password.toStdString()))
		*_hint = QInputDialog::getText(this, "Create Account", "Enter a hint to help you remember this password.").toStdString();

	if (_ok)
		*_ok = true;
	return password.toStdString();
}

void AleleZero::addSettingsPage(int _index, QString const& _categoryName, std::function<SettingsPage*()> const& _pageFactory)
{
	m_settingsDialog->addPage(_index, _categoryName, _pageFactory);
}

void AleleZero::on_settings_triggered()
{
	if (m_settingsDialog->exec() == QDialog::Accepted)
		writeSettings();
}

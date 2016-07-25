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
/** @file Slave.h
 * 
 * 
 */

#pragma once

#include <QThread>
#include <QStringList>
#include <libdevcore/Common.h>
#include <libelecore/Common.h>
#include <libelhashseal/ElhashAux.h>

Q_DECLARE_METATYPE(dev::h256)
Q_DECLARE_METATYPE(dev::ele::ElhashProofOfWork::Solution)
Q_DECLARE_METATYPE(dev::ele::ElhashProofOfWork::WorkPackage)

class QTimer;

namespace jsonrpc { class HttpClient; }

namespace dev
{

namespace ele { class Elhash; }

namespace alele
{

class SlaveAux: public QObject
{
	Q_OBJECT

public:
	SlaveAux();
	~SlaveAux();

	void start();
	void stop();

	bool precompute() const { return m_precompute; }
	void setPrecompute(bool _v) { m_precompute = _v; }

	QString url() const { return m_url; }
	void setURL(QString _url);

	QString sealer() const;
	void setSealer(QString _id);

	QStringList sealers() const;

	bool isWorking() const;
	u256 hashrate() const { return m_hashrate; }

signals:
	void generatingDAG(unsigned _pc);
	void solutionFound(ele::ElhashProofOfWork::Solution _sol);
	void dagGenerationFailed();

private:
	void onNewWork(ele::ElhashProofOfWork::WorkPackage const& _work);
	void onGeneratingDAG(unsigned _pc);
	bool onSolutionFound(ele::ElhashProofOfWork::Solution const& _sol);

	void timerEvent(QTimerEvent*) override;

	bool reportSolution(ele::ElhashProofOfWork::Solution const& _sol);

	std::shared_ptr<ele::Elhash> m_engine;
	std::shared_ptr<jsonrpc::HttpClient> m_client;

	h256 m_id;
	QString m_url;
	QTimer* m_work;
	int m_rateTimer;
	int m_workTimer;
	u256 m_hashrate;

	ele::ElhashProofOfWork::WorkPackage m_current;
	ele::ElhashProofOfWork::Solution m_solution;
	ele::ElhashAux::FullType m_dag;
	bool m_precompute = false;
};

class Slave: public QObject
{
	Q_OBJECT

public:
	Slave();
	~Slave();

	void start() { m_aux->start(); }
	void stop() { m_aux->stop(); }

	bool isWorking() const { return m_aux->isWorking(); }
	u256 hashrate() const { return m_aux->hashrate(); }

	QString url() const { return m_aux->url(); }
	void setURL(QString _url) { m_aux->setURL(_url); }

	QString sealer() const { return m_aux->sealer(); }
	void setSealer(QString _id) { m_aux->setSealer(_id); }
	QStringList sealers() const { return m_aux->sealers(); }

signals:
	void generatingDAG(unsigned _pc);
	void solutionFound(ele::ElhashProofOfWork::Solution _sol);
	void dagGenerationFailed();

private:
	bool m_precompute = false;

	SlaveAux* m_aux;
	QThread m_worker;
};

}
}

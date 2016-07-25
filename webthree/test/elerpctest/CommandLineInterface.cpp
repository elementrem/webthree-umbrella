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
/** @file CommandLineInterface.cpp
 * @
 * 
 */

#include <string>
#include <iostream>
#include <fstream>
#include <csignal>
#include <thread>
#include <boost/filesystem.hpp>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <libtestutils/Common.h>
#include <libtestutils/BlockChainLoader.h>
#include <libtestutils/FixedClient.h>
#include <libweb3jsonrpc/ModularServer.h>
#include <libweb3jsonrpc/AccountHolder.h>
#include <libweb3jsonrpc/Ele.h>
//#include <libweb3testutils/FixedWebThreeServer.h>
#include "CommandLineInterface.h"

using namespace std;
using namespace dev;
using namespace dev::ele;
using namespace dev::test;
namespace po = boost::program_options;

bool CommandLineInterface::parseArguments(int argc, char** argv)
{
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Show help message and exit")
		("json", po::value<vector<string>>()->required(), "input file")
		("test", po::value<vector<string>>()->required(), "test case name");

	// All positional options should be interpreted as input files
	po::positional_options_description p;

	// parse the compiler arguments
	try
	{
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).allow_unregistered().run(), m_args);

		if (m_args.count("help"))
		{
			cout << desc;
			return false;
		}

		po::notify(m_args);
	}
	catch (po::error const& _exception)
	{
		cout << _exception.what() << endl;
		return false;
	}

	return true;
}

bool CommandLineInterface::processInput()
{
	string infile = m_args["json"].as<vector<string>>()[0];

	auto path = boost::filesystem::path(infile);
	if (!boost::filesystem::exists(path))
	{
		cout << "Non existant input file \"" << infile << "\"" << endl;
		return false;
	}

	string test = m_args["test"].as<vector<string>>()[0];
	Json::Value j = dev::test::loadJsonFromFile(path.string());

	if (j[test].empty())
	{
		cout << "Non existant test case \"" << infile << "\"" << endl;
		return false;
	}
	
	if (!j[test].isObject())
	{
		cout << "Incorrect JSON file \"" << infile << "\"" << endl;
		return false;
	}
	
	m_json = j[test];

	return true;
}

bool g_exit = false;

void sighandler(int)
{
	g_exit = true;
}

void CommandLineInterface::actOnInput()
{
	BlockChainLoader bcl(m_json);
	cerr << "void CommandLineInterface::actOnInput() FixedClient now accepts ele::Block!!! (cannot pass bcl.state() any more)" << endl;
	FixedClient client(bcl.bc(), ele::Block(Block::Null));
	FixedAccountHolder accountHolder([&](){return &client;}, {});
	ModularServer<rpc::EleFace> server(new rpc::Ele(client, accountHolder));
	server.addConnector(new jsonrpc::HttpServer(8080, "", "", 2));
	server.StartListening();
	
	signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);

	while (!g_exit)
		this_thread::sleep_for(chrono::milliseconds(1000));
}

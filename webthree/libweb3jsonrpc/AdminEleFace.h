/**
 * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!
 */

#ifndef JSONRPC_CPP_STUB_DEV_RPC_ADMINELEFACE_H_
#define JSONRPC_CPP_STUB_DEV_RPC_ADMINELEFACE_H_

#include "ModularServer.h"

namespace dev {
    namespace rpc {
        class AdminEleFace : public ServerInterface<AdminEleFace>
        {
            public:
                AdminEleFace()
                {
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_blockQueueStatus", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_OBJECT, "param1",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_blockQueueStatusI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_setAskPrice", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_STRING,"param2",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_setAskPriceI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_setBidPrice", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_STRING,"param2",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_setBidPriceI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_setMining", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_BOOLEAN,"param2",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_setMiningI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_findBlock", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_OBJECT, "param1",jsonrpc::JSON_STRING,"param2",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_findBlockI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_blockQueueFirstUnknown", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_STRING, "param1",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_blockQueueFirstUnknownI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_blockQueueRetryUnknown", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_blockQueueRetryUnknownI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_allAccounts", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_ARRAY, "param1",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_allAccountsI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_newAccount", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_OBJECT, "param1",jsonrpc::JSON_OBJECT,"param2",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_newAccountI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_setMiningBenefactor", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_STRING,"param2",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_setMiningBenefactorI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_inspect", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_OBJECT, "param1",jsonrpc::JSON_STRING,"param2",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_inspectI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_reprocess", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_OBJECT, "param1",jsonrpc::JSON_STRING,"param2",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_reprocessI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_vmTrace", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_OBJECT, "param1",jsonrpc::JSON_STRING,"param2",jsonrpc::JSON_INTEGER,"param3",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_vmTraceI);
                    this->bindAndAddMethod(jsonrpc::Procedure("admin_ele_getReceiptByHashAndIndex", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_OBJECT, "param1",jsonrpc::JSON_STRING,"param2",jsonrpc::JSON_INTEGER,"param3",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::admin_ele_getReceiptByHashAndIndexI);
                    this->bindAndAddMethod(jsonrpc::Procedure("miner_start", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_INTEGER, NULL), &dev::rpc::AdminEleFace::miner_startI);
                    this->bindAndAddMethod(jsonrpc::Procedure("miner_stop", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN,  NULL), &dev::rpc::AdminEleFace::miner_stopI);
                    this->bindAndAddMethod(jsonrpc::Procedure("miner_setElementbase", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::miner_setElementbaseI);
                    this->bindAndAddMethod(jsonrpc::Procedure("miner_setExtra", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::miner_setExtraI);
                    this->bindAndAddMethod(jsonrpc::Procedure("miner_setGasPrice", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_BOOLEAN, "param1",jsonrpc::JSON_STRING, NULL), &dev::rpc::AdminEleFace::miner_setGasPriceI);
                    this->bindAndAddMethod(jsonrpc::Procedure("miner_hashrate", jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_STRING,  NULL), &dev::rpc::AdminEleFace::miner_hashrateI);
                }

                inline virtual void admin_ele_blockQueueStatusI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_blockQueueStatus(request[0u].asString());
                }
                inline virtual void admin_ele_setAskPriceI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_setAskPrice(request[0u].asString(), request[1u].asString());
                }
                inline virtual void admin_ele_setBidPriceI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_setBidPrice(request[0u].asString(), request[1u].asString());
                }
                inline virtual void admin_ele_setMiningI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_setMining(request[0u].asBool(), request[1u].asString());
                }
                inline virtual void admin_ele_findBlockI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_findBlock(request[0u].asString(), request[1u].asString());
                }
                inline virtual void admin_ele_blockQueueFirstUnknownI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_blockQueueFirstUnknown(request[0u].asString());
                }
                inline virtual void admin_ele_blockQueueRetryUnknownI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_blockQueueRetryUnknown(request[0u].asString());
                }
                inline virtual void admin_ele_allAccountsI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_allAccounts(request[0u].asString());
                }
                inline virtual void admin_ele_newAccountI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_newAccount(request[0u], request[1u].asString());
                }
                inline virtual void admin_ele_setMiningBenefactorI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_setMiningBenefactor(request[0u].asString(), request[1u].asString());
                }
                inline virtual void admin_ele_inspectI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_inspect(request[0u].asString(), request[1u].asString());
                }
                inline virtual void admin_ele_reprocessI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_reprocess(request[0u].asString(), request[1u].asString());
                }
                inline virtual void admin_ele_vmTraceI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_vmTrace(request[0u].asString(), request[1u].asInt(), request[2u].asString());
                }
                inline virtual void admin_ele_getReceiptByHashAndIndexI(const Json::Value &request, Json::Value &response)
                {
                    response = this->admin_ele_getReceiptByHashAndIndex(request[0u].asString(), request[1u].asInt(), request[2u].asString());
                }
                inline virtual void miner_startI(const Json::Value &request, Json::Value &response)
                {
                    response = this->miner_start(request[0u].asInt());
                }
                inline virtual void miner_stopI(const Json::Value &request, Json::Value &response)
                {
                    (void)request;
                    response = this->miner_stop();
                }
                inline virtual void miner_setElementbaseI(const Json::Value &request, Json::Value &response)
                {
                    response = this->miner_setElementbase(request[0u].asString());
                }
                inline virtual void miner_setExtraI(const Json::Value &request, Json::Value &response)
                {
                    response = this->miner_setExtra(request[0u].asString());
                }
                inline virtual void miner_setGasPriceI(const Json::Value &request, Json::Value &response)
                {
                    response = this->miner_setGasPrice(request[0u].asString());
                }
                inline virtual void miner_hashrateI(const Json::Value &request, Json::Value &response)
                {
                    (void)request;
                    response = this->miner_hashrate();
                }
                virtual Json::Value admin_ele_blockQueueStatus(const std::string& param1) = 0;
                virtual bool admin_ele_setAskPrice(const std::string& param1, const std::string& param2) = 0;
                virtual bool admin_ele_setBidPrice(const std::string& param1, const std::string& param2) = 0;
                virtual bool admin_ele_setMining(bool param1, const std::string& param2) = 0;
                virtual Json::Value admin_ele_findBlock(const std::string& param1, const std::string& param2) = 0;
                virtual std::string admin_ele_blockQueueFirstUnknown(const std::string& param1) = 0;
                virtual bool admin_ele_blockQueueRetryUnknown(const std::string& param1) = 0;
                virtual Json::Value admin_ele_allAccounts(const std::string& param1) = 0;
                virtual Json::Value admin_ele_newAccount(const Json::Value& param1, const std::string& param2) = 0;
                virtual bool admin_ele_setMiningBenefactor(const std::string& param1, const std::string& param2) = 0;
                virtual Json::Value admin_ele_inspect(const std::string& param1, const std::string& param2) = 0;
                virtual Json::Value admin_ele_reprocess(const std::string& param1, const std::string& param2) = 0;
                virtual Json::Value admin_ele_vmTrace(const std::string& param1, int param2, const std::string& param3) = 0;
                virtual Json::Value admin_ele_getReceiptByHashAndIndex(const std::string& param1, int param2, const std::string& param3) = 0;
                virtual bool miner_start(int param1) = 0;
                virtual bool miner_stop() = 0;
                virtual bool miner_setElementbase(const std::string& param1) = 0;
                virtual bool miner_setExtra(const std::string& param1) = 0;
                virtual bool miner_setGasPrice(const std::string& param1) = 0;
                virtual std::string miner_hashrate() = 0;
        };

    }
}
#endif //JSONRPC_CPP_STUB_DEV_RPC_ADMINELEFACE_H_

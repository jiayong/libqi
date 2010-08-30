/*
** Author(s):
**  - Chris Kilner <ckilner@aldebaran-robotics.com>
**
** Copyright (C) 2010 Aldebaran Robotics
*/

#include <alcommon-ng/common/client_node.hpp>
#include <alcommon-ng/messaging/client.hpp>

// would not be needed if we had a specific client
#include <alcommon-ng/messaging/call_definition.hpp>
#include <alcommon-ng/messaging/result_definition.hpp>

namespace AL {
  using namespace Messaging;
  namespace Common {

    ClientNode::ClientNode(
      const std::string& clientName,
      const std::string& masterAddress) {
      fName = clientName;
      fMasterAddress = masterAddress;
      xInit();
    }

    ClientNode::~ClientNode() {}

    void ClientNode::xInit() {
      NodeInfo master("master", fMasterAddress);
      xCreateServerClient(master);
      xUpdateServicesFromMaster();
    }

    ResultDefinition ClientNode::call(const CallDefinition& callDef) {
        
        // todo lookup in services

        // todo make a hash from the calldef
        std::string hash = callDef.moduleName();
        
        // get the relevant messaging client for the node that host the servivce
        NameLookup<boost::shared_ptr<DefaultClient> >::iterator it;
        it = fServerClients.find(hash);
        if (it == fServerClients.end()) {
           // create messaging client if needed ???
          std::cout << "Client: " << fName << ", could not find Server for message " << hash << std::endl;
          // throw?
          ResultDefinition r;
          return r;
        }

        // call
        std::cout << "Client: " << fName << ", found server for message " << hash << "." << callDef.methodName() <<  std::endl;
        ResultDefinition result = (it->second)->send(callDef);
        std::cout << "  Client: " << fName << ", received result" << std::endl;
        return result;  
    }

    void ClientNode::xUpdateServicesFromMaster() {
      // if master is alive
      // get list of services
    }

    void ClientNode::xCreateServerClient(const NodeInfo& serverNodeInfo) {
      // TODO error handling
      boost::shared_ptr<DefaultClient> client = 
        boost::shared_ptr<DefaultClient>(new DefaultClient(serverNodeInfo.address));

      fServerClients.insert(make_pair(serverNodeInfo.name, client));
      fServerList.insert(make_pair(serverNodeInfo.name, serverNodeInfo)); // why not!
    }

    const ServiceInfo& ClientNode::xGetService(const std::string& methodHash) {
      // lookup service in cache
      NameLookup<ServiceInfo>::const_iterator it = fServiceCache.find(methodHash);
      if (it != fServiceCache.end()) {
        return it->second;
      }

      if (fName != "master") {
        // cache lookup failed ... time to ask master

        // add service to cache

        // return it
      }

      return fInvalidService;
    }

  }
}

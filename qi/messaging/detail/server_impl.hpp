#pragma once
/*
** Author(s):
**  - Chris Kilner <ckilner@aldebaran-robotics.com>
**
** Copyright (C) 2010 Aldebaran Robotics
*/
#ifndef   __QI_MESSAGING_DETAIL_SERVER_IMPL_HPP__
#define   __QI_MESSAGING_DETAIL_SERVER_IMPL_HPP__

#include <string>
#include <qi/transport/message_handler.hpp>
#include <qi/messaging/serviceinfo.hpp>
#include <qi/messaging/detail/mutexednamelookup.hpp>
#include <qi/transport/server.hpp>
#include <qi/messaging/detail/client_impl_base.hpp>

namespace qi {
  namespace detail {
    class PublisherImpl;

    class ServerImpl : public qi::detail::ClientImplBase, public qi::transport::MessageHandler {
    public:
      ServerImpl();
      virtual ~ServerImpl();

      ServerImpl(const std::string nodeName,
        const std::string masterAddress);

      const std::string& getName() const;

      void addService(const std::string& methodSignature, qi::Functor* functor);

      boost::shared_ptr<qi::detail::PublisherImpl> advertiseTopic(
        const std::string& topicName,
        const std::string& typeSignature);

      // MessageHandler Implementation -----------------
      void messageHandler(std::string& defData, std::string& resultData);
      // -----------------------------------------------

    protected:
      /// <summary> true if this server belongs to the master </summary>
      bool _isMasterServer;

      /// <summary> The friendly name of this server </summary>
      std::string _name;

      /// <summary> The underlying transport server </summary>
      qi::transport::Server _transportServer;

      MutexedNameLookup<ServiceInfo> _localServices;

      const ServiceInfo& xGetService(const std::string& methodHash);
      void xRegisterServiceWithMaster(const std::string& methodHash);
      bool xTopicExists(const std::string& topicName);
    };
  }
}

#endif // __QI_MESSAGING_DETAIL_SERVER_IMPL_HPP__


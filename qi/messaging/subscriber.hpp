#pragma once
/*
*  Author(s):
*  - Chris  Kilner <ckilner@aldebaran-robotics.com>
*  - Cedric Gestes <gestes@aldebaran-robotics.com>
*
*  Copyright (C) 2010, 2011 Aldebaran Robotics
*/


#ifndef _QI_MESSAGING_SUBSCRIBER_HPP_
#define _QI_MESSAGING_SUBSCRIBER_HPP_

#include <string>
#include <boost/scoped_ptr.hpp>
#include <qi/signature.hpp>
#include <qi/functors/makefunctor.hpp>

namespace qi {
  namespace detail {
    class SubscriberImpl;
  }

  class Context;

  /// <summary>
  /// Used to subscribe to named Topics.  If the Topic is unknown, the
  /// master is interrogated to find the appropriate server.
  /// </summary>
  /// \b Subscribe to a Topic
  /// \include example_qi_subscriber.cpp
  /// \ingroup Messaging
  class Subscriber {
  public:
    /// <summary>
    /// Creates a subscriber that can subscribe to names Topics.
    /// </summary>
    /// <param name="name">
    /// The name you want to give to this subscriber
    /// e.g. "subscriber"
    /// </param>
    /// <param name="context">
    /// An optional context that can be used to group or separate
    /// transport resources.
    /// </param>
    Subscriber(const std::string& name = "subscriber", Context *context = 0);

    /// <summary> Connect to masterAddress. If no address is specified
    /// the default 127.0.0.1:5555 is used </summary>
    /// <param name="masterAddress"> The master address. </param>
    void connect(const std::string &masterAddress = "127.0.0.1:5555");

    /// <summary>Finaliser. </summary>
    virtual ~Subscriber();

    /// <summary>Query if this object is initialized. </summary>
    /// <returns>true if initialized, false if not.</returns>
    bool isInitialized() const;

    /// <summary>Subscribes to a published Topic.
    ///
    /// The provided callback method will be called when the publisher
    /// publishes on the topic.
    ///
    /// e.g. subscriber.subscribe("timeOfDay", &callbackFunction);
    /// </summary>
    /// <param name="name"> The advertised name of the service.</param>
    /// <param name="function"> The memory address of the function. e.g.
    /// "&globalFunction"
    /// </param>
    /// <seealso cref="qi::Publisher"/>
    /// <seealso cref="qi::Master"/>
    template<typename FUNCTION_TYPE>
    void subscribe(const std::string& topicName, FUNCTION_TYPE callback)
    {
      xSubscribe(makeFunctionSignature(topicName, callback), makeFunctor(callback));
    }

    /// <summary>Subscribes to a published Topic.
    ///
    /// The provided callback method will be called when the publisher
    /// publishes on the topic.
    ///
    /// e.g. subscriber.subscribe("timeOfDay", this, &myCallbackMethod);
    /// </summary>
    /// <param name="object"> The memory address of the object. This could
    /// be a "this" pointer if you are adding a callback to a method of your class,
    /// or a "&myObject" for the address of a member object.</param>
    /// <param name="method"> The memory address of the method to call. This should
    /// fully qualify the type of your method such as "&MyClass::myMethod"
    /// or "&MyObject::objectMethod".</param>
    /// <seealso cref="qi::Publisher"/>
    /// <seealso cref="qi::Master"/>
    template <typename OBJECT_TYPE, typename METHOD_TYPE>
    void subscribe(const std::string& topicName, OBJECT_TYPE object, METHOD_TYPE callback)
    {
      xSubscribe(makeFunctionSignature(topicName, callback), makeFunctor(object, callback));
    }

    template<typename FUNCTION_TYPE>
    void unsubscribe(const std::string& topicName, FUNCTION_TYPE callback)
    {
      xUnsubscribe(makeFunctionSignature(topicName, callback));
    }

    template <typename OBJECT_TYPE, typename METHOD_TYPE>
    void unsubscribe(const std::string& topicName, OBJECT_TYPE object, METHOD_TYPE callback)
    {
      xUnsubscribe(makeFunctionSignature(topicName, callback));
    }

  private:
    void xSubscribe(const std::string& signature, Functor* f);
    void xUnsubscribe(const std::string& signature);
    boost::scoped_ptr<qi::detail::SubscriberImpl> _impl;
  };
}

#endif  // _QI_MESSAGING_SUBSCRIBER_HPP_

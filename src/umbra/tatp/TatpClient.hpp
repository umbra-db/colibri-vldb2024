#pragma once
//---------------------------------------------------------------------------
#include "txbench/pq/PqPreparedStatement.hpp"
#include "txbench/tatp/TatpTransactionGenerator.hpp"
#include "umbra/UmbraClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// TATP client
class TatpClient : public UmbraClient {
   /// The transaction generator
   tatp::TatpTransactionGenerator generator;
   /// The subscriber count
   unsigned subscriberCount;

   /// The get subscriber data statement
   pq::PqPreparedStatement getSubscriberData;
   /// The get new destination statement
   pq::PqPreparedStatement getNewDestination;
   /// The get access data statement
   pq::PqPreparedStatement getAccessData;
   /// The update subscriber data statement
   pq::PqPreparedStatement updateSubscriberData;
   /// The update location statement
   pq::PqPreparedStatement updateLocation;
   /// The insert call forwarding statement
   pq::PqPreparedStatement insertCallForwarding;
   /// The delete call forwarding statement
   pq::PqPreparedStatement deleteCallForwarding;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TatpClient(const Database& database, unsigned clientIndex);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

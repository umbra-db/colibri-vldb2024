#include "txbench/DatabaseClient.hpp"
#include "txbench/Exception.hpp"
#include <cassert>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
DatabaseClient::DatabaseClient(const Database& database)
   : state(IDLE), database(database)
// Constructor
{
}
//---------------------------------------------------------------------------
DatabaseClient::~DatabaseClient()
// Destructor
{
}
//---------------------------------------------------------------------------
void DatabaseClient::cancelQueries()
// Cancel the running queries
{
}
//---------------------------------------------------------------------------
void DatabaseClient::start()
// Start processing
{
   {
      std::unique_lock lock(mutex);
      assert(state.load() == IDLE);
      state.store(RUNNING);
   }

   thread = ::thread([this]() {
      try {
         performWork();
      } catch (Exception& e) {
         cerr << e.getMessage() << endl;
      }

      std::unique_lock lock(mutex);
      state.store(STOPPED);
      cond.notify_all();
   });
}
//---------------------------------------------------------------------------
void DatabaseClient::stop()
// Stop processing
{
   std::unique_lock lock(mutex);
   const auto s = state.load();
   if (s == IDLE) {
      state.store(STOPPED);
   } else if (s == RUNNING) {
      state.store(STOPPING);

      // Cancel running queries
      cancelQueries();
   }
}
//---------------------------------------------------------------------------
bool DatabaseClient::join(bool wait)
// Join the client
{
   std::unique_lock lock(mutex);
   if (state.load() == STOPPED || cond.wait_for(lock, wait ? 30min : 10s, [this] { return state.load() == STOPPED; })) {
      thread.join();
      return true;
   }

   return false;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

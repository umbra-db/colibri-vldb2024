#include "umbra/tpch/TpchRefreshClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TpchRefreshClient::TpchRefreshClient(const Database& database)
   : UmbraClient(database)
// Constructor
{
}
//---------------------------------------------------------------------------
void TpchRefreshClient::performWork()
// Perform work
{
   static atomic<unsigned> atomic_index = 0;
   while (running()) {
      // There are exactly 4000 update / delete pairs before data returns to the initial state
      auto index = atomic_index.fetch_add(1) % 4000;

      submittedTransactions.fetch_add(1, memory_order_acq_rel);

      // Insert new sales
      connection.execute("BEGIN");
      connection.execute("COPY orders FROM 'data/update/orders.tbl.u" + to_string(index + 1) + "' DELIMITER '|'");
      connection.execute("COPY lineitem FROM 'data/update/lineitem.tbl.u" + to_string(index + 1) + "' DELIMITER '|'");
      connection.execute("COMMIT");

      // Remove old sales
      connection.execute("BEGIN");
      connection.execute("DELETE FROM orders USING umbra.csvview('data/delete/delete." + to_string(index + 1) + "', 'DELIMITER ''|''', 'id INTEGER NOT NULL') WHERE o_orderkey = id;");
      connection.execute("DELETE FROM lineitem USING umbra.csvview('data/delete/delete." + to_string(index + 1) + "', 'DELIMITER ''|''', 'id INTEGER NOT NULL') WHERE l_orderkey = id;");
      connection.execute("COMMIT");

      processedTransactions.fetch_add(1, memory_order_acq_rel);
   }
}
//---------------------------------------------------------------------------
void TpchRefreshClient::initialize()
// Initialize the client
{
   UmbraClient::initialize();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

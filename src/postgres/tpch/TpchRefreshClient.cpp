#include "postgres/tpch/TpchRefreshClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TpchRefreshClient::TpchRefreshClient(const Database& database)
   : PostgresClient(database)
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
      const auto index = atomic_index.fetch_add(1) % 4000;

      submittedTransactions.fetch_add(1, memory_order_acq_rel);

      // Insert new sales
      connection.execute("BEGIN");
      connection.execute("COPY orders FROM '/data/update/orders.tbl.u" + to_string(index + 1) + "' DELIMITER '|'");
      connection.execute("COPY lineitem FROM '/data/update/lineitem.tbl.u" + to_string(index + 1) + "' DELIMITER '|'");
      connection.execute("COMMIT");

      // Remove old sales
      connection.execute("BEGIN");
      connection.execute("CREATE TEMPORARY TABLE to_delete (id INTEGER NOT NULL) ON COMMIT DROP");
      connection.execute("COPY to_delete FROM '/data/delete/delete." + to_string(index + 1) + "' DELIMITER '|'");
      connection.execute("DELETE FROM orders USING to_delete WHERE o_orderkey = id;");
      connection.execute("DELETE FROM lineitem USING to_delete WHERE l_orderkey = id;");
      connection.execute("COMMIT");

      processedTransactions.fetch_add(1, memory_order_acq_rel);
   }
}
//---------------------------------------------------------------------------
void TpchRefreshClient::initialize()
// Initialize the client
{
   PostgresClient::initialize();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

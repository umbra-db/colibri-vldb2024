#include "duckdb/tpch/TpchRefreshClient.hpp"
#include "txbench/Exception.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
TpchRefreshClient::TpchRefreshClient(const Database& database)
   : DuckDBClient(database)
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
      connection->exec("begin");
      connection->exec("copy orders from '/data/update/orders.tbl.u" + to_string(index + 1) + "' with (delimiter '|')");
      connection->exec("copy lineitem from '/data/update/lineitem.tbl.u" + to_string(index + 1) + "' with (delimiter '|')");
      connection->exec("commit");

      // Remove old sales
      connection->exec("begin");
      connection->exec("create temporary table to_delete (id integer not null)");
      connection->exec("copy to_delete from '/data/delete/delete." + to_string(index + 1) + "' with (delimiter '|')");
      connection->exec("delete from orders where o_orderkey in (select * from to_delete);");
      connection->exec("delete from lineitem where l_orderkey in (select * from to_delete);");
      connection->exec("drop table to_delete;");
      connection->exec("commit");

      processedTransactions.fetch_add(1, memory_order_acq_rel);
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

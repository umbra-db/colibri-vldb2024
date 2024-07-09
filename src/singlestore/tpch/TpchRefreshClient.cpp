#include "singlestore/tpch/TpchRefreshClient.hpp"
#include "txbench/Exception.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
TpchRefreshClient::TpchRefreshClient(const Database& database, const odbc::Environment& environment)
   : SingleStoreClient(database, environment)
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
      connection.exec("BEGIN");
      connection.exec("LOAD DATA INFILE '/data/update/orders.tbl.u" + to_string(index + 1) + "' INTO TABLE orders FIELDS TERMINATED BY '|'");
      connection.exec("LOAD DATA INFILE '/data/update/lineitem.tbl.u" + to_string(index + 1) + "' INTO TABLE lineitem FIELDS TERMINATED BY '|'");
      connection.exec("COMMIT");

      // Remove old sales
      connection.exec("BEGIN");
      connection.exec("CREATE TEMPORARY TABLE to_delete (id INTEGER NOT NULL)");
      connection.exec("LOAD DATA INFILE '/data/delete/delete." + to_string(index + 1) + "' INTO TABLE to_delete FIELDS TERMINATED BY '|'");
      connection.exec("DELETE FROM orders WHERE o_orderkey IN (SELECT * FROM to_delete) OPTION (columnstore_table_lock_threshold = 1000000000);");
      connection.exec("DELETE FROM lineitem WHERE l_orderkey IN (SELECT * FROM to_delete)  OPTION (columnstore_table_lock_threshold = 1000000000);");
      connection.exec("DROP TABLE to_delete;");
      connection.exec("COMMIT");

      processedTransactions.fetch_add(1, memory_order_acq_rel);
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

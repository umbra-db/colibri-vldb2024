#include "singlestore/tpcc/TpccClient.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Statement.hpp"
#include "txbench/tpcc/TpccConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
TpccClient::TpccClient(const Database& database, const odbc::Environment& environment, unsigned homeWarehouseId)
   : SingleStoreClient(database, environment), generator(42 + homeWarehouseId), dispatch(database.getPipelineDepth())
// Constructor
{
   const auto pipelineDepth = database.getPipelineDepth();
   auto& tpccConfig = reinterpret_cast<const tpcc::TpccConfig&>(database.getBenchmarkConfig());

   transactionNumbers.resize(pipelineDepth);
   homeWarehouseIds.resize(pipelineDepth, homeWarehouseId);
   warehouseCounts.resize(pipelineDepth);
   generateUserRollbacks.resize(pipelineDepth, tpccConfig.shouldGenerateUserRollbacks());
   generateRemoteAccesses.resize(pipelineDepth, tpccConfig.shouldGenerateRemoteAccesses());
}
//---------------------------------------------------------------------------
void TpccClient::performWork()
// Perform work
{
   const unsigned pipelineDepth = database.getPipelineDepth();

   while (running()) {
      submittedTransactions += pipelineDepth;

      for (unsigned i = 0; i < pipelineDepth; ++i)
         transactionNumbers[i] = static_cast<SQLINTEGER>(generator.pickTransactionType());

      dispatch.exec();

      processedTransactions += pipelineDepth;
   }
}
//---------------------------------------------------------------------------
void TpccClient::initialize()
// Initialize the client
{
   SingleStoreClient::initialize();

   {
      odbc::Statement statement;
      statement.allocate(connection);
      statement.exec("SELECT COUNT(*) FROM warehouse");

      SQLINTEGER warehouseCount;
      statement.bindColumn(1, warehouseCount);
      if (!statement.next())
         throw Exception("no data returned from query"sv);

      for (auto& v : warehouseCounts)
         v = warehouseCount;
   }

   dispatch.prepare(connection, "{CALL dispatch(?, ?, ?, ?, ?)}");
   dispatch.bind(transactionNumbers.data(), homeWarehouseIds.data(), warehouseCounts.data(), generateUserRollbacks.data(), generateRemoteAccesses.data());
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

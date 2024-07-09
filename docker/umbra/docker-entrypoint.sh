#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# Setup a database
umbra-sql -createdb /db/umbra.db <<<"ALTER ROLE postgres WITH LOGIN SUPERUSER PASSWORD '${POSTGRES_PASSWORD}';" || exit 1

# Start the server
if [[ $TRACE == 1 ]]; then
  exec umbra-trace-server -createSSLFiles -certFile /db/umbra.cert -keyFile /db/umbra.pem -address 0.0.0.0 /db/umbra.db || exit 1
else
  exec umbra-server -createSSLFiles -certFile /db/umbra.cert -keyFile /db/umbra.pem -address 0.0.0.0 /db/umbra.db || exit 1
fi

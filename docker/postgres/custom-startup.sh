#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

source docker-entrypoint.sh

#---------------------------------------------------------------------------
# vvv copied from Postgres docker-entrypoint.sh
#---------------------------------------------------------------------------
# Copyright (c) 2014, Docker PostgreSQL Authors (See AUTHORS)
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#---------------------------------------------------------------------------
docker_setup_env
docker_create_db_directories

# only run initialization on an empty data directory
if [ -z "$DATABASE_ALREADY_EXISTS" ]; then
  docker_verify_minimum_env
  docker_init_database_dir

  pg_setup_hba_conf "$@"

  export PGPASSWORD="${PGPASSWORD:-$POSTGRES_PASSWORD}"
  docker_temp_server_start "$@"
  docker_setup_db
  docker_temp_server_stop
  unset PGPASSWORD

  echo
  echo 'PostgreSQL init process complete; ready for start up.'
  echo
else
  echo
  echo 'PostgreSQL Database directory appears to contain a database; Skipping initialization'
  echo
fi
#---------------------------------------------------------------------------
# ^^^ copied from docker-entrypoint.sh
#---------------------------------------------------------------------------

# start postgres
postgres "$@"

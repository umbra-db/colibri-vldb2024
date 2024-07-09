#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
FROM ubuntu:23.10

# Install apt dependencies
RUN apt-get update -qq \
    && apt-get install -y \
        binutils \
        ccache \
        cmake \
        g++-13 \
        make \
        ninja-build \
        lsb-release \
        gnupg \
        wget \
        gosu \
        unixodbc-dev \
    && wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | apt-key add - \
    && wget --quiet -O - https://packages.microsoft.com/keys/microsoft.asc | apt-key add - \
    && sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list' \
    && wget --quiet -O - https://packages.microsoft.com/config/ubuntu/22.10/prod.list | tee /etc/apt/sources.list.d/msprod.list \
    && apt-get update -qq \
    && apt-get install -y \
        libpq-dev \
        postgresql-server-dev-14 \
    && ACCEPT_EULA=y apt-get install -y msodbcsql18 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 13 \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 13

WORKDIR /usr/local/src

# Build txbench binaries
COPY src/txbench txbench
RUN set -x \
    && cd txbench \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/ .. \
    && make -j \
    && cd ../..

# Build TPCH data generator
COPY src/tpch-dbgen tpch-dbgen
RUN set -x \
    && cd tpch-dbgen \
    && make \
    && cd ..

# Build the production image
FROM ubuntu:23.10

# Install apt dependencies
RUN apt-get update -qq \
    && apt-get install -y \
        lsb-release \
        gnupg \
        wget \
        gosu \
        unixodbc \
        odbcinst \
        libmariadb-dev \
    && wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | apt-key add - \
    && wget --quiet -O - https://packages.microsoft.com/keys/microsoft.asc | apt-key add - \
    && sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list' \
    && wget --quiet -O - https://packages.microsoft.com/config/ubuntu/22.10/prod.list | tee /etc/apt/sources.list.d/msprod.list \
    && apt-get update -qq \
    && apt-get install -y \
        libpq5 \
    && ACCEPT_EULA=y apt-get install -y msodbcsql18 \
    && wget https://dlm.mariadb.com/3680409/Connectors/odbc/connector-odbc-3.1.20/mariadb-connector-odbc-3.1.20-debian-bookworm-amd64.deb \
    && dpkg -i mariadb-connector-odbc-3.1.20-debian-bookworm-amd64.deb \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

COPY maodbc.ini /maodbc.ini
RUN odbcinst -i -d -n "MariaDB ODBC Connector" -f /maodbc.ini

RUN userdel ubuntu

# Copy binaries
COPY --from=0 /usr/local/src/txbench/build/tools/txbench/txbench /usr/local/bin/txbench
COPY --from=0 /usr/local/src/txbench/build/tools/datagen/txbench-datagen /usr/local/bin/txbench-datagen
COPY --from=0 /usr/local/src/txbench/thirdparty/duckdb/libduckdb.so /lib/x86_64-linux-gnu/libduckdb.so
COPY --from=0 /usr/local/src/tpch-dbgen/dbgen /usr/local/bin/tpch-dbgen
COPY --from=0 /usr/local/src/tpch-dbgen/dists.dss /var/lib/txbench/dists.dss
COPY tpch-dbgen.sh /var/lib/txbench/tpch-dbgen.sh

# Setup entrypoint
COPY entrypoint.sh /entrypoint.sh
RUN set -x \
    && chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

# Client working directory
WORKDIR /var/lib/txbench

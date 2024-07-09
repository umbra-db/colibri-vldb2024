#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
FROM ubuntu:23.10

# Install apt dependencies
RUN apt-get update -qq \
    && apt-get install -y \
        binutils \
        bison \
        ccache \
        clang \
        clang-17 \
        clang-format-17 \
        cmake \
        g++-12 \
        git \
        libasan5 \
        libboost1.74-all-dev \
        libc6-dev \
        libclang-17-dev \
        libjemalloc-dev \
        libre2-dev \
        libssl-dev \
        libtinfo-dev \
        liburing-dev \
        lld \
        lld-17 \
        llvm \
        llvm-17 \
        make \
        ninja-build \
        lsb-release \
        gnupg \
        wget \
        gosu \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 12 \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12

WORKDIR /usr/local/src

# Build cppmake
COPY src/cppmake cppmake
RUN set -x \
    && cd cppmake \
    && make \
    && mv bin/cppmake /usr/local/bin \
    && cd .. \
    && rm -rf cppmake

# Build umbra binaries
COPY src/umbra umbra
RUN set -x \
   && cd umbra \
   && cppmake bin/sql bin/server \
   && TRACE=1 cppmake bin/trace/server

# Build actual production image
FROM ubuntu:23.10

# Install apt dependencies
RUN apt-get update -qq \
    && apt-get install -y \
        libboost-context1.74.0 \
        libboost-filesystem1.74.0 \
        libboost-system1.74.0 \
        libboost-thread1.74.0 \
        libclang1-17 \
        libjemalloc2 \
        libre2-10 \
        libssl3 \
        libtinfo6 \
        liburing2 \
        llvm-17-runtime \
        gosu \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN userdel ubuntu

# Copy Umbra binaries
COPY --from=0 /usr/local/src/umbra/bin/sql /usr/local/bin/umbra-sql
COPY --from=0 /usr/local/src/umbra/bin/server /usr/local/bin/umbra-server
COPY --from=0 /usr/local/src/umbra/bin/trace/server /usr/local/bin/umbra-trace-server

# Setup the entrypoint
COPY entrypoint.sh /entrypoint.sh
COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN set -x \
    && chmod +x /entrypoint.sh \
    && chmod +x /docker-entrypoint.sh
ENTRYPOINT ["/entrypoint.sh", "/docker-entrypoint.sh"]

# Expose the Umbra port
EXPOSE 5432

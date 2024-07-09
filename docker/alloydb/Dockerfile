#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
FROM google/alloydbomni:15

USER root

# Install apt dependencies
RUN apt-get update -qq \
    && apt-get install -y \
        gosu \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Setup entrypoint
COPY entrypoint.sh /entrypoint.sh
RUN set -x \
    && chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh", "/usr/local/bin/docker-entrypoint.sh", "postgres"]

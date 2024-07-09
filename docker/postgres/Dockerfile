#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
FROM postgres:14.1

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

COPY custom-startup.sh /custom-startup.sh
RUN set -x \
    && chmod +x /custom-startup.sh

ENTRYPOINT ["/entrypoint.sh", "/custom-startup.sh"]

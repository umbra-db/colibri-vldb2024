#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
FROM ubuntu:23.10

# Install apt dependencies
RUN apt-get update -qq \
    && apt-get install -y \
        curl \
        gosu \
        cargo \
        rustc \
        tzdata \
    && apt-get clean \
    && cp /usr/share/zoneinfo/Europe/London /etc/localtime \
    && rm -rf /var/lib/apt/lists/*

RUN userdel ubuntu

RUN useradd local
RUN mkdir -p /home/local
RUN chown local:local /home/local

# Setup the entrypoint
COPY entrypoint.sh /entrypoint.sh
COPY docker-entrypoint.sh /docker-entrypoint.sh
COPY install.sh /install.sh
RUN set -x \
    && chmod +x /entrypoint.sh \
    && chmod +x /docker-entrypoint.sh \
    && chmod +x /install.sh
ENTRYPOINT ["/entrypoint.sh", "/docker-entrypoint.sh"]

RUN gosu local "/install.sh"
RUN userdel local

# Expose the TiDB port
EXPOSE 4000

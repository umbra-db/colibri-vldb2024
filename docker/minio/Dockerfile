#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
# Build actual production image
FROM ubuntu:23.10

# Install apt dependencies
RUN apt-get update -qq \
    && apt-get install -y \
            awscli \
            wget \
            gosu \
    && wget https://dl.min.io/server/minio/release/linux-amd64/archive/minio_20230923034750.0.0_amd64.deb -O minio.deb \
    && dpkg -i minio.deb && rm minio.deb \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN userdel ubuntu

# Setup the entrypoint
COPY entrypoint.sh /entrypoint.sh
COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN set -x \
    && chmod +x /entrypoint.sh \
    && chmod +x /docker-entrypoint.sh
ENTRYPOINT ["/entrypoint.sh", "/docker-entrypoint.sh"]

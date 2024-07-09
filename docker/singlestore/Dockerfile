#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
FROM singlestore/cluster-in-a-box:latest

USER root

# Install gosu
ENV GOSU_VERSION=1.17
RUN curl -o /usr/local/bin/gosu -SL "https://github.com/tianon/gosu/releases/download/${GOSU_VERSION}/gosu-amd64" \
    && chmod +x /usr/local/bin/gosu \
    # Verify that the binary works
    && gosu nobody true

# Setup entrypoint
COPY entrypoint.sh /entrypoint.sh
RUN set -x \
    && chmod +x /entrypoint.sh

# Use the local user to start singlestore
RUN chmod 777 /startup
RUN sed -i 's/user = "memsql"/user = "local"/' /etc/memsql/memsqlctl.hcl 

ENTRYPOINT ["/entrypoint.sh", "/startup"]

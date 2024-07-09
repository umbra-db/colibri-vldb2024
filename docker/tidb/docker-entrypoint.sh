#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# Link the data
whoami
cd /home/local
.tiup/bin/tiup -v
.tiup/bin/tiup playground --without-monitor --db.host 0.0.0.0 --pd.host 0.0.0.0

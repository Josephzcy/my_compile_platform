#######
# SPDX license identifier: MPL-2.0
#
# Copyright (C) 2011-2015, BMW AG
#
# This file is part of GENIVI Project DLT - Diagnostic Log and Trace.
#
# This Source Code Form is subject to the terms of the
# Mozilla Public License (MPL), v. 2.0.
# If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#
# For further information see http://www.genivi.org/.
#######

[Unit]
Description=GENIVI DLT system. Application to forward syslog messages to DLT, transfer system information, logs and files.
Documentation=man:dlt-system(1) man:dlt-system.conf(5)
Wants=dlt.service

[Service]
Type=simple
ExecStart=@CMAKE_INSTALL_PREFIX@/bin/dlt-system
WatchdogSec=@DLT_WatchdogSec@
NotifyAccess=main
LimitCORE=infinity

[Install]
WantedBy=basic.target


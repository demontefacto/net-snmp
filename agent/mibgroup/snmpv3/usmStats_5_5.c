/*
 * usmStats.c: implements the usmStats portion of the SNMP-USER-BASED-SM-MIB
 *
 * Portions of this file are subject to the following copyright(s).  See
 * the Net-SNMP's COPYING file for more details and other copyrights
 * that may apply:
 *
 * Portions of this file are copyrighted by:
 * Copyright (c) 2016 VMware, Inc. All rights reserved.
 * Use is subject to license terms specified in the COPYING file
 * distributed with the Net-SNMP package.
 */

#include <net-snmp/net-snmp-config.h>

#include <net-snmp/net-snmp-features.h>

#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/sysORTable.h>

#include "usmStats_5_5.h"

#include <net-snmp/agent/snmp_get_statistic.h>

#define snmpUsmMIB 1, 3, 6, 1, 6, 3, 15
#define usmMIBCompliances snmpUsmMIB, 2, 1

static oid usmStats[] = { snmpUsmMIB, 1, 1 };

static netsnmp_handler_registration* usmStats_reg = NULL;
static oid usmMIBCompliance[] = { usmMIBCompliances, 1 };

netsnmp_feature_require(helper_statistics)

void
init_usmStats_5_5(void)
{
    netsnmp_handler_registration* s =
        netsnmp_create_handler_registration(
            "usmStats", NULL, usmStats, OID_LENGTH(usmStats),
            HANDLER_CAN_RONLY);
    if (!s)
        return;

    if (NETSNMP_REGISTER_STATISTIC_HANDLER(s, 1, USM) != MIB_REGISTERED_OK) {
        netsnmp_handler_registration_free(s);
        return;
    }

    REGISTER_SYSOR_ENTRY(usmMIBCompliance,
                         "The management information definitions for the "
                         "SNMP User-based Security Model.");
    usmStats_reg = s;
}

void
shutdown_usmStats_5_5(void)
{
    UNREGISTER_SYSOR_ENTRY(usmMIBCompliance);
    if (usmStats_reg) {
        netsnmp_unregister_handler(usmStats_reg);
        usmStats_reg = NULL;
    }
}

// SPDX-License-Identifier: GPL-2.0-only
#include <debug.h>
#include <rpm-smd.h>
#include <rpm-ipc.h>
#include <string.h>

#define RPMPD_SMPA            0x61706d73
#define MSM8953_VDDCX_ID      2
#define KEY_LEVEL             0x6c766c76
#define RPM_SMD_LEVEL_TURBO   5

int lk2nd_vddcx_enable(void)
{
    uint32_t data[8];
    int ret;

    memset(data, 0, sizeof(data));
    data[RESOURCETYPE] = RPMPD_SMPA;
    data[RESOURCEID]   = MSM8953_VDDCX_ID;
    data[KVP_KEY]      = KEY_LEVEL;
    data[KVP_LENGTH]   = 4;
    data[KVP_VALUE]    = RPM_SMD_LEVEL_TURBO;

    ret = rpm_smd_send_data(data, sizeof(kvp_data), RPM_REQUEST_TYPE);

    if (ret) {
        dprintf(CRITICAL, "VDDCX: send failed (ret=%d)\n", ret);
    } else {
        dprintf(INFO, "VDDCX: level=5 sent OK\n");
    }
    return ret;
}

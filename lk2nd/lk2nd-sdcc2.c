// SPDX-License-Identifier: GPL-2.0-only
#include <debug.h>
#include <arch/defines.h>
#include <spmi.h>
#include <platform/gpio.h>
#include <platform/timer.h>

#ifndef readl
#define readl(a)     (*((volatile uint32_t *)(a)))
#endif
#ifndef writel
#define writel(v, a) (*((volatile uint32_t *)(a)) = (v))
#endif

#define GCC_BASE       0x01800000
#define CORE_MEM       0x07864000
#define HC_MEM         0x07864900

#define CORE_POWER         0x00
#define CORE_MCI_VERSION   0x50
#define CORE_HC_MODE       0x78
#define HC_MODE_EN         0x1

/* PM8953 SPMI 地址 */
#define PM8953_L11      ((0 << 16) | (0x4A << 8) | 0x46)
#define PM8953_L12      ((0 << 16) | (0x4B << 8) | 0x46)
#define LDO_ENABLE_VAL  0x80

int lk2nd_sdcc2_ldo_enable(void)
{
    uint8_t r11, r12;

    dprintf(INFO, "SDC2: enabling PM8953 L11/L12\n");
    pmic_spmi_reg_write(PM8953_L11, LDO_ENABLE_VAL);
    pmic_spmi_reg_write(PM8953_L12, LDO_ENABLE_VAL);
    for (volatile int i = 0; i < 100000; i++);

    r11 = pmic_spmi_reg_read(PM8953_L11);
    r12 = pmic_spmi_reg_read(PM8953_L12);
    dprintf(INFO, "SDC2: L11 EN=%02X L12 EN=%02X\n", r11, r12);
    return 0;
}

#define TLMM_SDC_QDSD    0x01109000

int lk2nd_sdcc2_gpio_init(void)
{
    uint32_t val;

    /* 1. GPIO133: 卡检测，GPIO 功能，输入，上拉 */
    gpio_tlmm_config(133, 0, GPIO_INPUT, GPIO_PULL_UP, GPIO_2MA, GPIO_ENABLE);
    dprintf(INFO, "SDC2: GPIO133 cfg=0x%X IO=0x%X\n",
            readl(0x01000000 + 133 * 0x1000),
            readl(0x01000000 + 4 + 133 * 0x1000));

    /* 2. ★ SDC2 QDSD 引脚复用（TLMM 0x109000）
     *    来自 Linux SDC_QDSD_PINGROUP 定义：
     *      sdc2_clk:  pull_bit=14, drv_bit=6
     *      sdc2_cmd:  pull_bit=11, drv_bit=3
     *      sdc2_data: pull_bit=9,  drv_bit=0
     */
    val = readl(TLMM_SDC_QDSD);
    dprintf(INFO, "SDC2: QDSD before=0x%08X\n", val);

    /* sdc2_clk: 无上下拉(0)，驱动 16mA(7) */
    val &= ~((3U << 14) | (7U << 6));
    val |=  (0U << 14) | (7U << 6);

    /* sdc2_cmd: 上拉(3)，驱动 10mA(4) */
    val &= ~((3U << 11) | (7U << 3));
    val |=  (3U << 11) | (4U << 3);

    /* sdc2_data: 上拉(3)，驱动 10mA(4) */
    val &= ~((3U << 9) | (7U << 0));
    val |=  (3U << 9) | (4U << 0);

    writel(val, TLMM_SDC_QDSD);
    dprintf(INFO, "SDC2: QDSD after =0x%08X\n", readl(TLMM_SDC_QDSD));

    return 0;
}

/* 上电 SDCC 核心 */
static void core_power_on(void)
{
    /* 1. 软件复位：写 bit7=1 */
    writel((1U << 7), CORE_MEM + CORE_POWER);
    udelay(10000);

    /* 2. 轮询 bit7 清 0 */
    for (volatile int i = 0; i < 1000000; i++) {
        if ((readl(CORE_MEM + CORE_POWER) & (1U << 7)) == 0) break;
    }

    /* 3. 上电：写 bit6=1 */
    writel((1U << 6), CORE_MEM + CORE_POWER);
    udelay(10000);
}

int lk2nd_sdcc2_keepalive(void)
{
    dprintf(INFO, "SDCC2 keepalive: start\n");

    /* 1. GCC 时钟 */
    writel(readl(GCC_BASE + 0x43018) & ~1U, GCC_BASE + 0x43018);
    writel(readl(GCC_BASE + 0x4301C) & ~1U, GCC_BASE + 0x4301C);

    writel(1U, GCC_BASE + 0x43000);
    udelay(1);
    writel(0U, GCC_BASE + 0x43000);

    writel((1U << 8) | 16U, GCC_BASE + 0x43008);
    writel(0U, GCC_BASE + 0x4300C);
    writel(0U, GCC_BASE + 0x43010);
    writel(0U, GCC_BASE + 0x43014);
    writel(1U, GCC_BASE + 0x43004);
    udelay(100);

    writel(readl(GCC_BASE + 0x43018) | 1U, GCC_BASE + 0x43018);
    writel(readl(GCC_BASE + 0x4301C) | 1U, GCC_BASE + 0x4301C);
    udelay(100);

    /* 2. ★ 先写 HC_MODE（Linux 驱动顺序） */
    writel(readl(CORE_MEM + CORE_HC_MODE) | HC_MODE_EN,
           CORE_MEM + CORE_HC_MODE);
    udelay(100);
    dprintf(INFO, "  HC_MODE=%08X\n", readl(CORE_MEM + CORE_HC_MODE));

    /* 3. ★ 再核心复位 + 上电 */
    /* SW_RST: 写 bit7=1 */
    writel((1U << 7), CORE_MEM + CORE_POWER);
    udelay(10000);

    /* 等 bit7 清 0 */
    for (volatile int i = 0; i < 1000000; i++) {
        if ((readl(CORE_MEM + CORE_POWER) & (1U << 7)) == 0) break;
    }

    /* PWR_ON: 写 bit6=1 */
    writel((1U << 6), CORE_MEM + CORE_POWER);
    udelay(10000);

    dprintf(INFO, "  CORE_POWER=%08X HC_MODE=%08X\n",
            readl(CORE_MEM + CORE_POWER),
            readl(CORE_MEM + CORE_HC_MODE));

    /* 4. 时钟：内部时钟使能 + 400kHz */
    *(volatile uint16_t *)(HC_MEM + 0x2C) = 0x0000;
    udelay(10);
    *(volatile uint16_t *)(HC_MEM + 0x2C) = 0x0001;
    for (volatile int i = 0; i < 1000000; i++) {
        if (*(volatile uint16_t *)(HC_MEM + 0x2C) & 0x0002) break;
    }

    *(volatile uint16_t *)(HC_MEM + 0x2C) = (61U << 8) | 0x0007;
    udelay(100);

    dprintf(INFO, "  SDHCI CLK=%04X PRESENT=%08X\n",
            *(volatile uint16_t *)(HC_MEM + 0x2C),
            readl(HC_MEM + 0x24));

    dprintf(INFO, "SDCC2 keepalive: done\n");
    return 0;
}

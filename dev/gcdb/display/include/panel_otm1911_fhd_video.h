// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2026 FIXME
// Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:
//   Copyright (c) 2014, The Linux Foundation. All rights reserved. (FIXME)

#ifndef _PANEL_OTM1911_FHD_VIDEO_H_
#define _PANEL_OTM1911_FHD_VIDEO_H_

#include <mipi_dsi.h>
#include <panel_display.h>
#include <panel.h>
#include <string.h>

static struct panel_config otm1911_fhd_video_panel_data = {
	.panel_node_id = "qcom,mdss_dsi_otm1911_fhd_video",
	.panel_controller = "dsi:0:",
	.panel_compatible = "qcom,mdss-dsi-panel",
	.panel_type = 0,
	.panel_destination = "DISPLAY_1",
	/* .panel_orientation not supported yet */
	.panel_framerate = 60,
	.panel_lp11_init = 0,
	.panel_init_delay = 0,
};

static struct panel_resolution otm1911_fhd_video_panel_res = {
	.panel_width = 1080,
	.panel_height = 1920,
	.hfront_porch = 24,
	.hback_porch = 24,
	.hpulse_width = 20,
	.hsync_skew = 0,
	.vfront_porch = 14,
	.vback_porch = 6,
	.vpulse_width = 2,
	/* Borders not supported yet */
};

static struct color_info otm1911_fhd_video_color = {
	.color_format = 24,
	.color_order = DSI_RGB_SWAP_RGB,
	.underflow_color = 0xff,
	/* Borders and pixel packing not supported yet */
};

static char otm1911_fhd_video_on_cmd_0[] = {
	0x02, 0x00, 0x39, 0xc0, 0x51, 0xff, 0xff, 0xff
};
static char otm1911_fhd_video_on_cmd_1[] = {
	0x02, 0x00, 0x39, 0xc0, 0x53, 0x2c, 0xff, 0xff
};
static char otm1911_fhd_video_on_cmd_2[] = {
	0x02, 0x00, 0x39, 0xc0, 0x55, 0x00, 0xff, 0xff
};
static char otm1911_fhd_video_on_cmd_3[] = {
	0x02, 0x00, 0x39, 0xc0, 0x11, 0x00, 0xff, 0xff
};
static char otm1911_fhd_video_on_cmd_4[] = {
	0x02, 0x00, 0x39, 0xc0, 0x29, 0x00, 0xff, 0xff
};

static struct mipi_dsi_cmd otm1911_fhd_video_on_command[] = {
	{ sizeof(otm1911_fhd_video_on_cmd_0), otm1911_fhd_video_on_cmd_0, 0 },
	{ sizeof(otm1911_fhd_video_on_cmd_1), otm1911_fhd_video_on_cmd_1, 0 },
	{ sizeof(otm1911_fhd_video_on_cmd_2), otm1911_fhd_video_on_cmd_2, 0 },
	{ sizeof(otm1911_fhd_video_on_cmd_3), otm1911_fhd_video_on_cmd_3, 120 },
	{ sizeof(otm1911_fhd_video_on_cmd_4), otm1911_fhd_video_on_cmd_4, 20 },
};

static char otm1911_fhd_video_off_cmd_0[] = {
	0x02, 0x00, 0x39, 0xc0, 0x28, 0x00, 0xff, 0xff
};
static char otm1911_fhd_video_off_cmd_1[] = {
	0x02, 0x00, 0x39, 0xc0, 0x10, 0x00, 0xff, 0xff
};

static struct mipi_dsi_cmd otm1911_fhd_video_off_command[] = {
	{ sizeof(otm1911_fhd_video_off_cmd_0), otm1911_fhd_video_off_cmd_0, 50 },
	{ sizeof(otm1911_fhd_video_off_cmd_1), otm1911_fhd_video_off_cmd_1, 120 },
};

static struct command_state otm1911_fhd_video_state = {
	.oncommand_state = 0,
	.offcommand_state = 0,
};

static struct commandpanel_info otm1911_fhd_video_command_panel = {
	/* Unused, this is a video mode panel */
};

static struct videopanel_info otm1911_fhd_video_video_panel = {
	.hsync_pulse = 1,
	.hfp_power_mode = 0,
	.hbp_power_mode = 0,
	.hsa_power_mode = 0,
	.bllp_eof_power_mode = 1,
	.bllp_power_mode = 1,
	.traffic_mode = 2,
	/* This is bllp_eof_power_mode and bllp_power_mode combined */
	.bllp_eof_power = 1 << 3 | 1 << 0,
};

static struct lane_configuration otm1911_fhd_video_lane_config = {
	.dsi_lanes = 4,
	.dsi_lanemap = 0,
	.lane0_state = 1,
	.lane1_state = 1,
	.lane2_state = 1,
	.lane3_state = 1,
	.force_clk_lane_hs = 0,
};

static const uint32_t otm1911_fhd_video_timings[] = {
	0x7d, 0x25, 0x1d, 0x00, 0x37, 0x33, 0x22, 0x27, 0x1e, 0x03, 0x04, 0x00
};

static struct panel_timing otm1911_fhd_video_timing_info = {
	.tclk_post = 0x0d,
	.tclk_pre = 0x2e,
};

static struct panel_reset_sequence otm1911_fhd_video_reset_seq = {
	.pin_state = { 1, 0, 1 },
	.sleep = { 10, 10, 10 },
	.pin_direction = 2,
};

static struct backlight otm1911_fhd_video_backlight = {
	.bl_interface_type = BL_WLED,
	.bl_min_level = 1,
	.bl_max_level = 4095,
};

static inline void panel_otm1911_fhd_video_select(struct panel_struct *panel,
						  struct msm_panel_info *pinfo,
						  struct mdss_dsi_phy_ctrl *phy_db)
{
	panel->paneldata = &otm1911_fhd_video_panel_data;
	panel->panelres = &otm1911_fhd_video_panel_res;
	panel->color = &otm1911_fhd_video_color;
	panel->videopanel = &otm1911_fhd_video_video_panel;
	panel->commandpanel = &otm1911_fhd_video_command_panel;
	panel->state = &otm1911_fhd_video_state;
	panel->laneconfig = &otm1911_fhd_video_lane_config;
	panel->paneltiminginfo = &otm1911_fhd_video_timing_info;
	panel->panelresetseq = &otm1911_fhd_video_reset_seq;
	panel->backlightinfo = &otm1911_fhd_video_backlight;
	pinfo->mipi.panel_on_cmds = otm1911_fhd_video_on_command;
	pinfo->mipi.panel_off_cmds = otm1911_fhd_video_off_command;
	pinfo->mipi.num_of_panel_on_cmds = ARRAY_SIZE(otm1911_fhd_video_on_command);
	pinfo->mipi.num_of_panel_off_cmds = ARRAY_SIZE(otm1911_fhd_video_off_command);
	memcpy(phy_db->timing, otm1911_fhd_video_timings, TIMING_SIZE);
	phy_db->regulator_mode = DSI_PHY_REGULATOR_DCDC_MODE;
}

#endif /* _PANEL_OTM1911_FHD_VIDEO_H_ */

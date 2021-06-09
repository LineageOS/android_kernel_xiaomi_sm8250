/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _DSI_PANEL_MI_H_
#define _DSI_PANEL_MI_H_

#include <linux/of_device.h>
#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/backlight.h>
#include <drm/drm_panel.h>
#include <drm/msm_drm.h>

#include "dsi_defs.h"
#include "dsi_ctrl_hw.h"
#include "dsi_clk.h"
#include "dsi_pwr.h"
#include "dsi_parser.h"
#include "msm_drv.h"

#define DEFAULT_FOD_OFF_DIMMING_DELAY     170
#define DEFAULT_FOD_OFF_ENTER_AOD_DELAY   300
#define DISPPARAM_THERMAL_SET             0x1

#define MAX_VSYNC_COUNT                   200

enum doze_bkl {
	DOZE_TO_NORMAL = 0,
	DOZE_BRIGHTNESS_HBM,
	DOZE_BRIGHTNESS_LBM,
};

enum bkl_dimming_state {
	STATE_NONE,
	STATE_DIM_BLOCK,
	STATE_DIM_RESTORE,
	STATE_ALL
};

enum dsi_gamma_cmd_set_type {
	DSI_CMD_SET_MI_GAMMA_SWITCH_60HZ = 0,
	DSI_CMD_SET_MI_GAMMA_SWITCH_90HZ,
	DSI_CMD_SET_MI_GAMMA_SWITCH_MAX
};

/* 60Hz gamma and 90Hz gamma info */
struct gamma_cfg {
	bool read_done;
	u8 otp_read_c8[135];
	u8 otp_read_c9[180];
	u8 otp_read_b3[45];

	u32 flash_read_total_param;
	u32 flash_read_c1_index;
	u32 gamma_checksum;
	u8 flash_read_c8[135];
	u8 flash_read_c9[180];
	u8 flash_read_b3[45];
	u8 flash_read_checksum[2];

	u32 update_c8_index;
	u32 update_c9_index;
	u32 update_b3_index;
	bool update_done_60hz;
	bool update_done_90hz;

	bool black_setting_flag;
};

/* Enter/Exit DC_LUT info */
struct dc_cfg {
	bool read_done;
	bool update_done;
	u32 update_d2_index;
	u8 enter_dc_lut[75];
	u8 exit_dc_lut[75];
};

struct greenish_gamma_cfg {
	u32 index_1st_param;
	u32 index_2nd_param;
	u32 index_3rd_param;
	u32 index_4th_param;
	u32 index_5th_param;
	u32 index_6th_param;

	u32 greenish_gamma_update_offset;
	u32 greenish_gamma_update_param_count;

	bool gamma_update_done;
};

typedef struct brightness_alpha {
	uint32_t brightness;
	uint32_t alpha;
} brightness_alpha;

struct dsi_panel_mi_cfg {
	struct dsi_panel *dsi_panel;

	/* xiaomi panel id */
	u64 panel_id;

	/* xiaomi feature enable flag */
	bool mi_feature_enabled;

	/* bl_is_big_endian indicate brightness value
	 * high byte to 1st parameter, low byte to 2nd parameter
	 * eg: 0x51 { 0x03, 0xFF } ->
	 * u8 payload[2] = { brightness >> 8, brightness & 0xff}
	 */
	bool bl_is_big_endian;
	u32 last_bl_level;

	/* indicate refresh frequency Fps gpio */
	int disp_rate_gpio;

	/* gamma read */
	bool gamma_update_flag;
	struct gamma_cfg gamma_cfg;

	/* greenish gamma read */
	bool greenish_gamma_update_flag;
	u32 greenish_gamma_read_len;
	struct greenish_gamma_cfg greenish_gamma_cfg;

	/* dc read */
	bool dc_update_flag;
	struct dc_cfg dc_cfg;

	/* white point coordinate info */
	bool wp_read_enabled;
	u32 wp_reg_read_len;
	u32 wp_info_index;
	u32 wp_info_len;

	/* HBM and brightness use 51 reg ctrl */
	bool hbm_51_ctrl_flag;
	u32 hbm_off_51_index;
	u32 fod_off_51_index;
	u32 fod_off_b5_index;
	u32 fod_on_b2_index;
	bool vi_setting_enabled;
	u32 vi_switch_threshold;

	bool dynamic_elvss_enabled;

	/* elvss dimming info */
	bool elvss_dimming_check_enable;
	u32 elvss_dimming_read_len;
	u32 update_hbm_fod_on_index;
	u32 update_hbm_fod_off_index;

	u32 dimming_state;
	u32 panel_on_dimming_delay;
	struct delayed_work dimming_enable_delayed_work;

	struct delayed_work enter_aod_delayed_work;

	bool hbm_enabled;
	bool thermal_hbm_disabled;
	bool fod_hbm_enabled;
	bool fod_hbm_layer_enabled;
	u32 doze_brightness_state;
	u32 unset_doze_brightness;
	u32 fod_off_dimming_delay;
	ktime_t fod_backlight_off_time;
	ktime_t fod_hbm_off_time;
	u32 fod_ui_ready;
	bool layer_fod_unlock_success;
	bool sysfs_fod_unlock_success;
	bool into_aod_pending;
	bool layer_aod_flag;
	struct wakeup_source *aod_wakelock;

	bool fod_backlight_flag;
	u32 fod_target_backlight;
	bool fod_flag;
	/* set doze hbm/lbm only in AOD */
	bool in_aod;
	u32 dc_threshold;
	bool dc_enable;
	u32 dc_type;
	u32 hbm_brightness;
	u32 max_brightness_clone;
	u32 aod_backlight;
	uint32_t doze_brightness;
	bool is_tddi_flag;
	bool tddi_doubleclick_flag;
	bool panel_dead_flag;

	bool fod_dimlayer_enabled;
	bool prepare_before_fod_hbm_on;
	bool delay_before_fod_hbm_on;
	bool delay_after_fod_hbm_on;
	bool delay_before_fod_hbm_off;
	bool delay_after_fod_hbm_off;
	uint32_t brightnes_alpha_lut_item_count;
	brightness_alpha *brightness_alpha_lut;

	/* smart fps control */
	bool smart_fps_support;
	bool smart_fps_restore;
	u32 smart_fps_max_framerate;
	u32 smart_fps_value;

	bool dither_enabled;
};

struct dsi_read_config {
	bool is_read;
	struct dsi_panel_cmd_set read_cmd;
	u32 cmds_rlen;
	u8 rbuf[256];
};

struct hw_vsync_info {
	u32 config_fps;
	ktime_t timestamp;
	u64 real_vsync_period_ns;
};

struct calc_hw_vsync {
	struct hw_vsync_info vsyc_info[MAX_VSYNC_COUNT];
	int vsync_count;
	ktime_t last_timestamp;
	u64 measured_vsync_period_ns;
	u64 measured_fps_x1000;
};

int dsi_panel_parse_mi_config(struct dsi_panel *panel,
				struct device_node *of_node);

void display_utc_time_marker(const char *format, ...);

int dsi_panel_write_cmd_set(struct dsi_panel *panel,
				struct dsi_panel_cmd_set *cmd_sets);

int dsi_panel_read_cmd_set(struct dsi_panel *panel,
				struct dsi_read_config *read_config);

int dsi_panel_write_mipi_reg(struct dsi_panel *panel, char *buf);

ssize_t dsi_panel_read_mipi_reg(struct dsi_panel *panel, char *buf);

int dsi_panel_set_disp_param(struct dsi_panel *panel, u32 param);

int dsi_panel_read_gamma_param(struct dsi_panel *panel);

ssize_t dsi_panel_print_gamma_param(struct dsi_panel *panel,
				char *buf);

int dsi_panel_update_gamma_param(struct dsi_panel *panel);

int dsi_panel_write_gamma_cmd_set(struct dsi_panel *panel,
				enum dsi_gamma_cmd_set_type type);

int dsi_panel_read_dc_param(struct dsi_panel *panel);

int dsi_panel_update_dc_param(struct dsi_panel *panel);

int dsi_panel_switch_disp_rate_gpio(struct dsi_panel *panel);

ssize_t dsi_panel_read_wp_info(struct dsi_panel *panel, char *buf);

int dsi_panel_set_doze_brightness(struct dsi_panel *panel,
				int doze_brightness, bool need_panel_lock);

ssize_t dsi_panel_get_doze_brightness(struct dsi_panel *panel, char *buf);

int dsi_panel_update_elvss_dimming(struct dsi_panel *panel);

int dsi_panel_read_greenish_gamma_setting(struct dsi_panel *panel);

int dsi_panel_update_greenish_gamma_setting(struct dsi_panel *panel);

int dsi_panel_set_thermal_hbm_disabled(struct dsi_panel *panel,
				bool thermal_hbm_disabled);
int dsi_panel_get_thermal_hbm_disabled(struct dsi_panel *panel,
				bool *thermal_hbm_disabled);

struct calc_hw_vsync *get_hw_calc_vsync_struct(int dsi_display_type);
ssize_t calc_hw_vsync_info(struct dsi_panel *panel,
				char *buf);

#endif /* _DSI_PANEL_MI_H_ */

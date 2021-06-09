
#ifndef XIAOMI_FRAME_STAT_H_
#define XIAOMI_FRAME_STAT_H_

#include <linux/ktime.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/kernel.h>
#include <linux/timekeeping.h>
#include "msm_drv.h"
#include "sde_crtc.h"
#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/msm_drm.h>
#include "dsi_panel.h"
#include "dsi_display.h"

#ifndef FPS_PERIOD_1_SEC
#define FPS_PERIOD_1_SEC	(1000000)
#endif
#define NANO_TO_MICRO				(1000)
#define MAX_STAT_FRAME_COUNT 		(60)
#define STAT_ITEM_NUM 				(5)
#define IDLE_FPS					(50)
#define DEFAULT_FPS					(60)
#define LONG_INTERVAL_FRAME_COUNT	(3)
#define LONG_FRAME_INTERVAL			(70000)

extern struct drm_crtc *gcrtc;
extern struct dsi_panel *g_panel;

enum stat_item {
	COMMIT_START_TS = 1,
	GET_INPUT_FENCE_TS,
	VBLANK_TS,
	RETIRE_FENCE_TS,
	COMMIT_END_TS,
	RESERVED = 0xF,
};

struct frame_stat {
	ktime_t commit_start_ts;
	ktime_t get_input_fence_ts;
	ktime_t retire_fence_ts;
	ktime_t commit_end_ts;
	ktime_t last_sampled_time_us;
	ktime_t max_frame_duration;
	ktime_t delta_commit_duration;
	ktime_t last_frame_commit_time_us;
	ktime_t max_input_fence_duration;
	ktime_t delta_input_duration;
	u64 input_fence_duration;
	u64 frame_count;
	bool start;
	/* enabled will be changed by user application, the false indicates smart dfps disabled */
	bool enabled;
	bool idle_status;
	bool skip_once;
	u64 last_fps;
	u64 skip_count;
};

void frame_stat_collector(u64 duration, enum stat_item item);
void calc_fps(u64 duration, int input_event);
void frame_stat_notify(int data);
ssize_t smart_fps_value_show(struct device *device,
			struct device_attribute *attr,
			char *buf);
#endif

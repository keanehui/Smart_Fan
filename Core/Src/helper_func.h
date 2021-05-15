#ifndef HELPER_FUNC_H
#define HELPER_FUNC_H

#include "main.h"
#include "lcdtp.h"
#include "xpt2046.h"
#include "stdio.h"



struct Fan_Para_t {
	float air_tempt;
	float fan_speed_pctg; // 0 to 100
	uint32_t fan_speed_input_m; // adc
	int op_mode; // off, auto, manual
	int silent_enabled; // off, on
	int ctime_hms[3];
	int ctime_selected[3];
	int ctime_status;
	uint32_t sound_intensity;

	uint16_t touch_X, touch_Y;
	uint32_t sys_starttime;
	uint32_t currentTime;
	uint32_t button_nextcheck;
	uint32_t screen_nextrefresh;
	uint32_t screen_nexttouch;
	uint32_t counter_nexttick;
	uint32_t tempt_nextread;
	uint32_t sound_nextread;
};


enum display_section {LABELS=0, TEMPTNOISE, SPEED, OP_MODE, SILENT_MODE, COUNTER, COUNTER_SELECTOR, COUNTER_CONTROL};

void fan_turnoff(struct Fan_Para_t* fanPara);
void fan_switch_op_mode(struct Fan_Para_t* fanPara);
void fan_switch_silent_mode(struct Fan_Para_t* fanPara);
float fan_update_pctg(struct Fan_Para_t* fanPara);
void screen_draw_ui_section(struct Fan_Para_t* fanPara, enum display_section section);
void screen_clear_section(enum display_section section);
void screen_info_refresh(struct Fan_Para_t* fanPara);
int ctime_isEmpty(struct Fan_Para_t* fanPara);
void ctime_start_cancel(struct Fan_Para_t* fanPara);
void ctime_toggleSelected(struct Fan_Para_t* fanPara);
void ctime_increSelected(struct Fan_Para_t* fanPara);
int ctime_getSelected(struct Fan_Para_t* fanPara);
void ctime_tick(struct Fan_Para_t* fanPara);
void PWM_set_value(uint16_t value);
void motor_set(float speed_pctg);
void RGB_set(struct Fan_Para_t* fanPara);








#endif

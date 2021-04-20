#ifndef HELPER_FUNC_H
#define HELPER_FUNC_H

#include "main.h"
#include "lcd.h"

enum display_section {TEMPT=0, SPEED, OP_MODE, SILENT_MODE};

int fan_switch_op_mode(int current_mode);
int fan_switch_silent_mode(int current_mode);
void screen_draw_ui(float temperature, float current_power, int op_mode, int silent_mode_enable);
void screen_clear(enum display_section section);








#endif

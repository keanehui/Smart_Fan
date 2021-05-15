#include "helper_func.h"

void fan_switch_op_mode(struct Fan_Para_t* fanPara) {
	if (fanPara->touch_X >= 15 && fanPara->touch_X <= 50 && fanPara->touch_Y >= 100 && fanPara->touch_Y <= 125) {
		fanPara->op_mode = 0;
		fan_turnoff(fanPara);
	}
	else if (fanPara->touch_X >= 95 && fanPara->touch_X <= 140 && fanPara->touch_Y >= 100 && fanPara->touch_Y <= 125)
		fanPara->op_mode = 1;
	else if (fanPara->touch_X >= 170 && fanPara->touch_X <= 230 && fanPara->touch_Y >= 100 && fanPara->touch_Y <= 125) {
		fanPara->op_mode = 2;
		fanPara->silent_enabled = 0;
		screen_clear_section(SILENT_MODE);
		screen_draw_ui_section(fanPara, SILENT_MODE);
	}
	else
		;
	
	if (fanPara->op_mode == 2) // manual led
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}

void fan_switch_silent_mode(struct Fan_Para_t* fanPara) {
	if (fanPara->touch_X >= 45 && fanPara->touch_X <= 75 && fanPara->touch_Y >= 165 && fanPara->touch_Y <= 190)
		fanPara->silent_enabled = 0;
	else if (fanPara->touch_X >= 165 && fanPara->touch_X <= 190 && fanPara->touch_Y >= 165 && fanPara->touch_Y <= 190) {
		if (fanPara->op_mode == 1)
		fanPara->silent_enabled = 1;
	}
	else
		;
}

void fan_turnoff(struct Fan_Para_t* fanPara) {
	fanPara->op_mode = 0;
	fanPara->fan_speed_pctg = 0.0;
	fanPara->silent_enabled = 0;
	fanPara->ctime_status = 0;
	fanPara->ctime_hms[0] = 0;
	fanPara->ctime_hms[1] = 0;
	fanPara->ctime_hms[2] = 0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); // manual led off
	screen_clear_section(OP_MODE);
	screen_draw_ui_section(fanPara, OP_MODE);
	screen_clear_section(SILENT_MODE);
	screen_draw_ui_section(fanPara, SILENT_MODE);
	screen_clear_section(COUNTER_CONTROL);
	screen_draw_ui_section(fanPara, COUNTER_CONTROL);
}

void screen_draw_ui_section(struct Fan_Para_t* fanPara, enum display_section section) {
	char tempt_buffer[10];
	char noise_buffer[10];
	int power_bar_length;
	int hrs;
	int mins;
	int secs;
	char hms_str[20];
	
	switch(section) {
		case LABELS:
			LCD_DrawString_Color(30, 10, "Tempt(C): ", WHITE, BLACK); // tempt
			LCD_DrawString_Color(150, 10, "Noise: ", WHITE, BLACK); // noise
		
			LCD_DrawString_Color(90, 30, "Speed(%)", WHITE, BLACK); // speed
			LCD_DrawLine(20, 50, 220, 50, BLUE); // frame top y50
			LCD_DrawLine(20, 65, 220, 65, BLUE); // frame bottom y65
			LCD_DrawLine(20, 50, 20, 65, BLUE); // frame left x20
			LCD_DrawLine(220, 50, 220, 65, BLUE); // frame right x220
			
			LCD_DrawString_Color(105, 90, "Mode", WHITE, BLACK); // op_mode
			LCD_DrawString(20, 105, "Off");
			LCD_DrawString(100, 105, "Auto");
			LCD_DrawString(175, 105, "Manual");
			
			LCD_DrawString_Color(100, 150, "Silent", WHITE, BLACK); // silent_mode
			LCD_DrawString(50, 170, "Off");
			LCD_DrawString(170, 170, "On");
			
			LCD_DrawString_Color(100, 200, "Timer", WHITE, BLACK); // counter
			
			break;
		
		case TEMPTNOISE:
			sprintf(tempt_buffer, "%.1f", fanPara->air_tempt);
			sprintf(noise_buffer, "%d", fanPara->sound_intensity);
			LCD_DrawString(105, 10, tempt_buffer);
			LCD_DrawString(202, 10, noise_buffer);
			break;
		
		case SPEED:
			power_bar_length = 200*(fanPara->fan_speed_pctg);
			for (int i = 0; i < 15; ++i) // fill frame
				LCD_DrawLine(20, 50+i, 20+power_bar_length, 50+i, BLUE);
			break;
		
		case OP_MODE:
			if (fanPara->op_mode == 0) {
				//LCD_DrawLine(18, 121, 43, 121, BLUE); // off underline
				LCD_DrawString_Color(20, 105, "Off", YELLOW, BLACK);
				LCD_DrawString(100, 105, "Auto");
				LCD_DrawString(175, 105, "Manual");
			}
			else if (fanPara->op_mode == 1) {
				LCD_DrawString(20, 105, "Off");
				LCD_DrawString_Color(100, 105, "Auto", YELLOW, BLACK);
				LCD_DrawString(175, 105, "Manual");
			}
			else if (fanPara->op_mode == 2) {
				LCD_DrawString(20, 105, "Off");
				LCD_DrawString(100, 105, "Auto");
				LCD_DrawString_Color(175, 105, "Manual", YELLOW, BLACK);
			}
			break;
			
		case SILENT_MODE:
			if (fanPara->silent_enabled == 0) {
				LCD_DrawString_Color(50, 170, "Off", YELLOW, BLACK);
				LCD_DrawString(170, 170, "On");
			}
			else {
				LCD_DrawString(50, 170, "Off");
				LCD_DrawString_Color(170, 170, "On", YELLOW, BLACK);
			}
			break;
			
		case COUNTER:
			hrs = fanPara->ctime_hms[0];
			mins = fanPara->ctime_hms[1];
			secs = fanPara->ctime_hms[2];
			sprintf(hms_str, "%02d   :   %02d   :   %02d", hrs, mins, secs);
			LCD_DrawString(40, 220, hms_str);
			break;
		
		case COUNTER_SELECTOR:
			if (ctime_getSelected(fanPara) == 0)
				LCD_DrawString_Color(41, 235, "^^", WHITE, BLACK);
			else if (ctime_getSelected(fanPara) == 1)
				LCD_DrawString_Color(112, 235, "^^", WHITE, BLACK);
			else if (ctime_getSelected(fanPara) == 2)
				LCD_DrawString_Color(184, 235, "^^", WHITE, BLACK);
			else
				;
			break;
		
		case COUNTER_CONTROL:
			if (fanPara->ctime_status == 0) {
				LCD_DrawString(87, 250, "Start(K2)");
			} else {
				LCD_DrawString(85, 250, "Cancel(K2)");
			}
			break;
		
		default:
			;
	}
	
	return;
}

void screen_clear_section(enum display_section section) {
	switch (section) {
		case TEMPTNOISE:
			LCD_Clear(105, 10, 40, 20, WHITE);
			LCD_Clear(202, 10, 40, 20, WHITE);
			break;
		case SPEED:
			LCD_Clear(21, 51, 199, 14, WHITE);
			break;
		case OP_MODE:
			//LCD_DrawLine(20, 105, 225, 120, RED);
			LCD_Clear(20, 105, 205, 15, WHITE);
			break;
		case SILENT_MODE:
			//LCD_DrawLine(50, 170, 190, 185, RED);
			LCD_Clear(50, 170, 140, 15, WHITE);
			break;
		case COUNTER:
			LCD_Clear(40, 220, 170, 15, WHITE);
			break;
		case COUNTER_SELECTOR:
			LCD_Clear(41, 235, 160, 5, WHITE);
			break;
		case COUNTER_CONTROL:
			LCD_Clear(85, 250, 80, 20, WHITE);
			break;
		default:
			LCD_Clear(0, 0, 240, 320, WHITE);
	}
	return;
}

void screen_info_refresh(struct Fan_Para_t* fanPara) {
	screen_clear_section(TEMPTNOISE);
	screen_draw_ui_section(fanPara, TEMPTNOISE);
	screen_clear_section(SPEED);
	screen_draw_ui_section(fanPara, SPEED);
	screen_clear_section(COUNTER);
	screen_draw_ui_section(fanPara, COUNTER);
	screen_clear_section(COUNTER_SELECTOR);
	screen_draw_ui_section(fanPara, COUNTER_SELECTOR);
}

int ctime_isEmpty(struct Fan_Para_t* fanPara) {
	if (fanPara->ctime_hms[0] != 0 || fanPara->ctime_hms[1] != 0 || fanPara->ctime_hms[2] != 0)
		return 0;
	return 1;
}

void ctime_start_cancel(struct Fan_Para_t* fanPara) {
	if (fanPara->ctime_status == 0 && !ctime_isEmpty(fanPara)) { // start
		fanPara->ctime_status = 1;
		fanPara->counter_nexttick = HAL_GetTick();
	}
	else { // cancel
		fanPara->ctime_status = 0;
		fanPara->ctime_hms[0] = 0;
		fanPara->ctime_hms[1] = 0;
		fanPara->ctime_hms[2] = 0;
	}
}

void ctime_toggleSelected(struct Fan_Para_t* fanPara) {
	if (fanPara->ctime_selected[0]==0 && fanPara->ctime_selected[1]==0 && fanPara->ctime_selected[2]==0) { // no selected, choose 1
		fanPara->ctime_selected[0] = 1;
	}	else if (fanPara->ctime_selected[0] == 1) { // now 1, choose 2
		fanPara->ctime_selected[0] = 0;
		fanPara->ctime_selected[1] = 1;
		fanPara->ctime_selected[2] = 0;
	} else if (fanPara->ctime_selected[1] == 1) { // now 2, choose 3
		fanPara->ctime_selected[0] = 0;
		fanPara->ctime_selected[1] = 0;
		fanPara->ctime_selected[2] = 1;
	} else if (fanPara->ctime_selected[2] == 1) { // now 3, clear
		fanPara->ctime_selected[0] = 0;
		fanPara->ctime_selected[1] = 0;
		fanPara->ctime_selected[2] = 0;
	}
}

int ctime_getSelected(struct Fan_Para_t* fanPara) {
	for (int i = 0; i < 3; ++i) {
		if (fanPara->ctime_selected[i] == 1)
			return i;
	}
	return -1;
}

void ctime_increSelected(struct Fan_Para_t* fanPara) {
	int selected = ctime_getSelected(fanPara);
	fanPara->ctime_hms[selected]++;
	fanPara->ctime_hms[selected] %= 60;
}

void ctime_tick(struct Fan_Para_t* fanPara) {
	int ctime_total; // convert hms to sec
	ctime_total = (fanPara->ctime_hms[0])*60*60 + (fanPara->ctime_hms[1])*60 + fanPara->ctime_hms[2];
	if (ctime_total <= 0) { // exit if time up
		fanPara->ctime_status = 0;
		
		return;
	}

	ctime_total -= 1;

	if (ctime_total <= 0) { // time up
		fanPara->ctime_hms[0] = 0;
		fanPara->ctime_hms[1] = 0;
		fanPara->ctime_hms[2] = 0;
		fanPara->ctime_status = 0;
		return;
	}

	int n_hour, n_min, n_sec; // get new ctime_hms
	n_hour = ctime_total/(60*60);
	ctime_total -= n_hour*60*60;
	n_min = ctime_total/60;
	ctime_total -= n_min*60;
	n_sec = ctime_total;

	fanPara->ctime_hms[0] = n_hour; // update ctime_hms
	fanPara->ctime_hms[1] = n_min;
	fanPara->ctime_hms[2] = n_sec;
}


void motor_set(float speed_pctg) {
	int pulse = 1200*speed_pctg;
	PWM_set_value(pulse);
}



float fan_update_pctg(struct Fan_Para_t* fanPara) {
	float new_pctg;
	if (fanPara->op_mode == 0) { // off
		new_pctg = 0.0;
		return new_pctg;
	}
	if (fanPara->op_mode == 1) { // auto
		if (fanPara->air_tempt < 23) {
			new_pctg = 0.2;
		}
		else if (fanPara->air_tempt >= 23 && fanPara->air_tempt < 24) {
			new_pctg = 0.3;
		}
		else if (fanPara->air_tempt >= 24 && fanPara->air_tempt < 25) {
			new_pctg = 0.4;
		}
		else if (fanPara->air_tempt >= 25 && fanPara->air_tempt < 26) {
			new_pctg = 0.5;
		}
		else if (fanPara->air_tempt >= 26 && fanPara->air_tempt < 27) {
			new_pctg = 0.6;
		}
		else if (fanPara->air_tempt >= 27 && fanPara->air_tempt < 28) {
			new_pctg = 0.7;
		}
		else if (fanPara->air_tempt >= 28 && fanPara->air_tempt < 29) {
			new_pctg = 0.8;
		}
		else if (fanPara->air_tempt >= 29 && fanPara->air_tempt < 30) {
			new_pctg = 0.9;
		}
		else { // full speed
			new_pctg = 1;
		}
		if (fanPara->silent_enabled == 1 && fanPara->sound_intensity != 0) {
			if (fanPara->sound_intensity < 30) { // limit to slow
				new_pctg = (new_pctg > 0.25) ? 0.25 : new_pctg;
			}
			else if (fanPara->sound_intensity >= 30 && fanPara->sound_intensity < 100) { // limit to mid
				new_pctg = (new_pctg > 0.5) ? 0.5 : new_pctg;
			}
			else // no limit if too noisy
				;
		}
	}
	else if (fanPara->op_mode == 2) { // manual
		new_pctg = fanPara->fan_speed_input_m/4095.0;
	}
	
	return new_pctg;
}

void RGB_set(struct Fan_Para_t* fanPara) {
	float speed_pctg = fanPara->fan_speed_pctg;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	if (fanPara->op_mode == 0)
		return;
	
	if (speed_pctg <= 0.05)
		return;
	
	else if (speed_pctg <= 0.25) { // R
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	}
	else if (speed_pctg > 0.25 && speed_pctg <= 0.50) { // G
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	}
	else if (speed_pctg > 0.50 && speed_pctg <= 0.75) { // B
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	} 
	else if (speed_pctg > 0.75) { // W
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}
}


#include "helper_func.h"

int fan_switch_op_mode(int current_mode) {
	current_mode++;
	current_mode %= 3;
	return current_mode;
}

int fan_switch_silent_mode(int current_mode) {
	current_mode++;
	current_mode %= 2;
	return current_mode;
}



void screen_draw_ui_section(float temperature, float current_power_pctg, int op_mode, int silent_mode_enable, enum display_section section) {
	if (section == LABELS) {
		LCD_DrawString(65, 10, "Room Tem(C): "); // tempt
		
		LCD_DrawString(90, 30, "Speed(%)"); // speed
		LCD_DrawLine(20, 50, 220, 50, BLUE); // frame top y50
		LCD_DrawLine(20, 65, 220, 65, BLUE); // frame bottom y65
		LCD_DrawLine(20, 50, 20, 65, BLUE); // frame left x20
		LCD_DrawLine(220, 50, 220, 65, BLUE); // frame right x220
		
		LCD_DrawString(105, 90, "Mode"); // op_mode
		LCD_DrawString(20, 105, "Off");
		LCD_DrawString(100, 105, "Auto");
		LCD_DrawString(175, 105, "Manual");
		
		LCD_DrawString(100, 150, "Silent"); // silent_mode
		LCD_DrawString(50, 170, "Off");
		LCD_DrawString(170, 170, "On");
		
		LCD_DrawString(95, 200, "Counter"); // counter
	}
	
	else if (section == TEMPT) {
		char tempt_buffer[10]; // tempt
		sprintf(tempt_buffer, "%g", temperature);
		LCD_DrawString(170, 10, tempt_buffer);
	} 
	
	else if (section == SPEED) {
		int power_bar_length = 200*current_power_pctg;
		for (int i = 0; i < 15; ++i) // fill frame
			LCD_DrawLine(20, 50+i, 20+power_bar_length, 50+i, BLUE);
	}
	
	else if (section == OP_MODE) {
		if (op_mode == 0)
			LCD_DrawLine(18, 121, 43, 121, BLUE); // off underline
		else if (op_mode == 1)
			LCD_DrawLine(98, 122, 132, 122, BLUE); // auto underline
		else if (op_mode == 2)
			LCD_DrawLine(173, 123, 224, 123, BLUE); // manual underline
	}
	
	else if (section == SILENT_MODE) {
		if (silent_mode_enable == 0) 
			LCD_DrawLine(48, 186, 73, 186, BLUE); // off underline
		else 
			LCD_DrawLine(168, 187, 187, 187, BLUE); // on underline	
	}
	
	else if (section == COUNTER) {
		
		
	}
	
	return;
}

void screen_clear(enum display_section section) {
	switch (section) {
		case TEMPT:
			LCD_Clear(170, 10, 20, 20, WHITE);
			break;
		case SPEED:
			LCD_Clear(21, 51, 199, 14, WHITE);
			break;
		case OP_MODE:
			LCD_Clear(18, 121, 207, 3, WHITE);
			break;
		case SILENT_MODE:
			LCD_Clear(48, 186, 140, 2, WHITE);
			break;
		case COUNTER:
			break;
		default:
			LCD_Clear(0, 0, 240, 320, WHITE);
	}
	return;
}


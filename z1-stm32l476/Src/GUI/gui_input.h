#ifndef _GUI_INPUT_H
#define _GUI_INPUT_H
#include "type.h"
	
//参数声明
extern uint8 user_pwd_nb;
extern uint32 user_pwd_refresh_nb;
extern uint8 user_pwd_input_enter;


	
	
//输入操作员密码
void GUI_userpwd_input(void);

//输入金额
uint8 GUI_money_input(uint8* money);

#endif
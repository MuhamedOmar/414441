/*
 * Timer0.c
 *
 * Created: Wednesday-11-Jan 6:17:01 PM
 *  Author: mohanad1
 */ 
#include "Timer0.h"
////////////////////////////////////////////////////////////////
void TIMER0_Init(timer0_configuration_struct STRUCT_V_G_T0_config)	/*This Function Sets and initialize mode of operation and compare out. mode and initial values of Timer0/Counter0 Register & Output Compare Reg.*/
{
		//The FOC0 bit is only active when the WAVEFORM_GENERATION_MODE_BIT_ZERO bit specifies a non-PWM mode.
		/*	if (STRUCT_V_G_T0_config.pwm_mode == 1)//T0_PWM_MODE = 1
			{
				CLEAR_BIT(TIMER0_CONTROL_REGISTER,FOC0);	//FOC0 must be set to zero when TIMER0_CONTROL_REGISTER is written when operating in PWM mode.
			}	*/
	TIMER0_Reset();
	switch(STRUCT_V_G_T0_config.mode_of_operation)
	{
		case T0_NORMAL_MODE://T0_NORMAL_MODE=0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,WAVEFORM_GENERATION_MODE_BIT_ZERO);//0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,WAVEFORM_GENERATION_MODE_BIT_ONE);//0			
			break;
		case T0_PHASE_CORRECT_PWM_MODE://T0_PHASE_CORRECT_PWM_MODE=1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,WAVEFORM_GENERATION_MODE_BIT_ONE);//0
			SET_BIT(TIMER0_CONTROL_REGISTER,WAVEFORM_GENERATION_MODE_BIT_ZERO);//1
			break;
		case T0_CLEAR_TIMER_ON_COMPARE_MATCH_MODE://T0_CLEAR_TIMER_ON_COMPARE_MATCH_MODE=2
			SET_BIT(TIMER0_CONTROL_REGISTER,WAVEFORM_GENERATION_MODE_BIT_ONE);//1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,WAVEFORM_GENERATION_MODE_BIT_ZERO);//0
			break;
		case T0_FAST_PWM_MODE://T0_FAST_PWM_MODE=3
			SET_BIT(TIMER0_CONTROL_REGISTER,WAVEFORM_GENERATION_MODE_BIT_ONE);//1
			SET_BIT(TIMER0_CONTROL_REGISTER,WAVEFORM_GENERATION_MODE_BIT_ZERO);//1
			break;		
	}
	switch(STRUCT_V_G_T0_config.compare_output_mode)
	{
		case T0_OC0_DISCONNECTED://T0_OC0_DISCONNECTED=0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,COMPARE_OUTPUT_MODE_BIT_ONE);//0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,COMPARE_OUTPUT_MODE_BIT_ZERO);//0
			break;
		case T0_TOGGLE_OC0://T0_TOGGLE_OC0=1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,COMPARE_OUTPUT_MODE_BIT_ONE);//0
			SET_BIT(TIMER0_CONTROL_REGISTER,COMPARE_OUTPUT_MODE_BIT_ZERO);//1
			break;
		case T0_CLEAR_OC0://T0_CLEAR_OC0=2
			SET_BIT(TIMER0_CONTROL_REGISTER,COMPARE_OUTPUT_MODE_BIT_ONE);//1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,COMPARE_OUTPUT_MODE_BIT_ZERO);//0
			break;
		case T0_SET_OC0://T0_SET_OC0=3
			SET_BIT(TIMER0_CONTROL_REGISTER,COMPARE_OUTPUT_MODE_BIT_ONE);//1
			SET_BIT(TIMER0_CONTROL_REGISTER,COMPARE_OUTPUT_MODE_BIT_ZERO);//1
			break;
	}
	TIMER0_COUNTER_REGISTER=STRUCT_V_G_T0_config.U8_TIMER0_COUNTER_REGISTER_value;
	TIMER0_COMPARE_OUTPUT_REGISTER=STRUCT_V_G_T0_config.U8_TIMER0_COMPARE_OUTPUT_REGISTER_value;	
	switch (STRUCT_V_G_T0_config.interrupt_mode)
	{
		case T0_INTERRUPT_OVF:
			SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_OVERFLOW_INTERRUPT);//timer0/counter0 overflow interrupt enable
			break;
		case T0_INTERRUPT_OCR:
			SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_COMPARE_INTERRUPT);//timer0/counter0 output compare match interrupt enable
			break;
		case T0_INTERRUPT_OVF_AND_OCR:
			SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_COMPARE_INTERRUPT);//timer0/counter0 output compare match interrupt enable
			SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_OVERFLOW_INTERRUPT);//timer0/counter0 overflow interrupt enable
			break;
	}
	return;
}
//////////////////////////////////////////////////////////////////////
void TIMER0_Start(void)	/*This Function Sets/Starts the counter-clock source and continue counting after stop*/
{		
	switch (STRUCT_V_G_T0_config.clock_select)
	{
		case T0_NO_CLOCK_SOURCE://T0_NO_CLOCK_SOURCE = 0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//0			
			break;
		case T0_CLK_NO_PRESCALE://T0_CLK_NO_PRESCALE = 1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//0
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//1
			break;
		case T0_CLK_8://.T0_CLK_8 = 2
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//0
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//0
			break;
		case T0_CLK_64://.T0_CLK_64 = 3
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//0
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//1
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//1
			break;
		case T0_CLK_256://.T0_CLK_256 = 4
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//0
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//0
			break;
		case T0_CLK_1024://.T0_CLK_1024 = 5
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//0
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//1
			break;
		case T0_EXTERNAL_CLK_FALLING://.T0_EXTERNAL_CLK_FALLING = 6
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//1
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//1
			CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//0
			break;
		case T0_EXTERNAL_CLK_RISING://.T0_EXTERNAL_CLK_RISING = 7
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//1
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//1
			SET_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//1		
			break;	
	}	
	return;
}
//////////////////////////////////////////////////////////////////////
void TIMER0_Stop(void)	/*This Function stops the counter from counting till further notice holding its current state --No Clock Source applied--*/
{		
	CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//0
	CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//0
	CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//0 no clock source
	return;
}
//////////////////////////////////////////////////////////////////////
void TIMER0_Clear(void)	/*This Function clears the counter to its initial  TIMER0_COUNTER_REGISTER and TIMER0_COMPARE_OUTPUT_REGISTER Values*/
{		
	TIMER0_COUNTER_REGISTER=STRUCT_V_G_T0_config.U8_TIMER0_COUNTER_REGISTER_value;
	TIMER0_COMPARE_OUTPUT_REGISTER=STRUCT_V_G_T0_config.U8_TIMER0_COMPARE_OUTPUT_REGISTER_value;
	return;	
}
//////////////////////////////////////////////////////////////////////
void TIMER0_Reset(void)	/*This Function resets the timer0 module registers and flags to its reset Values*/
{
	CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ZERO);	//0 no clk
	CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_ONE);	//0
	CLEAR_BIT(TIMER0_CONTROL_REGISTER,TIMER0_CLOCK_SELECT_BIT_TWO);	//0
	CLEAR_BIT(TIMER0_INTERRUPT_FLAG_REGISTER,TIMER0_OUTPUT_COMPARE_INTERRUPT_FLAG);//clear output compare interrupt flag
	CLEAR_BIT(TIMER0_INTERRUPT_FLAG_REGISTER,TIMER0_OVERFLOW_INTERRUPT_FLAG);//clear overflow interrupt flag
	
	CLEAR_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_COMPARE_INTERRUPT);//timer0/counter0 output compare match interrupt disabled
	CLEAR_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_OVERFLOW_INTERRUPT);//timer0/counter0 overflow interrupt disabled
	
	SET_BIT(TIMER0_SPECIAL_FUNCTION_IO_REGISTER,TIMER0_PRESCALER_RESET);//Pre-scaler reset timer(0,1)/counter(0,1)	
	TIMER0_COUNTER_REGISTER=0x00;
	TIMER0_COMPARE_OUTPUT_REGISTER=0x00;
	return;
}
//////////////////////////////////////////////////////////////////////
/*This function sets Pre-scaler to achieve the desired time per ovf given on condition counting from 0 to 255 --the full range of TIMER0_COUNTER_REGISTER--*/
/*
void TIMER0_Time_To_OVF(double t_ovf)
{		
	double time_per_tick=0;
	INT8U number_of_ticks=0;
	time_per_tick= (STRUCT_V_G_T0_config.clock_select)/F_CPU;//if F_CPU = 8M --> 1/8(0.125us), 8/8(1us), 64/8(8us), 256/8(32us), 1024/8(128us)
	number_of_ticks=(INT8U)t_ovf/time_per_tick;//1-->256
	switch (STRUCT_V_G_T0_config.clock_select)
	{
		case T0_CLK_NO_PRESCALE:
			if(number_of_ticks<=256){
			break;
			}
		case
	}	
return;	
}
*/
//////////////////////////////////////////////////////////////////////
void TIMER0_Set_TIMER0_COMPARE_OUTPUT_REGISTER_Val(INT8U val)	/*This function Set a new value to the  timer0  Output Compare Register*/
{
	STRUCT_V_G_T0_config.U8_TIMER0_COMPARE_OUTPUT_REGISTER_value=val;
	TIMER0_COMPARE_OUTPUT_REGISTER= STRUCT_V_G_T0_config.U8_TIMER0_COMPARE_OUTPUT_REGISTER_value;
	return;
}
//////////////////////////////////////////////////////////////////////
void TIMER0_Set_TIMER0_COUNTER_REGISTER_Val(INT8U val)
{
	STRUCT_V_G_T0_config.U8_TIMER0_COUNTER_REGISTER_value=val;
	TIMER0_COUNTER_REGISTER= STRUCT_V_G_T0_config.U8_TIMER0_COUNTER_REGISTER_value;
	return;
}
//////////////////////////////////////////////////////////////////////
void TIMER0_Enable_OVF_INT()	/*This function enable timer0 overflow interrupt*/
{
SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_OVERFLOW_INTERRUPT);//timer0/counter0 overflow interrupt enabled	
return;
}
//////////////////////////////////////////////////////////////////////
void TIMER0_Enable_OCR_INT()	/*This function enable timer0 output compare match interrupt*/
{
	SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_COMPARE_INTERRUPT);//timer0/counter0 output compare match interrupt enabled
	return;
}
//////////////////////////////////////////////////////////////////////
void TIMER0_Disable_OVF_INT()	/*This function disable timer0 overflow interrupt*/
{
	CLEAR_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_COMPARE_INTERRUPT);//timer0/counter0 output compare match interrupt disabled
	return;
}
//////////////////////////////////////////////////////////////////////
/*This function disable timer0 output compare match interrupt*/
void TIMER0_Disable_OCR_INT()
{
	CLEAR_BIT(TIMER0_INTERRUPT_MASK_REGISTER,TIMER0_OVERFLOW_INTERRUPT);//timer0/counter0 output compare match interrupt disabled
	return;
}
//////////////////////////////////////////////////////////////////////
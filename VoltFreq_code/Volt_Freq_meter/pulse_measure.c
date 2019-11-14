#include "pulse_measure.h"
#include "timer.h"

uint8  g_edgeCount = 0;

volatile uint32 dutyCycle = 0,frequency = 0,	period = 0, mili_volt = 0;
uint16 g_timeHigh  = 0, g_timePeriod = 0, g_timePeriodPlusHigh = 0;

TIMER_ConfigType  Icu_Config = {TIMER1_ICU,F_CPU_CLOCK,RISING};

void pulse_init(void){   //measure
	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	TIMER1_CallBack (APP_edgeProcessing);
	/* Create configuration structure for ICU driver */
	/* Set the Call back function pointer in the ICU driver */
	TIMER1_init(&Icu_Config);
}

uint8 get_pulse_read(){

	if(g_edgeCount == 4)
	{
		TIMER_Deinit(TIMER1); /* Disable ICU Driver */
		g_edgeCount = 0;

		/* calculate the freq */
		frequency = 1000000 / (g_timePeriodPlusHigh - g_timeHigh);
		/* calculate the duty cycle */
		dutyCycle = ((float)(g_timePeriodPlusHigh-g_timePeriod) / (g_timePeriodPlusHigh - g_timeHigh)) * 100;
		/* calculate the period */
		period = ((g_timePeriodPlusHigh - g_timeHigh) / 1000);
		//to turn it to mili volt on 5 volt reference
		mili_volt = dutyCycle*(5*10) ;
		
		return 1;  //true
	}
	//Icu_Rerun(F_CPU_CLOCK);
	return 0;
}


void APP_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		TIMER_Clear(TIMER1);
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
	else if(g_edgeCount == 3)
	{
		/* Store the Period time value */
		g_timePeriod = Icu_getInputCaptureValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 4)
	{
		//g_edgeCount=0;
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = Icu_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		TIMER_Clear(TIMER1);
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
}

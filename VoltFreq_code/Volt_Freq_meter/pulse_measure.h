#ifndef PULSE_H_
#define PULSE_H_

#include "std_types.h"
#include "std_macros.h"
#include "std_micro_comp_conf.h"

extern volatile uint32 dutyCycle, frequency ,	period , mili_volt ;


void   pulse_init(void);
uint8 get_pulse_read();
void APP_edgeProcessing(void);

#endif

/*
 * odroidu2-fan.c
 *
 *  Created on: May 9, 2013
 *      Author: mdrjr
 */


#include <stdio.h>
#include <string.h>

#include "odroidu2-fan.h"
#include "fileoperations.h"

void u2_service(void) {

	openfiles();

	int temperature = gettemp();

	/*
	 * For U2 FAN PWM it starts on 0 and max value is 255
	 * We'll also use kernel based parameters of the tmu to handle this.
	 * Since we know that the first throttle happens at 85C, 85C is a temperature that we don't want.
	 * we'll set our max speed temp as 80C
	 */

	int target_pwm_value = 255 * temperature / 80;

	if(target_pwm_value <= 0) {
		target_pwm_value = 0;
	}

	if(target_pwm_value > 255) {
		target_pwm_value = 255;
	}

	setfan(target_pwm_value);

	closefiles();
}

int main(int argc, char *argv[]) {

	if(fork() == 0) {
		while(1) {
			sleep(PROBES_PER_SECOND);
			u2_service();
		}
	}

	return 0;
}

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

int openfiles() {
	fp_tmu = fopen(TMU_PATH, "r");
	if(!fp_tmu) {
		printf("odroidu2-fan: cannot open tmu file: %s\n", TMU_PATH);
		return 10;
	}
	fp_fan = fopen(FAN_PATH, "w");
	if(!fp_fan) {
		printf("odroidu2-fan: cannot open fan file: %s\n", FAN_PATH);
		return 11;
	}
	return 0;
}

int main(int argc, char *argv[]) {

	if(openfiles() != 0) {
		return -1;
	}

	setfan(10);

/*	if(fork() == 0) {
		while(1) {
			sleep(1);
			printf("teste\n");
		}
	}*/

	return 0;
}

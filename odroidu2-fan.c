/*
 * odroidu2-fan.c
 *
 *  Created on: May 9, 2013
 *      Author: mdrjr
 */


#include <stdio.h>

#include "odroidu2-fan.h"

int openfiles(char *argv[]) {
	fp_tmu = fopen(TMU_PATH, "r");
	if(!fp_tmu) {
		printf("%s: cannot open tmu file: %s\n", argv[0], TMU_PATH);
		return 10;
	}
	fp_fan = fopen(FAN_PATH, "r");
	if(!fp_fan) {
		printf("%s: cannot open fan file: %s\n", argv[0], FAN_PATH);
		return 11;
	}
	return 0;
}

char * gettemp() {
	char *ret;

	return ret;
}

int setfan(int temperature) {

	return 0;
}

int main(int argc, char *argv[]) {

	if(openfiles(argv) != 0) {
		return -1;
		exit(-1);
	}

/*	if(fork() == 0) {
		while(1) {
			sleep(1);
			printf("teste\n");
		}
	}*/

	return 0;
}

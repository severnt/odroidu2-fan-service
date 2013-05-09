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

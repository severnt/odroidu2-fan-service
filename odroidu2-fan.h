/*
 * odroidu2-fan.c
 *
 *  Created on: May 9, 2013
 *      Author: mdrjr
 */

#define	TMU_PATH "tmu"
#define FAN_PATH "fan"

/*
 * Create both file handlers, one for the TMU and one for the FAN
 */
FILE *fp_tmu;
FILE *fp_fan;

int gettemp() {
	int ret;
	fscanf(fp_tmu, "%d", &ret);
	rewind(fp_tmu);
	return ret;
}

void setfan(int temperature) {
	char to_write[3];
	sprintf(to_write, "%d", temperature);
	printf("%s", to_write);
}

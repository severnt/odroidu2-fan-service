/*
 * fileoperations.h
 *
 *  Created on: May 9, 2013
 *      Author: mdrjr
 */

#ifndef FILEOPERATIONS_H_
#define FILEOPERATIONS_H_

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
	sprintf(to_write, "%d\n", temperature);
	fputs(to_write, fp_fan);
}



#endif /* FILEOPERATIONS_H_ */

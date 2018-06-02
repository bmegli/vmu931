/*
 * vmu931-calibrate example for vmu931 library
 *
 * Copyright 2018 (C) Bartosz Meglicki <meglickib@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. 
 *
 */
 
 /*
  * This example:
  * - initilies communication with VMU931
  * - calibrates the devices and outputs calibration status
  * - cleans after itself
  * 
  * Program expects terminal device, e.g.
  * 
  * ./vmu931-calibrate /dev/ttyACM0
  * 
  *
  */

#include "../vmu931.h"

#include <stdio.h> //printf
#include <stdlib.h> //exit

void usage(char **argv);

int main(int argc, char **argv)
{
	struct vmu *vmu=NULL;
	struct vmu_text text;
	const char *tty_device=argv[1];
	int ret;
	
	if(argc != 2)
	{
		usage(argv);
		return 0;
	}
	
	if( (vmu=vmu_init(tty_device)) == NULL)
	{
		perror(	"unable to initialize VMU931\n\n"
				"hints:\n"
				"- it takes a few seconds after plugging in to initialize device\n"
				"- make sure you are using correct tty device (dmesg after plugging vmu)\n"
				"- if all else fails unplug/plug VMU931\n\n"
				"error details");
		return 1;
	}	
	
	ret=vmu_calibrate(vmu, &text);
	
	if(ret == VMU_OK)	
		printf("Calibration succeded\n");
	else if(ret == VMU_ERROR)
	{
		printf("Calibration failed: \n\n"
		"hints:\n"
		"- device should remain still\n"
		"- Z axis should point upwards\n"
		"- keep away from strong magnetic fields\n\n");
		perror("Additional error information: ");
	}
	
	printf("Messages returned by the device:\n%s", text.text);	
	
	vmu_close(vmu);
	
	return 0;
}

void usage(char **argv)
{
	printf("Usage:\n");
	printf("%s tty_device\n\n", argv[0]);
	printf("examples:\n");
	printf("%s /dev/ttyACM0\n", argv[0]);
}

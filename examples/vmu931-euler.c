/*
 * vmu931-euler example for vmu931 library
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
  * - enables (only) euler data stream
  * - reads & prints data from VMU931 1000 times (+- 5 seconds)
  * - cleans after itself
  * 
  * Program expects terminal device, e.g.
  * 
  * ./vmu931-euler /dev/ttyACM0
  * 
  *
  */

#include "../vmu931.h"

#include <stdio.h> //printf
#include <stdlib.h> //exit

void usage(char **argv);

const int EULER_DATA_SIZE=10; 
const int MAX_READS=1000;


int main(int argc, char **argv)
{
	struct vmu *vmu=NULL;
	struct vmu_txyz euler_data[EULER_DATA_SIZE];
	const char *tty_device=argv[1];
	int ret, reads=0;
	
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
	
	if( vmu_stream(vmu, VMU_STREAM_EULER) == VMU_ERROR )
	{
		perror("failed to stream euler data");
		exit(1);
	}
		
	while( (ret=vmu_euler(vmu, euler_data, EULER_DATA_SIZE)) != VMU_ERROR )
	{
		//ret greter than EULER_DATA_SIZE indicates there is more data pending without blocking 
		for(int i=0; i<EULER_DATA_SIZE && i<ret;++i)
			printf("t=%u x=%f y=%f z=%f\n", euler_data[i].timestamp_ms, euler_data[i].x, euler_data[i].y, euler_data[i].z);
		
		//terminate after reading MAX_READS times
		//remove those lines if you want to read infinitely
		if(++reads >= MAX_READS) 
			break;
	}
		
	if(ret == VMU_ERROR)
		perror("failed to read from VMU931");
	else
		printf("success reading from VMU931, bye...\n");
	
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

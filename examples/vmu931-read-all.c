/*
 * vmu931-read-all example for vmu931 library
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
  * - enables euler, magnetometer and accelerometer data streams
  * - reads & prints data from VMU931 1000 times (+- 5 seconds)
  * - cleans after itself
  * 
  * Program expects terminal device, e.g.
  * 
  * ./vmu931-read-all /dev/ttyACM0
  * 
  *
  */

#include "../vmu931.h"

#include <stdio.h> //printf
#include <stdlib.h> //exit

void usage(char **argv);

const int DATA_SIZE=10; 
const int MAX_READS=1000;


int main(int argc, char **argv)
{
	struct vmu *vmu=NULL;
	// suppose we are intersted in euler, magnetometer and accelerometer
	// note that quaternions and heading use different data types
	struct vmu_txyz euler_data[DATA_SIZE];
	struct vmu_txyz mag_data[DATA_SIZE];
	struct vmu_txyz accel_data[DATA_SIZE];

	struct vmu_data data={0}; //the library will return data here and note number of read values in data.size
	struct vmu_size size={0}; //this notes sizes of our arrays, data.size is refreshed with this value before read

	size.accel = size.mag = size.euler = DATA_SIZE;	

	data.euler = euler_data;
	data.mag = mag_data;
	data.accel = accel_data;
	data.size = size;
		
	const char *tty_device=argv[1];
	int ret, reads=0;
	
	if(argc != 2)
	{
		usage(argv);
		return EXIT_SUCCESS;
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
	
	if( vmu_stream(vmu, VMU_STREAM_EULER | VMU_STREAM_MAG | VMU_STREAM_ACCEL) == VMU_ERROR )
	{
		perror("failed to stream euler/mag/accel data");
		exit(EXIT_FAILURE);
	}
		
	while( (ret=vmu_read_all(vmu, &data)) != VMU_ERROR )
	{
		for(int i=0;i<data.size.euler;++i)
			printf("[euler] t=%u x=%f y=%f z=%f\n", data.euler[i].timestamp_ms, data.euler[i].x, data.euler[i].y, data.euler[i].z);

		for(int i=0;i<data.size.mag;++i)
			printf("[mag] t=%u x=%f y=%f z=%f\n", data.mag[i].timestamp_ms, data.mag[i].x, data.mag[i].y, data.mag[i].z);

		for(int i=0;i<data.size.accel;++i)
			printf("[accel] t=%u x=%f y=%f z=%f\n", data.accel[i].timestamp_ms, data.accel[i].x, data.accel[i].y, data.accel[i].z);
		
		//terminate after reading MAX_READS times
		//remove those lines if you want to read infinitely
		if(++reads >= MAX_READS) 
			break;
			
		//refresh the sizes of the arrays for data streams
		data.size=size;
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

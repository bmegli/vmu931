/*
 * vmu931-estimate-bias example for vmu931 library
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
  * - reads & prints data from VMU931 in csv format (redirect to file)
  * - estimates bias (offset from initial angles)
  * - prints the estimated bias
  * - cleans after itself
  * 
  * Program expects terminal device and number of seconds to run, e.g.
  * 
  * ./vmu931-estimate-bias /dev/ttyACM0 10
  * 
  * Optionally redirect output to file and leave only the csv data part for futher processing:
  * 
  *./vmu931-estimate-bias /dev/ttyACM0 60 > data.csv 
  */

#include "../vmu931.h"

#include <stdio.h> //printf
#include <stdlib.h> //exit, atof
#include <time.h> //time_t, time, difftime 

void usage(char **argv);

const int EULER_DATA_SIZE=10; 

int main(int argc, char **argv)
{
	struct vmu *vmu=NULL;
	struct vmu_txyz euler_data[EULER_DATA_SIZE], euler_first, euler_last;
	double estimation_seconds, per_hour_factor;
	int ret, first_recorded=0, last_recorded_index=0;
	const char *tty_device=argv[1];
	
	if(argc != 3)
	{
		usage(argv);
		return 0;
	}
	
	estimation_seconds=atof(argv[2]);

	if(estimation_seconds<=0.0)
	{
		fprintf(stderr, "Incorrect number of seconds specified (has to be >= 0)\n");
		return 1;
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
	
	time_t start_time=time(NULL);

	if( vmu_stream(vmu, VMU_STREAM_EULER) == VMU_ERROR )
	{
		perror("failed to stream euler data");
		vmu_close(vmu);
		return 1;
	}
		
	printf("--------------- start of .csv data ----------------\n");

	printf("t;x;y;z\n");
	
	while( (ret=vmu_euler(vmu, euler_data, EULER_DATA_SIZE)) != VMU_ERROR )
	{
		for(int i=0; i<EULER_DATA_SIZE && i<ret;++i)
			printf("%u;%f;%f;%f\n", euler_data[i].timestamp_ms, euler_data[i].x, euler_data[i].y, euler_data[i].z);
		
		if(!first_recorded && ret>0)
		{
			euler_first=euler_data[0];
			first_recorded=1;
		}
		
		if(ret>0)
			last_recorded_index=ret-1;
			
		if(difftime(time(NULL), start_time) > estimation_seconds)
			break;
	}

	printf("--------------- end of .csv data ----------------\n\n");
		
	if(ret == VMU_ERROR)
	{
		perror("Failed to read from VMU931");
		vmu_close(vmu);
		return 1;
	}
	
	if(first_recorded==0)
	{
		fprintf(stderr, "Failed to read data from VMU931\n");
		vmu_close(vmu);
		return 1;
	}
	
	//now calculate the stats
	if(last_recorded_index >= EULER_DATA_SIZE)
		last_recorded_index = EULER_DATA_SIZE-1;

	euler_last=euler_data[last_recorded_index];
	per_hour_factor= 3600.0 / estimation_seconds; //3600 seconds in hour
		
	printf("Bias estimation results:\n");
	printf("-after %f seconds\n", estimation_seconds);
	printf("-elapsed %d ms in device clock\n", euler_last.timestamp_ms-euler_first.timestamp_ms);
	printf("-bias on X=%f, Y=%f, Z=%f\n", 	euler_last.x-euler_first.x,
											euler_last.y-euler_first.y,
											euler_last.z-euler_first.z);
	printf("-bias per hour on X=%f, Y=%f, Z=%f\n\n", 	(euler_last.x-euler_first.x)*per_hour_factor,
													(euler_last.y-euler_first.y)*per_hour_factor,
													(euler_last.z-euler_first.z)*per_hour_factor);

	printf("Notes:\n");
	printf("- the calculation is simple: end value - start value\n");
	printf("- calculation doesn't take into account \"wrap around\"!\n"
			"  (e.g. -180, 180 for Z axis)\n");
	printf("- the result is only correct if no \"wrap around\" happened \n");
	printf("- in case you need more insight redirect to file and plot the result\n");
		
	vmu_close(vmu);
	
	return 0;
}

void usage(char **argv)
{
	printf("Usage:\n");
	printf("%s tty_device seconds\n\n", argv[0]);
	printf("examples:\n");
	printf("%s /dev/ttyACM0 10\n", argv[0]);
	printf("%s /dev/ttyACM0 10 > some_file.csv\n", argv[0]);
}

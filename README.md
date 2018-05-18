# vmu931

This is C library for communication with Variense [VMU931](https://variense.com/product/vmu931/) inertial measurment unit.

## Platforms 

Library works on Unix platforms (e.g. Linux).

## Hardware & Firmware

Library was written for [VMU931](https://variense.com/product/vmu931/) and tested with [firmware 1.0.1](https://variense.com/downloads/)

## Library design

Goals:
- retrieve readings history (not only the latest reading)
- convinient interface for simple scenarios (e.g. grab readings in simple `while` loop)
- usable in advanced scenarios (e.g. synchronous I/O multiplexing like `select`)

## Scope

Library implements full functionality as defined in [VMU931 User Guide 1.3](http://variense.com/Docs/VMU931/VMU931_UserGuide.pdf) for [firmware 1.0.1](https://variense.com/downloads/).

This includes:
- enabling/disabling sensor streams
- setting resolution for gyroscope and accelerometer
- reading data from the device
- selftest and calibration routines
- retrieving device status

## Building Instructions

### Compilers and make

``` bash
$ sudo apt-get update
$ sudo apt-get install build-essential 
```

### Getting git

``` bash
sudo apt-get install git
```

### Cloning the repository

``` bash
git clone https://github.com/bmegli/vmu931.git
```

### Building the examples

``` bash
cd vmu931
make all
```

## Testing

Run `vmu931-euler` with you device, e.g.: 

```bash
./vmu931-euler /dev/ttyACM0
```

## Using

See examples directory for more complete and commented examples with error handling.

```bash
int ret;
struct vmu *vmu=NULL;
struct vmu_txyz euler_data[10];

vmu=vmu_init("/dev/ttyACM0");
vmu_stream(vmu, VMU_STREAM_EULER);

while( (ret=vmu_euler(vmu, euler_data, EULER_DATA_SIZE)) != VMU_ERROR )
{
	for(int i=0; i<10 && i<ret;++i)
		printf("t=%u x=%f y=%f z=%f\n",	euler_data[i].timestamp_ms,
						euler_data[i].x,
						euler_data[i].y,
		 				euler_data[i].z);
}

vmu_close(vmu);

```

### Compiling your code

C
``` bash
gcc vmu931.c your_program.c -o your-program
```

C++
``` bash
gcc -c vmu931.c
g++ -c your_program.cpp
g++ vmu931.o your_program.o -o your-program
```

## License

Library is licensed under Mozilla Public License, v. 2.0

This is similiar to LGPL but more permissive:

- you can use it as LGPL in prioprietrary software
- unlike LGPL you may compile it statically with your code

Like in LGPL, if you modify this library, you have to make your changes available. Making a github fork of the library with your changes satisfies those requirements perfectly.

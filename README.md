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

TO DO (examples not included yet)

``` bash
cd vmu931
make all
```

## Testing

TO DO (examples not included yet)

## Using

TO DO 

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

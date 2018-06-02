# vmu931

This is C library for communication with Variense [VMU931](https://variense.com/product/vmu931/) inertial measurment unit.

## Platforms 

Library works on Unix platforms (e.g. Linux).

## Hardware & Firmware

Library was written for [VMU931](https://variense.com/product/vmu931/) and tested with [firmware 1.0.1](https://variense.com/downloads/)

## Scope

Library implements full functionality as defined in [VMU931 User Guide 1.3](http://variense.com/Docs/VMU931/VMU931_UserGuide.pdf) for [firmware 1.0.1](https://variense.com/downloads/).

| Functionality                      | Library functions                                                                      |
| -----------------------------------|----------------------------------------------------------------------------------------|
| init and cleanup                   | `vmu_init`, `vmu_close`                                                                |
| enabling/disabling sensor streams  | `vmu_stream`                                                                           |
| setting resolutions                | `vmu_resolution`                                                                       |
| reading data from the device       | `vmu_accel`, `vmu_gyro`, `vmu_mag`, `vmu_euler`, `vmu_quat`, `vmu_head`, `vmu_read_all`|
| selftest and calibration routines  | `vmu_selftest`, `vmu_calibrate`                                                        |
| device status                      | `vmu_status`                                                                           |

Library was designed to retrieve readings history (not only the latest reading).

Library [documentation](https://bmegli.github.io/vmu931/vmu931_8h.html).

## Building Instructions

``` bash
# compilers, make and git
sudo apt-get update
sudo apt-get install build-essential git

# clone the repository
git clone https://github.com/bmegli/vmu931.git

# build the examples
cd vmu931
make all
```

## Testing

Run `vmu931-euler` with your device, e.g.: 

```bash
./vmu931-euler /dev/ttyACM0
```

Run `vmu931-read-all` with your device, e.g.: 

```bash
./vmu931-read-all /dev/ttyACM0
```

## Using

See examples directory for more complete examples with error handling.

Library [documentation](https://bmegli.github.io/vmu931/vmu931_8h.html).

### Calibration

```C
struct vmu *vmu = NULL;
struct vmu_text text;

vmu = vmu_init("/dev/ttyACM0");
vmu_calibrate(vmu, &text);
vmu_close(vmu);
```

### Single stream (e.g. euler)

```C
int ret;
struct vmu *vmu = NULL;
struct vmu_txyz euler_data[10];

vmu = vmu_init("/dev/ttyACM0");
vmu_stream(vmu, VMU_STREAM_EULER);

while( (ret = vmu_euler(vmu, euler_data, EULER_DATA_SIZE) ) != VMU_ERROR )
{
   for(int i=0; i<10 && i<ret;++i)
      printf("t=%u x=%f y=%f z=%f\n",
         euler_data[i].timestamp_ms,
         euler_data[i].x,
         euler_data[i].y,
         euler_data[i].z);
}

vmu_close(vmu);

```

### Multiple streams (e.g. euler, magnetometer, accelerometer)

```C
struct vmu *vmu=NULL;
struct vmu_txyz euler_data[10];
struct vmu_txyz mag_data[10];
struct vmu_txyz accel_data[10];

struct vmu_data data={0};
struct vmu_size size={0};

size.accel = size.mag = size.euler = 10;	

data.euler = euler_data;
data.mag = mag_data;
data.accel = accel_data;
data.size = size;

vmu = vmu_init("/dev/ttyACM0");
vmu_stream(vmu, VMU_STREAM_EULER | VMU_STREAM_MAG | VMU_STREAM_ACCEL);

while( vmu_read_all(vmu, &data) != VMU_ERROR )
{
   for(int i=0;i<data.size.euler;++i)
      printf("[euler] t=%u x=%f y=%f z=%f\n", data.euler[i].timestamp_ms,
                                              data.euler[i].x,
                                              data.euler[i].y,
                                              data.euler[i].z);

   for(int i=0;i<data.size.mag;++i)
      printf("[mag] t=%u x=%f y=%f z=%f\n", data.mag[i].timestamp_ms,
                                            data.mag[i].x,
                                            data.mag[i].y,
                                            data.mag[i].z);

   for(int i=0;i<data.size.accel;++i)
      printf("[accel] t=%u x=%f y=%f z=%f\n", data.accel[i].timestamp_ms,
                                              data.accel[i].x,
                                              data.accel[i].y,
                                              data.accel[i].z);
			
   data.size=size;
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

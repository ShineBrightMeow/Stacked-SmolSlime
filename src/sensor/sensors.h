#ifndef SLIMENRF_SENSOR_SENSORS
#define SLIMENRF_SENSOR_SENSORS

#include <zephyr/kernel.h>

#include "sensor_none.h"
#include "sensor_ext.h"

#include "imu/BMI270.h"
#include "imu/ICM42688.h"
#include "imu/ICM45686.h"
#include "imu/LSM6DSV.h"
#include "imu/LSM6DSO.h"

#include "mag/AK09940.h"
#include "mag/BMM150.h"
#include "mag/BMM350.h"
#include "mag/LIS2MDL.h"
#include "mag/LIS3MDL.h"
#include "mag/MMC5983MA.h"

#include "scan.h"
#include "../sensor.h"
#include "sensors_enum.h"

const char *dev_imu_names[] = {
	"BMI160",
	"BMI270",
	"BMI323",
	"MPU-6000/MPU-6050",
	"MPU-6500",
	"MPU-9250",
	"ICM-20948",
	"ICM-42688-P/ICM-42688-V",
	"ICM-45686",
	"LSM6DSO16IS/ISM330IS",
	"LSM6DS3",
	"LSM6DS3TR-C/LSM6DSL/LSM6DSM/ISM330DLC",
	"LSM6DSR/ISM330DHCX",
	"LSM6DSO",
	"LSM6DST",
	"LSM6DSV",
	"LSM6DSV16B/ISM330BX"
};
const sensor_imu_t *sensor_imus[] = {
	&sensor_imu_none, // will not implement, too low quality
	&sensor_imu_bmi270,
	&sensor_imu_none,
	&sensor_imu_none, // cardinal sin
	&sensor_imu_none, // cardinal sin
	&sensor_imu_none, // cardinal sin
	&sensor_imu_none,
	&sensor_imu_icm42688,
	&sensor_imu_icm45686,
	&sensor_imu_none,
	&sensor_imu_none, // will not implement, too low quality
	&sensor_imu_none,
	&sensor_imu_lsm6dso, // compatible with driver
	&sensor_imu_lsm6dso,
	&sensor_imu_none,
	&sensor_imu_lsm6dsv,
	&sensor_imu_lsm6dsv // compatible with driver
};
const int i2c_dev_imu_addr_count = 2;
const uint8_t i2c_dev_imu_addr[] = {
	2,	0x68,0x69,
	2,	0x6A,0x6B
};
const uint8_t i2c_dev_imu_reg[] = {
	3,	0x00,
		0x72,
		0x75,
	1,	0x0F
};
const uint8_t i2c_dev_imu_id[] = {
	4,	0xEA,0xD1,0x24,0x43, // reg 0x00
	1,	0xE9, // reg 0x72
	5,	0x68,0x70,0x71,0x47,0xDB, // reg 0x75
	8,	0x22,0x69,0x6A,0x6B,0x6C,0x6D,0x70,0x71 // reg 0x0F
};
const int i2c_dev_imu[] = {
	IMU_ICM20948, IMU_BMI160, IMU_BMI270, IMU_BMI323,
	IMU_ICM45686,
	IMU_MPU6050, IMU_MPU6500, IMU_MPU9250, IMU_ICM42688, IMU_ICM42688, // ICM-42688-P, ICM-42688-V
	IMU_ISM330IS, IMU_LSM6DS3, IMU_LSM6DSM, IMU_LSM6DSR, IMU_LSM6DSO, IMU_LSM6DST, IMU_LSM6DSV, IMU_ISM330BX
};

const char *dev_mag_names[] = {
	"HMC5883L",
	"QMC5883L",
	"AK8963",
	"AK09916",
	"AK09940",
	"BMM150",
	"BMM350",
	"IIS2MDC/LIS2MDL",
	"LIS3MDL",
	"MMC34160PJ",
	"MMC3630KJ",
	"MMC5603NJ/MMC5633NJL",
	"MMC5616WA",
	"MMC5983MA"
};
const sensor_mag_t *sensor_mags[] = {
	&sensor_mag_none, // will not implement, too low quality
	&sensor_mag_none, // not implemented
	&sensor_mag_none,
	&sensor_mag_none,
	&sensor_mag_ak09940,
	&sensor_mag_bmm150,
	&sensor_mag_bmm350,
	&sensor_mag_lis2mdl,
	&sensor_mag_lis3mdl,
	&sensor_mag_none,
	&sensor_mag_none,
	&sensor_mag_none,
	&sensor_mag_none,
	&sensor_mag_mmc5983ma
};
const int i2c_dev_mag_addr_count = 8;
const uint8_t i2c_dev_mag_addr[] = {
	1,	0x0B,
	1,	0x0C,
	3,	0x0D,0x0E,0x0F,
	4,	0x10,0x11,0x12,0x13, // why bosch
	4,	0x14,0x15,0x16,0x17,
	1,	0x1C,
	1,	0x1E,
	1,	0x30
};
const uint8_t i2c_dev_mag_reg[] = {
	1,	0x0D,
	2,	0x01, // AK09916/AK09940 first
		0x00,
	2,	0x01, // AK09940 first
		0x00,
	1,	0x40,
	1,	0x00,
	1,	0x0F,
	3,	0x0F,
		0x4F,
		0x0A,
	3,	0x20,
		0x2F,
		0x39
};
const uint8_t i2c_dev_mag_id[] = {
	1,	0xFF, // reg 0x0D
	2,	0x09,0xA3, // reg 0x01
	1,	0x48, // reg 0x00
	1,	0xA3, // reg 0x01
	1,	0x48, // reg 0x00
	1,	0x32, // reg 0x40
	1,	0x33, // reg 0x00
	1,	0x3D, // reg 0x0F
	1,	0x3D, // reg 0x0F
	1,	0x40, // reg 0x4F
	1,	0x48, // reg 0x0A
	1,	0x06, // reg 0x20
	2,	0x0A,0x30, // reg 0x2F
	2,	0x10,0x11 // reg 0x39
};
const int i2c_dev_mag[] = {
	MAG_QMC5883L,
	MAG_AK09916, MAG_AK09940,
	MAG_AK8963,
	MAG_AK09940,
	MAG_AK8963,
	MAG_BMM150,
	MAG_BMM350,
	MAG_LIS3MDL,
	MAG_LIS3MDL,
	MAG_LIS2MDL,
	MAG_HMC5883L,
	MAG_MMC34160PJ,
	MAG_MMC3630KJ, MAG_MMC5983MA,
	MAG_MMC5633NJL, MAG_MMC5616WA
};

int sensor_scan_imu(struct i2c_dt_spec *i2c_dev, uint8_t *i2c_dev_reg)
{
	return sensor_scan(i2c_dev, i2c_dev_reg, i2c_dev_imu_addr_count, i2c_dev_imu_addr, i2c_dev_imu_reg, i2c_dev_imu_id, i2c_dev_imu);
}

int sensor_scan_mag(struct i2c_dt_spec *i2c_dev, uint8_t *i2c_dev_reg)
{
	return sensor_scan(i2c_dev, i2c_dev_reg, i2c_dev_mag_addr_count, i2c_dev_mag_addr, i2c_dev_mag_reg, i2c_dev_mag_id, i2c_dev_mag);
}

#endif
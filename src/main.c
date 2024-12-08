#include "globals.h"
#include "system.h"
//#include "timer.h"
#include "esb.h"
#include "sensor.h"

#include <zephyr/sys/reboot.h>
#include <zephyr/drivers/gpio.h>

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)
#if DT_NODE_HAS_PROP(ZEPHYR_USER_NODE, pwr_gpios)
static const struct gpio_dt_spec pwr = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, pwr_gpios);
static const struct gpio_dt_spec gnd = GPIO_DT_SPEC_GET(ZEPHYR_USER_NODE, gnd_gpios);

#else
#warning "IMU power pins not defined: do not stack IMU on SUPERMINI"
#endif
#define DFU_DBL_RESET_MEM 0x20007F7C
#define DFU_DBL_RESET_APP 0x4ee5677e

uint32_t* dbl_reset_mem = ((uint32_t*) DFU_DBL_RESET_MEM);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#if DT_NODE_HAS_PROP(DT_ALIAS(sw0), gpios)
#define BUTTON_EXISTS true
#endif

#define DFU_EXISTS CONFIG_BUILD_OUTPUT_UF2 || CONFIG_BOARD_HAS_NRF5_BOOTLOADER
#define ADAFRUIT_BOOTLOADER CONFIG_BUILD_OUTPUT_UF2

int main(void)
{
#if DT_NODE_HAS_PROP(ZEPHYR_USER_NODE, pwr_gpios)
    gpio_pin_configure_dt(&gnd, GPIO_OUTPUT_ACTIVE);
    gpio_pin_set_dt(&gnd, 0);
    gpio_pin_configure_dt(&pwr, GPIO_OUTPUT_ACTIVE);
    gpio_pin_set_dt(&pwr, 1);
#endif 
#if IGNORE_RESET && BUTTON_EXISTS
	bool reset_pin_reset = false;
#else
	bool reset_pin_reset = NRF_POWER->RESETREAS & 0x01;
#endif
	NRF_POWER->RESETREAS = NRF_POWER->RESETREAS; // Clear RESETREAS

//	start_time = k_uptime_get(); // Need to get start time for imu startup delay
	set_led(SYS_LED_PATTERN_ON, SYS_LED_PRIORITY_BOOT); // Boot LED

#if ADAFRUIT_BOOTLOADER && !(IGNORE_RESET && BUTTON_EXISTS) // Using Adafruit bootloader
	(*dbl_reset_mem) = DFU_DBL_RESET_APP; // Skip DFU
	ram_range_retain(dbl_reset_mem, sizeof(dbl_reset_mem), true);
#endif

	uint8_t reboot_counter = reboot_counter_read();
	bool booting_from_shutdown = !reboot_counter && (reset_pin_reset || button_read()); // 0 means from user shutdown or failed ram validation

	if (booting_from_shutdown)
		set_led(SYS_LED_PATTERN_ONESHOT_POWERON, SYS_LED_PRIORITY_BOOT);

	bool docked = dock_read();

	uint8_t reset_mode = -1;

	if (reboot_counter == 0)
		reboot_counter = 100;
	else if (reboot_counter > 200)
		reboot_counter = 200; // How did you get here
	reset_mode = reboot_counter - 100;
	if ((reset_pin_reset || button_read()) && !docked) // Count pin resets while not docked
	{
		reboot_counter++;
		reboot_counter_write(reboot_counter);
		LOG_INF("Reset count: %u", reboot_counter);
		k_msleep(1000); // Wait before clearing counter and continuing
	}
	reboot_counter_write(100);
	if (!reset_pin_reset && !button_read() && reset_mode == 0) // Only need to check once, if the button is pressed again an interrupt is triggered from before
		reset_mode = -1; // Cancel reset_mode (shutdown)

#if USER_SHUTDOWN_ENABLED
	bool charging = chg_read();
	bool charged = stby_read();
	bool plugged = vin_read();

	if (reset_mode == 0 && !booting_from_shutdown && !charging && !charged && !plugged) // Reset mode user shutdown, only if unplugged and undocked
		sys_user_shutdown();
#endif

	if (!booting_from_shutdown) // ONESHOT_POWERON automatically sets LED off
		set_led(SYS_LED_PATTERN_OFF, SYS_LED_PRIORITY_BOOT);

	sys_reset_mode(reset_mode);

	k_msleep(1); // TODO: fixes some weird issue with the device bootlooping, what is the cause

	clocks_start();

	esb_pair();

	esb_initialize(true);
	//timer_init();
// 1ms to start ESB

	return 0;
}

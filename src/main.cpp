#include <stdio.h>
#include "mgos.h"
#include "mgos_app.h"
#include "mgos_gpio.h"
#include "mgos_timers.h"
#include "mgos_mqtt.h"
#include "mgos_config.h"


// as pulseIn isn't supported in Mongoose Arduino compatability library yet, here's a local
// implementation of that. Full credit to "nliviu" on Mongoose OS forums for that
// https://forum.mongoose-os.com/discussion/1928/arduino-compat-lib-implicit-declaration-of-function-pulsein#latest
static inline uint64_t uptime()
{
    return (uint64_t)(1000000 * mgos_uptime());
}

uint32_t pulseInLongLocal(uint8_t pin, uint8_t state, uint32_t timeout)
{
    uint64_t startMicros = uptime();

    // wait for any previous pulse to end
    while (state == mgos_gpio_read(pin))
    {
        if ((uptime() - startMicros) > timeout)
        {
            return 0;
        }
    }

    // wait for the pulse to start
    while (state != mgos_gpio_read(pin))
    {
        if ((uptime() - startMicros) > timeout)
        {
            return 0;
        }
    }

    uint64_t start = uptime();

    // wait for the pulse to stop
    while (state == mgos_gpio_read(pin))
    {
        if ((uptime() - startMicros) > timeout)
        {
            return 0;
        }
    }
    return (uint32_t)(uptime() - start);
}

static void timer_cb(void *arg)
{
    //send trigger
    mgos_gpio_write(mgos_sys_config_get_app_gpio_trigger_pin(), 1);
    // wait 10 microseconds
    mgos_usleep(10);
    // stop the trigger
    mgos_gpio_write(mgos_sys_config_get_app_gpio_trigger_pin(), 0);

    // wait for response and calculate distance
    unsigned long duration = pulseInLongLocal(mgos_sys_config_get_app_gpio_echo_pin(), 1, mgos_sys_config_get_app_pulse_in_timeout_usecs());
    double distance = duration * 0.034 / 2;
    
    char strBuffer[64];
    snprintf(strBuffer, sizeof(strBuffer), "{\"report\":{\"distance\":%.2f}}\n", distance);

    printf(strBuffer);
    mgos_mqtt_pub(mgos_sys_config_get_app_mqtt_tank_level_topic(), strBuffer, strlen(strBuffer), 1, 0);  
    (void)arg;
}

enum mgos_app_init_result mgos_app_init(void)
{
    // set the modes for the pins
    mgos_gpio_set_mode(mgos_sys_config_get_app_gpio_trigger_pin(), MGOS_GPIO_MODE_OUTPUT);
    mgos_gpio_set_mode(mgos_sys_config_get_app_gpio_echo_pin(), MGOS_GPIO_MODE_INPUT);
    mgos_gpio_set_pull(mgos_sys_config_get_app_gpio_echo_pin(), MGOS_GPIO_PULL_UP);

    // Every x second, invoke timer_cb. 2nd arg means repeat continuously    
    mgos_set_timer(mgos_sys_config_get_app_sensor_read_interval_ms(), true, timer_cb, NULL);

    return MGOS_APP_INIT_SUCCESS;
}

#include "peripherals/hardwareprofile.h"
#include "peripherals/system.h"
#include "peripherals/display.h"
#include "lvgl.h"
#include "peripherals/timer.h"
#include "gel/timer/timecheck.h"
#include "peripherals/spi_devices.h"
#include "peripherals/touch.h"
#include "peripherals/digout.h"
#include "view/view.h"
#include "model/model.h"
#include "controller/controller.h"
#include "controller/gui.h"
#include "peripherals/digout.h"
#include "peripherals/digin.h"
#include "I2C/i2c_devices/eeprom/24LC1025/24lc1025.h"
#include "I2C/i2c_devices/rtc/M41T81/m41t81.h"
#include "I2C/i2c_devices/temperature/MCP9800/MCP9800.h"
#include "peripherals/i2c_devices.h"

static model_t pmodel;

int main (void) {
    unsigned long ts=0, touchts=0, digoutts=0,digints=0,read_digints=0;
    int blink=0;
    system_oscillator_config();
    system_gpio_init();
    init_display_driver();
    touch_init();
    timer_init();
    spi_devices_init();
    digin_filter_init();
    digout_update_retro(1);
    model_init(&pmodel);
    controller_init(&pmodel);
    view_init(&pmodel, display_flush_cb, display_rounder, display_set_pixel, touch_read);
    m41t81_init(rtc_driver);
    
    rtc_time_t time;
    
    uint8_t buff = 0;
    
    ee24lc1025_sequential_write(eeprom_driver, 0, 0x10, "0xAA", 1);
    ee24lc1025_sequential_read(eeprom_driver, 0, 0x10, &buff, 1);
    

    while (1) {
        controller_manage_gui(&pmodel);
        
        MCP9800_set_resolution(temperature_driver, MCP9800_12BIT);
        double temp = MCP9800_read_temperature(temperature_driver);
        
        if (is_expired(ts,get_millis(), 1000)) {
            HAP_RUN=blink;
            blink=!blink;
            ts=get_millis();
                m41t81_get_time(rtc_driver,&time);


        }
        if (is_expired(read_digints,get_millis(),5)) {
            digin_take_reading();
        }
        
        if (is_expired(touchts, get_millis(), 20)) {
            touch_manage();
            touchts = get_millis();
        } else if (is_expired(digoutts, get_millis(), 50)) {
            digout_period_check();
            digoutts = get_millis();
        } else if (is_expired(digints, get_millis(), 50)) {
            digin_exp_take_reading();
            digints = get_millis();
        }
    }
}

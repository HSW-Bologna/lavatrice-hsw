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
    
    unsigned int input = 0;
    

    while (1) {
        controller_manage_gui(&pmodel);
        
        input = digin_get_inputs();
       
        if (is_expired(ts,get_millis(), 1000)) {
            HAP_RUN=blink;
            blink=!blink;
            ts=get_millis();

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

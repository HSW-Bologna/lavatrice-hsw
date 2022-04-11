#include "peripherals/hardwareprofile.h"
#include "peripherals/system.h"
#include "peripherals/display.h"
#include "lvgl.h"
#include "peripherals/timer.h"
#include "gel/timer/timecheck.h"
#include "peripherals/spi_devices.h"
#include "peripherals/touch.h"
#include "peripherals/digout.h"


#define GBUFSIZE (240*128/8)
lv_color_t gbuf[GBUFSIZE];


void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
    // durata 0,39ms
    int row = area->y1, col;
    unsigned int address = (unsigned int)row * HW_COLUMNS + area->x1/8;
    unsigned char *buffer;
    int linelen = (area->x2 - area->x1)/8;
    
    buffer = (unsigned char*) color_p;
    
    for (row = area->y1; row <= area->y2; row++) {
        
        set_address_pointer(address);
        
//        for (col = area->x1/8; col <= area->x2/8; col++) {
//            vram_data_write(gbuf[(unsigned int) row * HW_COLUMNS + (unsigned int) col], TRUE);
//        }
        for (col = 0; col <= linelen; col++) {
            vram_data_write(buffer[col], 1);
        }
        buffer += linelen+1;
        address += HW_COLUMNS;
    }
    
    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}


 void lv_vdb_wr_1bpp_horiz(struct _lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa)
{
    buf += buf_w/8 * y;
    buf += x/8;
    if(lv_color_brightness(color) > 128) {(*buf) |= (1 << (7 - x % 8));}
    else {(*buf) &= ~(1 << (7 - x % 8));}
}


void rounder(struct _lv_disp_drv_t * disp_drv, lv_area_t *a)
{
    a->x1 = a->x1 & ~(0x7);
    a->x2 = a->x2 |  (0x7);
    a->x2 = a->x2 > 239 ? 239 : a->x2;
    a->y2 = a->y2 > 127 ? 127 : a->y2;
}

bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    data->point.x = Touch_Coord[0]; 
    data->point.y = Touch_Coord[1];
    data->state = f_touch_detected ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    return false; /*No buffering now so no more data read*/
}

int main (void) {
    unsigned long ts=0, touchts = 0;

    int blink=0;
    system_oscillator_config();
    system_gpio_init();
    init_display_driver();
    touch_init();
    
    timer_init();
    lv_init();
    spi_devices_init();
    digout_update_retro(1);
    
    /*A static or global variable to store the buffers*/
    static lv_disp_draw_buf_t disp_buf;

    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
    lv_disp_draw_buf_init(&disp_buf, gbuf, NULL, GBUFSIZE);
    
    static lv_disp_drv_t disp_drv;                     /*A variable to hold the drivers. Must be static or global.*/
    lv_disp_drv_init(&disp_drv);                       /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf;                     /*Set an initialized buffer*/
    disp_drv.flush_cb = my_flush_cb;                      /*Set a flush callback to draw to the display*/
    disp_drv.set_px_cb = lv_vdb_wr_1bpp_horiz;
    disp_drv.rounder_cb = rounder;
    disp_drv.hor_res  = 240; /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res  = 128;   /*Set the vertical resolution in pixels*/

    lv_disp_t *disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
    lv_theme_t *th = lv_theme_mono_init(disp, 0, &lv_font_unscii_8);
    lv_disp_set_theme(disp, th);
    
    
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); /*Basic initialization*/
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    /*Register the driver in LVGL and save the created input device object*/
    lv_indev_t *indev = lv_indev_drv_register(&indev_drv);
   

    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    
    
    /*Create a Label on the currently active screen*/
    lv_obj_t *label1 =  lv_label_create(btn);
    lv_label_set_text(label1, "testo");
    lv_obj_center(label1);
    lv_obj_set_pos(btn,10, 10);// position, position);
    lv_obj_set_size(btn, 200,60);
    
    while (1) {
        lv_task_handler();
        lv_tick_inc(1);
        __delay_ms(1);
        
        if (is_expired(ts,get_millis(), 1000)) {
            HAP_RUN=blink;
            blink=!blink;
            ts=get_millis();
        }
        
        if (is_expired(touchts, get_millis(), 20)) {
            touch_manage();
            lv_label_set_text_fmt(label1, "touch %3i %3i", Touch_Coord[0], Touch_Coord[1]);
            touchts = get_millis();
        }
    }
}

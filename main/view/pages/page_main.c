#include <stdio.h>
#include "gel/pagemanager/page_manager.h"
#include "lvgl/lvgl.h"
#include "model/model.h"
#include "peripherals/timer.h"
#include "peripherals/digout.h"
#include "view/common.h"
#include "view/view.h"
#include "view/view_types.h"

static struct {
} page_data;

static void *create_page(model_t *model, void *extra) {
    return NULL;
}

static void open_page(model_t *model, void *data) {
    
    lv_obj_t *btn = lv_btn_create(lv_scr_act());

    /*Create a Label on the currently active screen*/
    lv_obj_t *label1 =  lv_label_create(btn);
    lv_label_set_text(label1, "testo");
    lv_obj_center(label1);
    lv_obj_set_pos(btn,10, 10);// position, position);
    lv_obj_set_size(btn, 200,60);
}

static view_message_t process_page_event(model_t *model, void *arg, pman_event_t event) {
    view_message_t msg = {.vmsg = {VIEW_PAGE_COMMAND_CODE_NOTHING}};

    return msg;
}

static view_t update_page(model_t *pmodel, void *arg) {

    return 0;
}

static void page_destroy(void *data, void *extra) {
}

static void view_close(void *data) {
    (void)data;
    lv_obj_clean(lv_scr_act());
}

const pman_page_t page_main = {
    .create        = create_page,
    .open          = open_page,
    .update        = update_page,
    .process_event = process_page_event,
    .close         = view_close,
    .destroy       = page_destroy,
};

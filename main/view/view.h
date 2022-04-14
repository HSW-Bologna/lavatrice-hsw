#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <stdint.h>
#include "view_types.h"
#include "model/model.h"
#include "gel/pagemanager/page_manager.h"
#include "lvgl.h"

view_t view_init(model_t *model, void (*flush_cb)(struct _lv_disp_drv_t *, const lv_area_t *, lv_color_t *),
                 void (*rounder_cb)(struct _lv_disp_drv_t *, lv_area_t *),
                 void (*set_px_cb)(struct _lv_disp_drv_t *, uint8_t *, lv_coord_t, lv_coord_t, lv_coord_t, lv_color_t,
                                   lv_opa_t),
        void (*read_cb)(struct _lv_indev_drv_t *indev_drv, lv_indev_data_t *data));

view_t view_change_page_extra(model_t *model, const pman_page_t *page, void *extra);
view_t view_change_page(model_t *model, const pman_page_t *page);
view_t view_rebase_page(model_t *model, const pman_page_t *page);
int    view_get_next_msg(model_t *model, view_message_t *msg, view_event_t *eventcopy);
int    view_process_msg(view_page_command_t vmsg, model_t *model);
void   view_event(view_event_t event);
void   view_close_all(void *data);

extern const pman_page_t page_main;

#endif
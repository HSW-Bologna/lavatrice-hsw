#include <assert.h>
#include "view/common.h"
#include "gel/timer/timecheck.h"


lv_obj_t *view_common_title(lv_obj_t *root, const char *str) {
    lv_obj_t *cont = lv_obj_create(root);

    lv_obj_t *title = lv_label_create(cont);
    lv_label_set_text(title, str);
    lv_obj_set_width(title, LV_HOR_RES - 4);

    lv_obj_set_width(cont, LV_HOR_RES - 4);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 2);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, 0);

    return title;
}


#ifndef TOUCH_H_INCLUDED
#define TOUCH_H_INCLUDED


#include "lvgl.h"


#define     K_REPEAT_DECS               5
#define     K_REPEAT_S                  50




// Default parameter
#define     DEFAULT_CALIBRATION_X_MIN   155
#define     DEFAULT_CALIBRATION_X_MAX   3885
#define     DEFAULT_CALIBRATION_Y_MIN   325
#define     DEFAULT_CALIBRATION_Y_MAX   3690

#define     AVERAGE_NUM                 2       // n.ro letture per MEDIA MISURE



// #define     PENIRQ_TSC2046              P7.3    /* Pen IRQ per TSC2046 */

// Screen resolution
#define     SCREEN_RES_X                240L    // pixel
#define     SCREEN_RES_Y                128L    // pixel

#define     MAX_ADC_VAL                 4000    // ignora letture oltre questa soglia limite

#define     DOUBLE_BUFFERING_SIZE       2

#define     TOUCH_Z1_MIN_TRESH          400     // range (0, 4095) era 390
#define     TOUCH_Z2_MAX_TRESH          3900    // range (0, 4095) era 3800
#define     TOUCH_ON_OFF_TIME           1       // loop time confirm on/off touch - debounce time





#define     NR_PT                       14

#define     PT_START                    1
#define     PT_STOP                     2
#define     PT_A                        3
#define     PT_B                        4
#define     PT_PROG_N                   5
#define     PT_PROG_1                   6
#define     PT_PROG_2                   7
#define     PT_PROG_3                   8
#define     PT_PROG_4                   9
#define     PT_FUNZ                     10
#define     PT_TEMP                     11
#define     PT_MENU                     12

#define     PT_TIME_1                   13
#define     PT_TIME_2                   14
#define     PT_TIME_3                   15

#define     PT_PIU                      16
#define     PT_MENO                     17

#define     PT_SU                       18
#define     PT_GIU                      19

#define     PT_ESC                      20
#define     PT_ENT                      21

#define     PT_SX                       22
#define     PT_DX                       23
#define     PT_SET                      24

#define     PT_GOSTH_LH                 30
#define     PT_GOSTH_CH                 31
#define     PT_GOSTH_RH                 32

#define     PT_PIN_A_C                  40



#define     CALIBRATION_TOUCH_TIME      2 /* in secondi */



extern unsigned char                rTtouch;                // rileva codice posizione tasto premuto
extern unsigned char                Ttouch_ABIL[NR_PT];     // segnala posizione tasto abilitato
extern unsigned char                Ttouch_VISIB[NR_PT];    // segnala se farlo vedere o "fantasma"
extern unsigned char                fTTkey;
extern unsigned char                cTkey;
extern unsigned char                cOldTkey;
extern unsigned int                 Key_repeat_cnt;

extern unsigned char                ab_touch_click;
extern char                         f_cal;




/******************************************************************************/
/*  Prototipi                                                                 */
/******************************************************************************/

typedef enum
{
    NO_CALIBRATION = 0,
    CALIBRATION_LEFT_LOW,
    CALIBRATION_RIGHT_HI,
    CALIBRATION_CONFIRM,
    CALIBRATION_UNDO,
    NUM_OF_TOUCH_CALIBRATION_ANGLE_ELEMENTS
} TOUCH_CALIBRATION_ANGLE_TYPE;

void touch_init(void);
//void Touch__CycleRoutine(void);
//unsigned char touch_detected(void);
//void Touch__UpdateCoordinates(void);
void Touch__Calibration(TOUCH_CALIBRATION_ANGLE_TYPE step);
void touch_manage(void);
void touch_read(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t *data);
void startCalibration(void);

void Touch_Key_Get (void);



typedef enum {
    ROTATE_0 = 0,
            ROTATE_90,
            ROTATE_180,
            ROTATE_270,
} display_rotation_t;


typedef enum
{
    CALIBRATION_X_MIN = 0,
    CALIBRATION_X_MAX,
    CALIBRATION_Y_MIN,
    CALIBRATION_Y_MAX,
    NUM_OF_TOUCH_CALIBRATION_ELEMENTS
} TOUCH_CALIBRATION_TYPE;

typedef enum
{
    X_COORD = 0,
    Y_COORD,
    NUM_OF_COORDINATES_ELEMENTS
} TOUCH_COORDINATES_TYPE;

extern unsigned char               f_touch_detected;



typedef enum
{
    TOUCH_X_CHANNEL = 0,
    TOUCH_Y_CHANNEL,
    TOUCH_Z1_CHANNEL,
    TOUCH_Z2_CHANNEL,
    //    TOUCH_TEMPERATURE_0_CHANNEL,
    //    TOUCH_TEMPERATURE_1_CHANNEL,
    //    TOUCH_V_BATTERY,
    //    TOUCH_AUX,
    NUM_OF_TOUCH_CHANNEL_ELEMENTS
} TOUCH_CHANNEL_TYPE;

extern unsigned short Touch_Calibration[NUM_OF_TOUCH_CALIBRATION_ELEMENTS];

extern unsigned short Touch_Coord[NUM_OF_COORDINATES_ELEMENTS];
extern unsigned short Touch_Avg[AVERAGE_NUM][NUM_OF_COORDINATES_ELEMENTS];
extern unsigned char               current_touch_channel;
extern unsigned char               bit_val;
extern unsigned short              word_val;
extern unsigned char               write_idx;
extern unsigned short Touch_Val[NUM_OF_TOUCH_CHANNEL_ELEMENTS][DOUBLE_BUFFERING_SIZE];

#endif
/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: Touch.c                                                           */
/*                                                                            */
/*      gestione touch screen 240x128 con Driver TSC2046                      */
/*                                                                            */
/*  Autore: Sabbiolino (Fabio BARALDI) e Massimo ZANNA                        */
/*                                                                            */
/*  Data  : 05/12/2019      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 23/02/2020      REV  : 01.0                                       */
/*                                                                            */
/******************************************************************************/

#include <string.h>
#include "touch.h"
#include "hardwareprofile.h"
#include "timer.h"
#include "display.h"
// 
// /******************************************************************************/
// /*  #define                                                                   */
// /******************************************************************************/
// 
// // #define     PENIRQ_TSC2046              P7.3    /* Pen IRQ per TSC2046 */
// 
// // Screen resolution
// #define     SCREEN_RES_X                240L    // pixel
// #define     SCREEN_RES_Y                128L    // pixel
// 
// #define     MAX_ADC_VAL                 4000    // ignora letture oltre questa soglia limite
// 
// #define     DOUBLE_BUFFERING_SIZE       2
// 
// #define     TOUCH_Z1_MIN_TRESH          400     // range (0, 4095) era 390
// #define     TOUCH_Z2_MAX_TRESH          3900    // range (0, 4095) era 3800
// #define     TOUCH_ON_OFF_TIME           1       // loop time confirm on/off touch - debounce time
// 

//                      "f_touch_detected" !!!! X sapere se TOUCH TOCCATO + LOAD tempo AUTO LOW RETRO LCD !!!!



#define TRUE                        1
#define FALSE                       0





// <editor-fold defaultstate="collapsed" desc="Struct e variabili">

typedef enum
{
    TOUCH_WAKE_UP = 0,
    TOUCH_START,
    TOUCH_A2,
    TOUCH_A1,
    TOUCH_A0,
    TOUCH_MODE,
    TOUCH_SER,
    TOUCH_PD1,
    TOUCH_PD0,
    TOUCH_BUSY_CHECK,
    TOUCH_ANSWER_BIT_11,
    TOUCH_ANSWER_BIT_10,
    TOUCH_ANSWER_BIT_9,
    TOUCH_ANSWER_BIT_8,
    TOUCH_ANSWER_BIT_7,
    TOUCH_ANSWER_BIT_6,
    TOUCH_ANSWER_BIT_5,
    TOUCH_ANSWER_BIT_4,
    TOUCH_ANSWER_BIT_3,
    TOUCH_ANSWER_BIT_2,
    TOUCH_ANSWER_BIT_1,
    TOUCH_ANSWER_BIT_0,
    NUM_OF_TOUCH_STATE_ELEMENTS
} TOUCH_STATE_TYPE;

TOUCH_STATE_TYPE Touch_State;



// 
// 
// typedef enum
// {
//     TOUCH_X_CHANNEL = 0,
//     TOUCH_Y_CHANNEL,
//     TOUCH_Z1_CHANNEL,
//     TOUCH_Z2_CHANNEL,
//     //    TOUCH_TEMPERATURE_0_CHANNEL,
//     //    TOUCH_TEMPERATURE_1_CHANNEL,
//     //    TOUCH_V_BATTERY,
//     //    TOUCH_AUX,
//     NUM_OF_TOUCH_CHANNEL_ELEMENTS
// } TOUCH_CHANNEL_TYPE;

TOUCH_CHANNEL_TYPE Current_Touch_Channel;

typedef union
{
    unsigned char BYTE;

    struct
    {
        unsigned char PD0 : 1;
        unsigned char PD1 : 1;
        unsigned char SER : 1;
        unsigned char MODE : 1;
        unsigned char A0 : 1;
        unsigned char A1 : 1;
        unsigned char A2 : 1;
        unsigned char START : 1;

    } BIT;
} TOUCH_CONTROL_BYTE_TYPE;

TOUCH_CONTROL_BYTE_TYPE Touch_Control_Byte;

union
{
    unsigned char BYTE;

    struct
    {
        unsigned char Clock_State : 1;
        unsigned char Busy : 1;
        unsigned char Read_Idx : 1;
        unsigned char Calibration : 1;
        unsigned char Free4 : 1;
        unsigned char Free5 : 1;
        unsigned char Free6 : 1;
        unsigned char Free7 : 1;
    } BIT;
} Touch_Flags;


unsigned char Touch_Clock_State;

unsigned short Touch_Val[NUM_OF_TOUCH_CHANNEL_ELEMENTS][DOUBLE_BUFFERING_SIZE];

typedef enum
{
    BIT_A0 = 0,
    BIT_A1,
    BIT_A2,
    NUM_OF_CHANNEL_ADDRESS_BIT_ELEMENTS
} TOUCH_CHANNEL_ADDRESS_BIT_TYPE;

const TOUCH_CHANNEL_ADDRESS_BIT_TYPE Touch_Channel_ddress_Bit[NUM_OF_TOUCH_CHANNEL_ELEMENTS][NUM_OF_CHANNEL_ADDRESS_BIT_ELEMENTS] =
{
    //  BIT_A0  BIT_A1  BIT_A2
    {1, 0, 1}, // TOUCH_X_CHANNEL
    {1, 0, 0}, // TOUCH_Y_CHANNEL
    {1, 1, 0}, // TOUCH_Z1_CHANNEL
    {0, 0, 1} // TOUCH_Z2_CHANNEL
};


display_rotation_t v_rotate = ROTATE_0;



TOUCH_CALIBRATION_ANGLE_TYPE Touch_Calibration_Request;

// typedef enum
// {
//     CALIBRATION_X_MIN = 0,
//     CALIBRATION_X_MAX,
//     CALIBRATION_Y_MIN,
//     CALIBRATION_Y_MAX,
//     NUM_OF_TOUCH_CALIBRATION_ELEMENTS
// } TOUCH_CALIBRATION_TYPE;

unsigned short Touch_Calibration[NUM_OF_TOUCH_CALIBRATION_ELEMENTS];
unsigned short Touch_Calibration_tmp[NUM_OF_TOUCH_CALIBRATION_ELEMENTS];
unsigned short Touch_Calibration_old[NUM_OF_TOUCH_CALIBRATION_ELEMENTS];





// Actual value of touch coordinates
unsigned short Touch_Coord[NUM_OF_COORDINATES_ELEMENTS];
unsigned short Touch_Avg[AVERAGE_NUM][NUM_OF_COORDINATES_ELEMENTS];





unsigned char               touch_detected;
unsigned char               f_touch_detected;
unsigned char               ct_touch_on_off_time;
extern unsigned char        buzzer_on;
extern unsigned char        nro_buzzer;


unsigned char               bit_val;
unsigned short              word_val;
unsigned char               write_idx;
unsigned char               current_touch_channel;


unsigned char               rTtouch;                // rileva codice posizione tasto premuto
unsigned char               Ttouch_ABIL[NR_PT];     // segnala posizione tasto abilitato
unsigned char               Ttouch_VISIB[NR_PT];    // segnala se farlo vedere o "fantasma"
unsigned char               fTTkey;
unsigned char               cTkey = 0;
unsigned char               cOldTkey = 0;
unsigned int                Key_repeat_cnt = 0;



unsigned char               fTbeep = 0;
unsigned char               ab_touch_click = 0;
char                        f_cal = 0;

// </editor-fold>

/******************************************************************************/
/*  Prototipi                                                                 */
/******************************************************************************/

//void Touch__Init(void);
void Touch__CycleRoutine(void);
unsigned char Touch__Detection(void);
void Touch__UpdateCoordinates(void);




/******************************************************************************/
/*  Funzioni                                                                  */
/******************************************************************************/




/*----------------------------------------------------------------------------*/
/*  Init_Touch                                                                */
/*----------------------------------------------------------------------------*/
void touch_init(void)
{
    TOUCH_CHANNEL_TYPE touch_channel;
    unsigned char i;
    
    CS_TOUCH = 1;
    SPI_1_SCK = 0;
    SPI_1_SDO = 0;
    
    // Init variabili
    Touch_Flags.BYTE = 0;
    Touch_State = TOUCH_WAKE_UP;
    
    Current_Touch_Channel = TOUCH_X_CHANNEL; // TOUCH_Z2_CHANNEL
    
    for (touch_channel = TOUCH_X_CHANNEL; touch_channel < NUM_OF_TOUCH_CHANNEL_ELEMENTS; touch_channel++)
    {
        for (i = 0; i < DOUBLE_BUFFERING_SIZE; i++)
        {
            Touch_Val[touch_channel][i] = 0;
        }
    }
    
    // Set default values for calibration
    Touch_Calibration[CALIBRATION_X_MIN] = DEFAULT_CALIBRATION_X_MIN;
    Touch_Calibration[CALIBRATION_X_MAX] = DEFAULT_CALIBRATION_X_MAX;
    Touch_Calibration[CALIBRATION_Y_MIN] = DEFAULT_CALIBRATION_Y_MIN;
    Touch_Calibration[CALIBRATION_Y_MAX] = DEFAULT_CALIBRATION_Y_MAX;
    
    Touch_Calibration_Request = NO_CALIBRATION;
}



    

bool touch_read(lv_indev_t *indev_drv, lv_indev_data_t *data) {
    (void)indev_drv;
    data->state = f_touch_detected ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    data->point.x = Touch_Coord[X_COORD];
    data->point.y = Touch_Coord[Y_COORD];
    return false;
}





/*----------------------------------------------------------------------------*/
/*  Touch_Key_Get                                                             */
/*----------------------------------------------------------------------------*/
void Touch_Key_Get (void)
{
    if (Touch_Coord [1]>112 && Touch_Coord [1]<127) // CP Y - PARTE BASSA
    {
        if (Touch_Coord [0]>1 && Touch_Coord [0]<25) // CP X
        {
            rTtouch = 1;
        }
        else if (Touch_Coord [0]>32 && Touch_Coord [0]<67)
        {
            rTtouch = 2;
        }
        else if (Touch_Coord [0]>72 && Touch_Coord [0]<118)
        {
            rTtouch = 3;
        }
        else if (Touch_Coord [0]>120 && Touch_Coord [0]<157)
        {
            rTtouch = 4;
        }
        else if (Touch_Coord [0]>160 && Touch_Coord [0]<201)
        {
            rTtouch = 5;
        }
        else if (Touch_Coord [0]>204 && Touch_Coord [0]<240)
        {
            rTtouch = 6;
        }
    }
    else if (Touch_Coord [1]>1 && Touch_Coord [1]<15) // CP Y - PARTE ALTA
    {
        if (Touch_Coord [0]>1 && Touch_Coord [0]<25) // CP X
        {
            rTtouch = 7;
        }
//         else if (Touch_Coord [0]>32 && Touch_Coord [0]<67)
//         {
//             rTtouch = 2;
//         }
//         else if (Touch_Coord [0]>72 && Touch_Coord [0]<118)
//         {
//             rTtouch = 3;
//         }
        else if (Touch_Coord [0]>100 && Touch_Coord [0]<140)
        {
            rTtouch = 8;
        }
//         else if (Touch_Coord [0]>120 && Touch_Coord [0]<157)
//         {
//             rTtouch = 4;
//         }
//         else if (Touch_Coord [0]>160 && Touch_Coord [0]<201)
//         {
//             rTtouch = 5;
//         }
        else if (Touch_Coord [0]>204 && Touch_Coord [0]<240)
        {
            rTtouch = 9;
        }
    }
    else
    {
        rTtouch = 0;
        cTkey = 0;
        fTTkey = 0;
        fTbeep = 0;
    }
    
    if (rTtouch != 0)
    {
        cTkey = Ttouch_ABIL[rTtouch-1];
        
        if (cTkey!=0 && fTbeep==0 && ab_touch_click==1)
        {
            fTbeep = 1;
            //GT_Buzzer(50, 3);
        }
    }
}




void rotate_touch_coordinates(unsigned short *x, unsigned short *y, display_rotation_t rotate)
{
    unsigned short oldx, oldy;
    oldx = *x;
    oldy = *y;
    
    if (rotate == ROTATE_90)
    {
        *x = PIXEL_COLUMNS - oldy;
        *y = oldx;
    }
    else if (rotate == ROTATE_180)
    {
        *x = PIXEL_ROWS - oldx;
        *y = PIXEL_COLUMNS - oldy;
    }
    else if (rotate == ROTATE_270)
    {
        *x =  oldy;
        *y = PIXEL_ROWS - oldx;
    }
}





/*----------------------------------------------------------------------------*/
/*  Touch__Refresh                                                            */
/*----------------------------------------------------------------------------*/
void touch_manage(void)
{
    static int calibration_touch_time = 0;
    unsigned char i, j;
    unsigned short x, y;
    
    x = y = 0;
    i = j = 0;
    
    Current_Touch_Channel = TOUCH_X_CHANNEL; // TOUCH_Z2_CHANNEL
    
    CS_TOUCH = 0; // seleziono CHIP TOUCH

    
    for (i = 0; i < AVERAGE_NUM; i++)
    {
        for (j = 0; j < NUM_OF_TOUCH_CHANNEL_ELEMENTS; j++)
        {
            Touch_State = TOUCH_START;

            while (Touch_State != TOUCH_WAKE_UP)
            {
                Touch__CycleRoutine();
            }
        }
        x += Touch_Coord[X_COORD];
        y += Touch_Coord[Y_COORD];
                
        if (f_touch_detected == 0)
        {
            fTTkey = 0;
            CS_TOUCH = 1; // deseleziono CHIP TOUCH
            
            Touch_Coord[X_COORD] = 0;
            Touch_Coord[Y_COORD] = 0;
            
            if (f_cal == 1)
            {
                f_cal++;
            }
            else if (f_cal == 3)
            {
                f_cal++;
            }
            else if (f_cal == 5)
            {
                f_cal = 0;
                Touch__Calibration(CALIBRATION_CONFIRM);
            }
            
            if (f_cal == 2 || f_cal == 4) {
                /* Segnati il tempo per cui sta premuto */
                 calibration_touch_time = get_millis()/1000;
            }
            return;
        }
    }
    CS_TOUCH = 1; // deseleziono CHIP TOUCH
    x /= AVERAGE_NUM;
    y /= AVERAGE_NUM;
    
    rotate_touch_coordinates(&x, &y, v_rotate);
    
    Touch_Coord[X_COORD] = x;
    Touch_Coord[Y_COORD] = y;
    
    /* Procedi con la calibrazione solo se l'utente ha premuto per
      almeno 2 secondi*/
    if (f_cal == 2 && (get_millis()/1000)- calibration_touch_time >= CALIBRATION_TOUCH_TIME)
    {
        Touch__Calibration(CALIBRATION_LEFT_LOW);
        //GT_Buzzer(200,1);
        f_cal++;
    }
    else if (f_cal == 4 && (get_millis()/1000) - calibration_touch_time >= CALIBRATION_TOUCH_TIME)
    {
        Touch__Calibration(CALIBRATION_RIGHT_HI);
        //GT_Buzzer(500,2);
        f_cal++;
    }
}





/*----------------------------------------------------------------------------*/
/*  Touch__Detection                                                          */
/*----------------------------------------------------------------------------*/
unsigned char Touch__Detection(void)
{
    // Check if screen has been touched
    if ((Touch_Val[TOUCH_Z1_CHANNEL][Touch_Flags.BIT.Read_Idx] > TOUCH_Z1_MIN_TRESH) ||
            (Touch_Val[TOUCH_Z2_CHANNEL][Touch_Flags.BIT.Read_Idx] < TOUCH_Z2_MAX_TRESH))
    {
        if (touch_detected == FALSE)
        {
            ct_touch_on_off_time++; // debounce time

            if (ct_touch_on_off_time > TOUCH_ON_OFF_TIME)
            {
                touch_detected = TRUE;
            }
        }

        if (touch_detected == TRUE)
        {
            ct_touch_on_off_time = 0;
        }
    }
    else
    {
        if (touch_detected == TRUE)
        {
            ct_touch_on_off_time++; // debounce time

            if (ct_touch_on_off_time > TOUCH_ON_OFF_TIME)
            {
                touch_detected = FALSE;
            }
        }

        if (touch_detected == FALSE)
        {
            ct_touch_on_off_time = 0;
        }
    }

    // Ignore touch in presence of abnormal value
    if ((Touch_Val[TOUCH_X_CHANNEL][Touch_Flags.BIT.Read_Idx] > MAX_ADC_VAL) ||
            (Touch_Val[TOUCH_Y_CHANNEL][Touch_Flags.BIT.Read_Idx] > MAX_ADC_VAL))
    {
        if (touch_detected == TRUE)
        {
            ct_touch_on_off_time++; // debounce time

            if (ct_touch_on_off_time > TOUCH_ON_OFF_TIME)
            {
                touch_detected = FALSE;
            }
        }

        if (touch_detected == FALSE)
        {
            ct_touch_on_off_time = 0;
        }
    }
    return touch_detected;
}





/*----------------------------------------------------------------------------*/
/*  Touch__UpdateCoordinates                                                  */
/*----------------------------------------------------------------------------*/
void Touch__UpdateCoordinates(void)
{
    unsigned long x_max_abs;
    unsigned long y_max_abs;
    unsigned long x;
    unsigned long y;
    unsigned short tmp1, tmp2;
    tmp1 = Touch_Val[TOUCH_X_CHANNEL][Touch_Flags.BIT.Read_Idx];
    tmp2 = Touch_Val[TOUCH_Y_CHANNEL][Touch_Flags.BIT.Read_Idx];


    if (Touch_Val[TOUCH_X_CHANNEL][Touch_Flags.BIT.Read_Idx] < Touch_Calibration[CALIBRATION_X_MIN])
    {
        x = 0;
    }
    else
    {
        x = Touch_Val[TOUCH_X_CHANNEL][Touch_Flags.BIT.Read_Idx] - Touch_Calibration[CALIBRATION_X_MIN];
    }

    if (Touch_Val[TOUCH_Y_CHANNEL][Touch_Flags.BIT.Read_Idx] < Touch_Calibration[CALIBRATION_Y_MIN])
    {
        y = 0;
    }
    else
    {
        y = Touch_Val[TOUCH_Y_CHANNEL][Touch_Flags.BIT.Read_Idx] - Touch_Calibration[CALIBRATION_Y_MIN];
    }

    // Calc absolute vale of X,Y max (remove offset of min value)
    x_max_abs = Touch_Calibration[CALIBRATION_X_MAX] - Touch_Calibration[CALIBRATION_X_MIN];
    y_max_abs = Touch_Calibration[CALIBRATION_Y_MAX] - Touch_Calibration[CALIBRATION_Y_MIN];

    // Calc actual coordinates
    Touch_Coord[X_COORD] = (x * SCREEN_RES_X) / x_max_abs;
    Touch_Coord[Y_COORD] = (y * SCREEN_RES_Y) / y_max_abs;

    if (Touch_Coord[X_COORD] > 240)
    {
        Touch_Coord[X_COORD] = 240;
    }

    if (Touch_Coord[Y_COORD] > 128)
    {
        Touch_Coord[Y_COORD] = 128;
    }
}



/*----------------------------------------------------------------------------*/
/* startCalibration                                                           */
/*----------------------------------------------------------------------------*/
void startCalibration() {
    f_cal = 1;
}


/*----------------------------------------------------------------------------*/
/* Touch__Calibration                                                         */
/*----------------------------------------------------------------------------*/
void Touch__Calibration(TOUCH_CALIBRATION_ANGLE_TYPE step)
{
    switch (step)
    {
        default:
        case NO_CALIBRATION:
            break;
            
        case CALIBRATION_LEFT_LOW:
            Touch_Calibration_tmp[CALIBRATION_X_MIN] =
                Touch_Val[TOUCH_X_CHANNEL][Touch_Flags.BIT.Read_Idx];
            Touch_Calibration_tmp[CALIBRATION_Y_MIN] =
                Touch_Val[TOUCH_Y_CHANNEL][Touch_Flags.BIT.Read_Idx];
            break;

        case CALIBRATION_RIGHT_HI:
            Touch_Calibration_tmp[CALIBRATION_X_MAX] =
                Touch_Val[TOUCH_X_CHANNEL][Touch_Flags.BIT.Read_Idx];
            Touch_Calibration_tmp[CALIBRATION_Y_MAX] =
                Touch_Val[TOUCH_Y_CHANNEL][Touch_Flags.BIT.Read_Idx];
            break;
            
        case CALIBRATION_UNDO:
            /* Scambia i nuovi valori con i vecchi*/
            memcpy(Touch_Calibration, Touch_Calibration_old,
                    sizeof(unsigned short)*NUM_OF_TOUCH_CALIBRATION_ELEMENTS);
            break;
        case CALIBRATION_CONFIRM:
            /* Scambia i nuovi valori con i vecchi*/
            memcpy(Touch_Calibration_old, Touch_Calibration,
                    sizeof(unsigned short)*NUM_OF_TOUCH_CALIBRATION_ELEMENTS);
            memcpy(Touch_Calibration, Touch_Calibration_tmp,
                    sizeof(unsigned short)*NUM_OF_TOUCH_CALIBRATION_ELEMENTS);
            break;
                       
    }
    
    // Reset touch calibration request
    // Touch_Calibration_Request = NO_CALIBRATION;
}



/*----------------------------------------------------------------------------*/
/*  Touch__CycleRoutine                                                       */
/*----------------------------------------------------------------------------*/
void Touch__CycleRoutine(void)
{    
    // Get double buffering index of Touch_Val to be updated
    if (Touch_Flags.BIT.Read_Idx == 0)
    {
        write_idx = 1;
    }
    else
    {
        write_idx = 0;
    }
    
    // Get index of current touch val to be updated
    if (Current_Touch_Channel > TOUCH_X_CHANNEL)
    {
        current_touch_channel = Current_Touch_Channel - 1;
    }
    else
    {
        current_touch_channel = NUM_OF_TOUCH_CHANNEL_ELEMENTS - 1;
    }
    
    // Get current touch val to be updated
//    word_val = Touch_Val[current_touch_channel][write_idx];

    // Touch State Machine
    switch (Touch_State)
    {
        default:
        case TOUCH_WAKE_UP:
            break;
 
        case TOUCH_START:                                           // CLK 01
            if (Touch_Flags.BIT.Clock_State == 0) // CP CLK
            {
                // CLK 0
                SPI_1_SDO = 1; // START bit always 1
                word_val = Touch_Val[current_touch_channel][write_idx];
            }
            break;
     
        case TOUCH_A2:                                              // CLK 02
            if (Touch_Flags.BIT.Clock_State == 0)
            {
                // Clock down
                SPI_1_SDO = Touch_Channel_ddress_Bit[Current_Touch_Channel][BIT_A2]; // A2 bit
            }
            break;
 
        case TOUCH_A1:                                              // CLK 03
            if (Touch_Flags.BIT.Clock_State == 0)
            {
                // Clock down
                SPI_1_SDO = Touch_Channel_ddress_Bit[Current_Touch_Channel][BIT_A1]; // A1 bit
            }
            break;
   
        case TOUCH_A0:                                              // CLK 04
            if (Touch_Flags.BIT.Clock_State == 0)
            {
                // Clock down
                SPI_1_SDO = Touch_Channel_ddress_Bit[Current_Touch_Channel][BIT_A0]; // A0 bit
            }
            break;

        case TOUCH_MODE:                                            // CLK 05
            if (Touch_Flags.BIT.Clock_State == 0)
            {
                // Clock down
                SPI_1_SDO = 0; // MODE bit always 0 -> 12 bit ADC resolution
            }
            break;

        case TOUCH_SER:                                             // CLK 06
            if (Touch_Flags.BIT.Clock_State == 0) {
                // Clock down
                SPI_1_SDO = 0; // SER bit always 0 -> differential mode
            }
            else {
                // Update Current_Touch_Channel
                if (Current_Touch_Channel < TOUCH_Z2_CHANNEL) {
                    Current_Touch_Channel++;
                } else {
                    ///////////////////// TOUCH SCAN CYCLE END /////////////////////
                    Current_Touch_Channel = TOUCH_X_CHANNEL; // TOUCH_Z2_CHANNEL; 

                    // Update double buffering index
                    if (Touch_Flags.BIT.Read_Idx == 0) {
                        Touch_Flags.BIT.Read_Idx = 1;
                    } else {
                        Touch_Flags.BIT.Read_Idx = 0;
                    }

                    // Check if screen is touched
                    
                }
            }
            break;
            
            
            
        case TOUCH_PD1:                                             // CLK 07
            if (Touch_Flags.BIT.Clock_State == 0) {
                // Clock down
                SPI_1_SDO = 1; // PD1 bit always 1 -> reference ON
            }
            break;
            
            
            
        case TOUCH_PD0:                                             // CLK 08
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock down
                SPI_1_SDO = 1; // PD0 bit always 1 -> ADC ON
            }
            break;
            
            
            
        case TOUCH_BUSY_CHECK:                                      // CLK 09
            if (Touch_Flags.BIT.Clock_State == 0)
            {
                // Clock down
                SPI_1_SDO = 0; // DIN signal OFF
            }
            else
            {
                // Read busy signal
                Touch_Flags.BIT.Busy = TOUCH_BUSY;
            }
            break;
            
            
            
        case TOUCH_ANSWER_BIT_11:                                   // CLK 10
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Read busy signal
                Touch_Flags.BIT.Busy = TOUCH_BUSY;

                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 11
                if (bit_val == 1)
                {
                    word_val |= 0x0800; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xF7FF; // bit == 0 -> reset
                }
            }
            break;
            
            
            
        case TOUCH_ANSWER_BIT_10:                                   // CLK 11
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 10
                if (bit_val == 1)
                {
                    word_val |= 0x0400; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFBFF; // bit == 0 -> reset
                }
            }
            break;
            
            
            
        case TOUCH_ANSWER_BIT_9:                                    // CLK 12
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 9
                if (bit_val == 1)
                {
                    word_val |= 0x0200; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFDFF; // bit == 0 -> reset
                }
            }
            break;
            
            
            
        case TOUCH_ANSWER_BIT_8:                                    // CLK 13
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 8
                if (bit_val == 1)
                {
                    word_val |= 0x0100; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFEFF; // bit == 0 -> reset
                }
            }
            break;
            
            
            
        case TOUCH_ANSWER_BIT_7:                                    // CLK 14
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 7
                if (bit_val == 1)
                {
                    word_val |= 0x0080; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFF7F; // bit == 0 -> reset
                }
            }
            break;
            
            
            
        case TOUCH_ANSWER_BIT_6:                                    // CLK 15
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 6
                if (bit_val == 1)
                {
                    word_val |= 0x0040; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFFBF; // bit == 0 -> reset
                }
            }
            break;
            
        case TOUCH_ANSWER_BIT_5:                                    // CLK 15
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 6
                if (bit_val == 1)
                {
                    word_val |= 0x0020; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFFDF; // bit == 0 -> reset
                }
            }
            break;
            
        case TOUCH_ANSWER_BIT_4:                                    // CLK 15
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 6
                if (bit_val == 1)
                {
                    word_val |= 0x0010; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFFEF; // bit == 0 -> reset
                }
            }
            break;
            
            
        case TOUCH_ANSWER_BIT_3:                                    // CLK 15
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 6
                if (bit_val == 1)
                {
                    word_val |= 0x0008; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFFF7; // bit == 0 -> reset
                }
            }
            break;
            
        case TOUCH_ANSWER_BIT_2:                                    // CLK 15
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 6
                if (bit_val == 1)
                {
                    word_val |= 0x0004; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFFFB; // bit == 0 -> reset
                }
            }
            break;
            
        case TOUCH_ANSWER_BIT_1:                                    // CLK 15
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;

                // Update touch value for bit 6
                if (bit_val == 1)
                {
                    word_val |= 0x0002; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFFFD; // bit == 0 -> reset
                }
            }
            break;
            
        case TOUCH_ANSWER_BIT_0:                                    // CLK 15
            if (Touch_Flags.BIT.Clock_State == 1)
            {
                // Clock up
                // Read bit value from TSC2046
                bit_val = SPI_1_SDI;
                
                // Update touch value for bit 6
                if (bit_val == 1)
                {
                    word_val |= 0x0001; // bit == 1 -> set
                }
                else
                {
                    word_val &= 0xFFFE; // bit == 0 -> reset
                }
                Touch_Val[current_touch_channel][write_idx] = word_val;
                
                /* Aggiorna le coordinate solo se non sto calibrando */
                if (Touch__Detection() == TRUE && f_cal == 0) {
                    // Update touch coordinates
                    Touch__UpdateCoordinates();
                }
            }
            break;
    }
    
    // Update current touch value
    Touch_Val[current_touch_channel][write_idx] = word_val;
    
    // Toggle Touch Clock_State
    Touch_Flags.BIT.Clock_State ^= 1;
    
    // Handle clock signal
    SPI_1_SCK = Touch_Flags.BIT.Clock_State;
    
    // Clock management
    if (Touch_Flags.BIT.Clock_State == 0)
    {
        if (Touch_State < NUM_OF_TOUCH_STATE_ELEMENTS)
        {
            Touch_State++;
        }
        else
        {
            Touch_State = TOUCH_WAKE_UP;
        }
    }
    
    if (touch_detected==1 && f_touch_detected==0)
    {
        f_touch_detected = 1;
        //TODO nro_buzzer = BEEP_TOUCH_SI;
        
        //if (ab_touch_click == 1)
            //BUZZER = 1;
    }
    else
    {
        //         nro_buzzer = 0;
        //if (ab_touch_click == 1)
          //  BUZZER = 0;
    }
    
    if (touch_detected == 0 && f_touch_detected == 1)
    {
        f_touch_detected = 0;
    }
}

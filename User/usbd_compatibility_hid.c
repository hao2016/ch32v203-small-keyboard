/********************************** (C) COPYRIGHT *******************************
 * File Name  : usbd_compatibility_hid.c
 * Author     : WCH
 * Version    : v0.01
 * Date       : 2024/02/05
 * Description:
*******************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "debug.h"
#include "string.h"
#include "usbd_compatibility_hid.h"

void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

__attribute__ ((aligned(4))) uint8_t HID_Report_Buffer[DEF_USBD_MAX_PACK_SIZE];   // HID Report Buffer
__attribute__ ((aligned(4))) uint8_t HIDTxBuffer[DEF_USBD_MAX_PACK_SIZE];         // HID Tx Buffer

extern uint8_t USBD_ENDPx_DataUp( uint8_t endp, uint8_t *pbuf, uint16_t len );
extern uint8_t USBD_Endp2_Busy;

/* global define */
#define Bitx(x)    (0x00000001 << (x))

/* global variables */
static uint8_t usbd_tx_buffer[6];
static uint8_t key_scan_flag;
static uint32_t key_previous_result;
static uint32_t key_current_result;

/*********************************************************************
 * @fn      TIM2_Init
 *
 * @brief   Timer2 100us initialisation
 *
 * @return  none
 */
void TIM3_Init( void )
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 10 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 10000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

/*********************************************************************
 * @fn      TIM2_IRQHandler
 *
 * @brief   TIM2 IRQ handler
 *
 * @return  none
 */
void TIM3_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

    key_scan();
}

void key_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef key_io = {
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3
    };
    GPIO_Init(GPIOA, &key_io);

    key_io.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &key_io);
}

void key_scan(void)
{
    // d
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
        key_current_result &= ~Bitx(3);
    else
        key_current_result |= Bitx(3);
    // e
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
        key_current_result &= ~Bitx(4);
    else
        key_current_result |= Bitx(4);
    // f
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
        key_current_result &= ~Bitx(5);
    else
        key_current_result |= Bitx(5);

    // c,d
    // uint16_t encoder_cnt = TIM_GetCounter(TIM2);
    // TIM_SetCounter(TIM2, 0x7FFF);
    // if(encoder_cnt < 0x7FFF)
    // {
    //     key_current_result |= Bitx(2);
    // }
    // else if (encoder_cnt > 0x7FFF)
    // {
    //     key_current_result |= Bitx(3);
    // }
    // else
    // {
    //     key_current_result &= ~Bitx(2);
    //     key_current_result &= ~Bitx(3);
    // }

    key_scan_flag = 1;
}

void key_scan_handle(void)
{
    static uint8_t tx_flag;
    if(key_scan_flag)
    {
        key_scan_flag = 0;
        if(key_current_result != key_previous_result)
        {
            memset(usbd_tx_buffer, 0, sizeof(usbd_tx_buffer));
            uint32_t *tx_p = (uint32_t *)&usbd_tx_buffer[2];
            *tx_p = key_current_result;

            key_previous_result = key_current_result;
            tx_flag = 1;
        }
    }

    if(tx_flag)
    {
        USBD_ENDPx_DataUp(ENDP2, usbd_tx_buffer, sizeof(usbd_tx_buffer));
    }
}

void encode_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef encoder_io = {
        .GPIO_Mode = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1
    };
    GPIO_Init(GPIOA, &encoder_io);

    TIM_TimeBaseInitTypeDef encoder_tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructInit(&encoder_tim);

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);

    TIM_ICInitTypeDef encoder_ic;
    TIM_ICStructInit(&encoder_ic);
    encoder_ic.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    encoder_ic.TIM_ICFilter = 15;
    TIM_ICInit(TIM2, &encoder_ic);
    encoder_ic.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM2, &encoder_ic);

    TIM_SetCounter(TIM2, 0x7FFF);
    TIM_Cmd(TIM2, ENABLE);
}
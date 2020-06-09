/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the LeafLabs Maple Mini.
 */

/*
 * Board identifier.
 */
#define BOARD_MAPLEMINI_STM32_F103
#define BOARD_NAME              "20004-MBDA"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F103xB

/*
 * IO pins assignments
 *
 */

/*
 * IO pins assignments.
 */
#define GPIOA_PWM0          0
#define GPIOA_PWM1          1
#define GPIOA_PWM2          2
#define GPIOA_DO_0          3
#define GPIOA_P4            4
#define GPIOA_P5            5
#define GPIOA_P6            6
#define GPIOA_P7            7

#define GPIOA_DC_EN         8
#define GPIOA_UART1_TX      9
#define GPIOA_UART1_RX      10
#define GPIOA_LED_GREEN     11 //IRQ
#define GPIOA_LED_BLUD      12 // HIBd
#define GPIOA_P13           13
#define GPIOA_P14           14
#define GPIOA_ADC_RDY       15

#define GPIOB_LORA_CTRLN    0
#define GPIOB_LORA_PWR      1
#define GPIOB_P2            2
#define GPIOB_LORA_RST      3
#define GPIOB_LORA_BUSY     4
#define GPIOB_LORA_DIO      5
#define GPIOB_I2C1_SCK      6
#define GPIOB_I2C1_SDA      7
#define GPIOB_P8            8
#define GPIOB_P9            9
#define GPIOB_UAR           10
#define GPIOB_UART2_RX      11
#define GPIOB_SPI2_CS       12
#define GPIOB_SPI2_SCK      13
#define GPIOB_SPI2_MISO     14
#define GPIOB_SPI2_MOSI     15

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_ANALOG(n)           (0 << (((n) & 7) * 4))
#define PIN_OUTPUT_PP_10(n)     (1 << (((n) & 7) * 4))
#define PIN_OUTPUT_PP_2(n)      (2 << (((n) & 7) * 4))
#define PIN_OUTPUT_PP_50(n)     (3 << (((n) & 7) * 4))
#define PIN_INPUT(n)            (4 << (((n) & 7) * 4))
#define PIN_OUTPUT_OD_10(n)     (5 << (((n) & 7) * 4))
#define PIN_OUTPUT_OD_2(n)      (6 << (((n) & 7) * 4))
#define PIN_OUTPUT_OD_50(n)     (7 << (((n) & 7) * 4))
#define PIN_INPUT_PUD(n)        (8 << (((n) & 7) * 4))
#define PIN_ALTERNATE_PP_10(n)  (9 << (((n) & 7) * 4))
#define PIN_ALTERNATE_PP_2(n)   (10 << (((n) & 7) * 4))
#define PIN_ALTERNATE_PP_50(n)  (11 << (((n) & 7) * 4))
#define PIN_ALTERNATE_OD_10(n)  (13 << (((n) & 7) * 4))
#define PIN_ALTERNATE_OD_2(n)   (14 << (((n) & 7) * 4))
#define PIN_ALTERNATE_OD_50(n)  (15 << (((n) & 7) * 4))
#define PIN_UNDEFINED(n)        PIN_INPUT_PUD(n)

/*
 * Port A setup.
 */
#define VAL_GPIOACRL    (PIN_ALTERNATE_PP_50(0)   | /* SMU_CPCK     */  \
                         PIN_ALTERNATE_PP_50(1)    | /* SMU_CPOL        */  \
                         PIN_ALTERNATE_PP_50(2)   | /* SMU_CPOH        */  \
                         PIN_OUTPUT_PP_50(3)           | /* Not Used         */  \
                         PIN_INPUT_PUD(4)    | /* SPI1_CS */            \
                         PIN_INPUT_PUD(5) | /* SPI1_SCK.          */  \
                         PIN_INPUT_PUD(6)           | /* SPI1_MISO.         */  \
                         PIN_INPUT_PUD(7))  /* SPI1_MOSI.         */
#define VAL_GPIOACRH    (PIN_INPUT_PUD(8) | /* Not use.               */  \
                         PIN_ALTERNATE_PP_50(9) | /* USART1_TX.         */  \
                         PIN_INPUT(10)          | /* USART1_RX.         */  \
                         PIN_OUTPUT_PP_2(11)      | /* USART1_CTS            */  \
                         PIN_OUTPUT_PP_2(12)      | /* USART1_RTS            */  \
                         PIN_UNDEFINED(13)          | /* Not use              */  \
                         PIN_UNDEFINED(14)          | /* not use            */  \
                         PIN_INPUT_PUD(15))     /* SMU_STB               */
#define VAL_GPIOAODR    0xFFFFE3FF

/*
 * Port B setup.
 */
#define VAL_GPIOBCRL    (PIN_OUTPUT_PP_2(0)    | /* SmartCard_3/5V.    */  \
                         PIN_OUTPUT_PP_2(1)       | /* Unconnected.       */  \
                         PIN_INPUT_PUD(2)    | /* SPI1_CS.           */  \
                         PIN_OUTPUT_PP_2(3)           | /* TDO.               */  \
                         PIN_INPUT_PUD(4)           | /* TRST.              */  \
                         PIN_OUTPUT_PP_2(5)       | /* Temp.Sensor INT.   */  \
                         PIN_ALTERNATE_OD_10(6) | /* I2C1_SCK.          */  \
                         PIN_ALTERNATE_OD_10(7))  /* I2C1_SDA.          */
#define VAL_GPIOBCRH    (PIN_UNDEFINED(8)           | /* CAN_RX.            */  \
                         PIN_UNDEFINED(9) | /* CAN_TX.            */  \
                         PIN_ALTERNATE_PP_50(10)| /* SmartCard IO.      */  \
                         PIN_OUTPUT_PP_2(11)   | /* SmartCard RST.     */  \
                         PIN_OUTPUT_PP_50(12)| /* SmartCard CLK.     */  \
                         PIN_ALTERNATE_PP_50(13)      |                           \
                         PIN_ALTERNATE_PP_50(14)   | /* USB disconnect.    */  \
                         PIN_ALTERNATE_PP_50(15))
#define VAL_GPIOBODR    0xFFFFFFFF

/*
 * Port C setup.
 */
#define VAL_GPIOCCRL    (PIN_UNDEFINED(0)       |                           \
                         PIN_UNDEFINED(1)       |                           \
                         PIN_UNDEFINED(2)       |                           \
                         PIN_UNDEFINED(3)       |                           \
                         PIN_UNDEFINED(4)          | /* Potentiometer.     */  \
                         PIN_UNDEFINED(5)       |                           \
                         PIN_UNDEFINED(6)    | /* SmartCard CMDVCC.  */  \
                         PIN_UNDEFINED(7))            /* SmartCard OFF.     */
#define VAL_GPIOCCRH    (PIN_UNDEFINED(8) | /* SDIO D0.           */  \
                         PIN_UNDEFINED(9) | /* SDIO D1.           */  \
                         PIN_UNDEFINED(10)| /* SDIO D2.           */  \
                         PIN_UNDEFINED(11)| /* SDIO D3.           */  \
                         PIN_UNDEFINED(12)| /* SDIO CLK.          */  \
                         PIN_OUTPUT_PP_50(13)          | /* Tamper Button.     */  \
                         PIN_UNDEFINED(14)          | /* OSC IN.            */  \
                         PIN_UNDEFINED(15))           /* OSC OUT.           */
#define VAL_GPIOCODR    0xFFFFFFFF

/*
 * Port D setup.
 * Everything input with pull-up except:
 * PD0  - Normal input (XTAL).
 * PD1  - Normal input (XTAL).
 */
#define VAL_GPIODCRL            0x88888844      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x88888888      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x88888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

/*
 * USB bus activation macro, required by the USB driver.
 */
#define usb_lld_connect_bus(usbp) palClearPad(GPIOB, GPIOB_USB_DISC)

/*
 * USB bus de-activation macro, required by the USB driver.
 */
#define usb_lld_disconnect_bus(usbp) palSetPad(GPIOB, GPIOB_USB_DISC)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */

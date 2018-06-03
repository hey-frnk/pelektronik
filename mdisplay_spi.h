/*
 * mdisplay_spi.h
 *
 *  Created on: 30.05.2018
 *      Author: Frank Zheng, MCD Application Team
 */

#ifndef MDISPLAY_SPI_H_
#define MDISPLAY_SPI_H_

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4XX_NUCLEO
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

// Static makes no sense here
void               SPIx_Write(uint8_t Value);

 /*###################### SPI3 ###################################*/
/* #define NUCLEO_SPIx                                 SPI3
 #define NUCLEO_SPIx_CLK_ENABLE()                    __HAL_RCC_SPI3_CLK_ENABLE()

 #define NUCLEO_SPIx_SCK_AF                          GPIO_AF6_SPI3
 #define NUCLEO_SPIx_SCK_GPIO_PORT                   GPIOB
 #define NUCLEO_SPIx_SCK_PIN                         GPIO_PIN_3
 #define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
 #define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

 #define NUCLEO_SPIx_MISO_MOSI_AF                    GPIO_AF6_SPI3
 #define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT             GPIOB
 #define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
 #define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
 #define NUCLEO_SPIx_MISO_PIN                        GPIO_PIN_4
 #define NUCLEO_SPIx_MOSI_PIN                        GPIO_PIN_5*/

#define NUCLEO_SPIx                                 SPI1
#define NUCLEO_SPIx_CLK_ENABLE()                    __HAL_RCC_SPI1_CLK_ENABLE()

#define NUCLEO_SPIx_SCK_AF                          GPIO_AF5_SPI1
#define NUCLEO_SPIx_SCK_GPIO_PORT                   GPIOA
#define NUCLEO_SPIx_SCK_PIN                         GPIO_PIN_5
#define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define NUCLEO_SPIx_MISO_MOSI_AF                    GPIO_AF5_SPI1
#define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT             GPIOA
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define NUCLEO_SPIx_MISO_PIN                        GPIO_PIN_6
#define NUCLEO_SPIx_MOSI_PIN                        GPIO_PIN_7

 /* Maximum Timeout values for flags waiting loops. These timeouts are not based
    on accurate values, they just guarantee that the application will not remain
    stuck if the SPI communication is corrupted.
    You may modify these timeout values depending on CPU frequency and application
    conditions (interrupts routines ...). */
 #define NUCLEO_SPIx_TIMEOUT_MAX                   1000

 /**
   * @brief  LCD Control Lines management
   */
 #define LCD_CS_LOW()      HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
 #define LCD_CS_HIGH()     HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
 #define LCD_DC_LOW()      HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
 #define LCD_DC_HIGH()     HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)

 /**
   * @brief  LCD Control Interface pins
   */

#define LCD_CS_PIN                                 GPIO_PIN_6
#define LCD_CS_GPIO_PORT                           GPIOB
#define LCD_CS_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  LCD Data/Command Interface pins
  */
#define LCD_DC_PIN                                 GPIO_PIN_9
#define LCD_DC_GPIO_PORT                           GPIOA
#define LCD_DC_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOA_CLK_DISABLE()


 /*#define LCD_CS_PIN                                 GPIO_PIN_7
 #define LCD_CS_GPIO_PORT                           GPIOB
 #define LCD_CS_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
 #define LCD_CS_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()*/

 /**
   * @brief  LCD Data/Command Interface pins
   */
 /*#define LCD_DC_PIN                                 GPIO_PIN_4
 #define LCD_DC_GPIO_PORT                           GPIOB
 #define LCD_DC_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOA_CLK_ENABLE()
 #define LCD_DC_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOA_CLK_DISABLE()*/


#endif /* MDISPLAY_SPI_H_ */

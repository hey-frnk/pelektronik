/*
 * mdisplay_spi.c
 *
 *  Created on: 30.05.2018
 *      Author: HARDWARECOP
 */

#include "mdisplay_spi.h"


uint32_t hnucleo_SpixTimeout = NUCLEO_SPIx_TIMEOUT_MAX;        /*<! Value of Timeout when SPI communication fails */
static SPI_HandleTypeDef hnucleo_Spi;

static void               SPIx_Init(void);
static void               SPIx_Error (void);
static void               SPIx_MspInit(void);

void                      LCD_IO_Init(void);
void                      LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
void                      LCD_IO_WriteReg(uint8_t LCDReg);
void                      LCD_Delay(uint32_t delay);


/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/
/**
  * @brief  Initialize SPI MSP.
  * @retval None
  */
static void SPIx_MspInit(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE();
  NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();

  /* Configure SPI SCK */
  gpioinitstruct.Pin        = NUCLEO_SPIx_SCK_PIN;
  gpioinitstruct.Mode       = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull       = GPIO_PULLUP;
  gpioinitstruct.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Alternate  = NUCLEO_SPIx_SCK_AF;
  HAL_GPIO_Init(NUCLEO_SPIx_SCK_GPIO_PORT, &gpioinitstruct);

  /* Configure SPI MISO and MOSI */
  gpioinitstruct.Pin        = NUCLEO_SPIx_MOSI_PIN;
  gpioinitstruct.Alternate  = NUCLEO_SPIx_MISO_MOSI_AF;
  gpioinitstruct.Pull       = GPIO_PULLDOWN;
  HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);

  gpioinitstruct.Pin        = NUCLEO_SPIx_MISO_PIN;
  HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);

  /*** Configure the SPI peripheral ***/
  /* Enable SPI clock */
  NUCLEO_SPIx_CLK_ENABLE();
}

/**
  * @brief  Initialize SPI HAL.
  * @retval None
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&hnucleo_Spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hnucleo_Spi.Instance = NUCLEO_SPIx;
      /* SPI baudrate is set to 8 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 32/4 = 8 MHz)
       to verify these constraints:
          - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
            Since the provided driver doesn't use read capability from LCD, only constraint
            on write baudrate is considered.
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 32 MHz
       */
    hnucleo_Spi.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_4;
    hnucleo_Spi.Init.Direction          = SPI_DIRECTION_2LINES;
    hnucleo_Spi.Init.CLKPhase           = SPI_PHASE_2EDGE;
    hnucleo_Spi.Init.CLKPolarity        = SPI_POLARITY_HIGH;
    hnucleo_Spi.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    hnucleo_Spi.Init.CRCPolynomial      = 7;
    hnucleo_Spi.Init.CRCLength          = SPI_CRC_LENGTH_DATASIZE;
    hnucleo_Spi.Init.DataSize           = SPI_DATASIZE_8BIT;
    hnucleo_Spi.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    hnucleo_Spi.Init.NSS                = SPI_NSS_SOFT;
    hnucleo_Spi.Init.NSSPMode           = SPI_NSS_PULSE_DISABLE;
    hnucleo_Spi.Init.TIMode             = SPI_TIMODE_DISABLE;
    hnucleo_Spi.Init.Mode               = SPI_MODE_MASTER;

    SPIx_MspInit();
    HAL_SPI_Init(&hnucleo_Spi);
  }
}

/**
  * @brief  SPI Write byte(s) to device
  * @param  DataIn: Pointer to data buffer to write
  * @param  DataOut: Pointer to data buffer for read data
  * @param  DataLength: number of bytes to write
  * @retval None
  */
static void SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) DataIn, DataOut, DataLength, hnucleo_SpixTimeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  * @retval None
  */
void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t data;

  status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) &Value, &data, 1, hnucleo_SpixTimeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI error treatment function
  * @retval None
  */
static void SPIx_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hnucleo_Spi);

  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}




















/******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/
/********************************* LINK LCD ***********************************/
/**
  * @brief  Initialize the LCD
  * @retval None
  */
void LCD_IO_Init(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /* LCD_CS_GPIO and LCD_DC_GPIO Periph clock enable */
  LCD_CS_GPIO_CLK_ENABLE();
  LCD_DC_GPIO_CLK_ENABLE();

  /* Configure LCD_CS_PIN pin: LCD Card CS pin */
  gpioinitstruct.Pin    = LCD_CS_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &gpioinitstruct);

  /* Configure LCD_DC_PIN pin: LCD Card DC pin */
  gpioinitstruct.Pin    = LCD_DC_PIN;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &gpioinitstruct);

  /* LCD chip select high */
  LCD_CS_HIGH();

  /* LCD SPI Config */
  SPIx_Init();
}

/**
  * @brief  Write command to select the LCD register.
  * @param  LCDReg: Address of the selected register.
  * @retval None
  */
void LCD_IO_WriteReg(uint8_t LCDReg)
{
  /* Reset LCD control line CS */
  LCD_CS_LOW();

  /* Set LCD data/command line DC to Low */
  LCD_DC_LOW();

  /* Send Command */
  SPIx_Write(LCDReg);

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

/**
* @brief  Write register value.
* @param  pData Pointer on the register value
* @param  Size Size of byte to transmit to the register
* @retval None
*/
void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size)
{
  uint32_t counter = 0;
  __IO uint32_t data = 0;

  /* Reset LCD control line CS */
  LCD_CS_LOW();

  /* Set LCD data/command line DC to High */
  LCD_DC_HIGH();

  if (Size == 1)
  {
    /* Only 1 byte to be sent to LCD - general interface can be used */
    /* Send Data */
    SPIx_Write(*pData);
  }
  else
  {
    /* Several data should be sent in a raw */
    /* Direct SPI accesses for optimization */
    for (counter = Size; counter != 0; counter--)
    {
      while(((hnucleo_Spi.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
      {
      }
      /* Need to invert bytes for LCD*/
      *((__IO uint8_t*)&hnucleo_Spi.Instance->DR) = *(pData+1);

      while(((hnucleo_Spi.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
      {
      }
      *((__IO uint8_t*)&hnucleo_Spi.Instance->DR) = *pData;
      counter--;
      pData += 2;
    }

    /* Wait until the bus is ready before releasing Chip select */
    while(((hnucleo_Spi.Instance->SR) & SPI_FLAG_BSY) != RESET)
    {
    }
  }

  /* Empty the Rx fifo */
  data = *(&hnucleo_Spi.Instance->DR);
  UNUSED(data);  /* Remove GNU warning */

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

/**
  * @brief  Wait for loop in ms.
  * @param  Delay in ms.
  * @retval None
  */
void LCD_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

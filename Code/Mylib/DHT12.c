#include "DHT12.h"

void I2C_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    /* SDA GPIO Clock enable */
    RCC_AHB1PeriphClockCmd(I2Cx_SDA_GPIO_CLK, ENABLE);

    /* SCL GPIO Clock enable */
    RCC_AHB1PeriphClockCmd(I2Cx_SCL_GPIO_CLK, ENABLE);
	
	 /* I2C Peripheral clock enable */
    RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);

     /* Reset I2Cx IP */
    RCC_APB1PeriphResetCmd(I2Cx_CLK, ENABLE);

    /* Release reset signal of I2Cx IP */
    RCC_APB1PeriphResetCmd(I2Cx_CLK, DISABLE);

       /* GPIO Configuration */
    /* Configure I2C SCL Pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);
    
    /* Connect PXx to I2C_SCL */
    GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
  
    /* Connect PXx to I2C_SDA */
     GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);

    /* Configure I2C SDA */
    GPIO_InitStructure.GPIO_Pin = I2Cx_SDA_PIN;
    GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);

   

    /* Cofigure I2C */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

    /* Configure I2C Filters */
    // I2C_AnalogFilterCmd(I2Cx, ENABLE);
    // I2C_DigitalFilterConfig(I2Cx, 0x0F);

    I2C_Init(I2Cxx, &I2C_InitStructure);
    I2C_Cmd(I2Cxx, ENABLE);
}


/**
 *
 * @param I2Cx - I2C peripheral module number
 * @param I2C_Addr - I2C SLAVE address in network
 * @param addr - slave register address
 * @param buf  - pointer to receiver buffer
 * @param num  - number of bytes to receive
 * @return buffer -
 *
 * @bug Function now works not actually write. input buffer, pointer on which must be used (*buf) do not used.
 * 		Instead of this, function is used with *buf == 0, and returns 1 byte via uint8_t buffer
 *
 * 		Need to be optimized!!!!!
 */

uint8_t I2C_Read(I2C_TypeDef *I2Cx, uint8_t I2C_Addr, uint8_t addr, uint8_t *buf, uint16_t num)
{
    if (num == 0)
        return 1;

    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); /* ch*-2+eck is the line is busy */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);/* enable acknowledgement */
    I2C_GenerateSTART(I2Cx, ENABLE); /* generate start condition */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)); /*check if MASTER mode for STM32 is selected */
    I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter); /* send SLAVE I2C address to SLAVE with TRANSMITION MODE SELECT bit */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); /* check if TRANSMITTER MODE SELECTED for STM32 */
    I2C_SendData(I2Cx, addr); /* transmit SLAVE's register address to read byte */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); /* check if the address byte transmitted - THIS IS THE END of TRANSMITION */
    I2C_GenerateSTART(I2Cx, ENABLE); /* generate start event for receiving */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)); /* check if MASTER mode for STM32 is selected */
    I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver); /* send self address to slave with RECEPTION MODE SELECT bit */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); /* check if RECEIVER MODE SELECTED for STM32 */
    while (num)
    { /* while number of bytes to receive (num) != 0 */

        if (num == 1)
        {
            I2C_AcknowledgeConfig(I2Cx, DISABLE); /* disable acknowledgement */
            I2C_GenerateSTOP(I2Cx, ENABLE);       /* // and generate end of communication */
        }
    
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */   /* // check if the next byte is received */
        *buf = I2C_ReceiveData(I2Cx); /* // white to reception Buffer */
        buf++;				/* // and increment the index of the buffer in case of */
        /* // multibytes reception operation */
        num--; /* // Decrement the read bytes counter */
    }
    
    I2C_AcknowledgeConfig(I2Cx, ENABLE); /* // enable acknowledge for next transmition session */
    
    return 0;
}

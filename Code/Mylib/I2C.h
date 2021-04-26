/* 
June 18,2020
LV_Embedded
 */

#ifndef I2C_H
#define I2C_H

#include "../Src/stm32f4xx.h"


#define I2Cx                     I2C1
#define I2Cx_CLK                 RCC_APB1Periph_I2C1
#define I2Cx_SDA_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define I2Cx_SDA_PIN             GPIO_Pin_9
#define I2Cx_SDA_GPIO_PORT       GPIOB
#define I2Cx_SDA_SOURCE          GPIO_PinSource9
#define I2Cx_SDA_AF              GPIO_AF_I2C1

#define I2Cx_SCL_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define I2Cx_SCL_PIN             GPIO_Pin_8
#define I2Cx_SCL_GPIO_PORT       GPIOB
#define I2Cx_SCL_SOURCE          GPIO_PinSource6
#define I2Cx_SCL_AF              GPIO_AF_I2C1


// void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_address_direction(uint8_t address, uint8_t direction);
void i2c_transmit(uint8_t byte);
uint8_t i2c_receive_ack(void);
uint8_t i2c_receive_nack(void);
void i2c_write(uint8_t address, uint8_t data);
void i2c_read(uint8_t address, uint8_t* data);

#endif
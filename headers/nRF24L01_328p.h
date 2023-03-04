/*
 * nRF24L01_328p.h
 *
 * Created: 22/9/2021 14:33:00
 *  Author: ezema
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nRF24L01.h"

#define DDR_SPI DDRB
#define DD_MOSI DDB3
#define DD_SCK DDB5
#define DD_CSN DDB2
#define CSN PORTB2	//Slave Select = CSN
#define DD_CE DDB1
#define CE PORTB1

#ifndef NRF24L01_328P_H_
#define NRF24L01_328P_H_

void INT0_init();
void SPI_Init(void);
uint8_t SPI_Transfer(uint8_t data);
uint8_t Read_nRF24L01(uint8_t Register_Address);
void Write_nRF24L01(uint8_t Register_Address, uint8_t Register_Content);
void Read_nRF24L01_Special(uint8_t Register_Address, uint8_t Register_Address_Width, uint8_t *Register_Content);
void Write_nRF24L01_Special(uint8_t Register_Address, uint8_t Register_Address_Width, uint8_t *Register_Content);
void Read_nRF24L01_RX_Payload(uint8_t *RX_Payload, uint8_t TX_RX_Payload_Width);
void Write_nRF24L01_TX_Payload(uint8_t *TX_Payload , uint8_t TX_RX_Payload_Width);
void Reset_nRF24L01();
void Start_RX_Mode_nRF24L01(uint8_t *TX_RX_Address);
void Start_TX_Mode_nRF24L01(uint8_t *TX_RX_Address);
void Send_Data(uint8_t *TX_Payload, uint8_t TX_RX_Payload_Width);
void Received_Data(uint8_t *Data, uint8_t Data_Width);


#endif /* NRF24L01_328P_H_ */
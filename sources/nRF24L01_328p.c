/*
 * nRF24L01.c
 *
 * Created: 22/9/2021 13:39:44
 *  Author: ezema
 */ 


#include "../headers/nRF24L01_328p.h"

void INT0_init(){
	
	EICRA |= (1<<ISC01);
	EIMSK |= (1<<INT0);
}

void SPI_Init(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_CSN)|(1<<DD_CE);
	PORTB |= (1<<CSN);
	PORTB &= ~(1<<CE);
	/* Enable SPI, Master*/
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t SPI_Transfer(uint8_t data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

uint8_t Read_nRF24L01(uint8_t Register_Address){
	
	uint8_t Register_Content;
	
	PORTB &= ~(1<<CSN);	//Se habilita el pin CSN
	
	SPI_Transfer(R_REGISTER+Register_Address);
	Register_Content=SPI_Transfer(NOP);

	PORTB |= (1<<CSN);	//Se deshabilita el pin CSN
	
	return(Register_Content);
}

void Write_nRF24L01(uint8_t Register_Address, uint8_t Register_Content){
	
	 PORTB &= ~(1<<CSN);	//Se habilita el pin CSN
	
	SPI_Transfer(W_REGISTER+Register_Address);
	SPI_Transfer(Register_Content);

	PORTB |= (1<<CSN);	//Se deshabilita el pin CSN
}

void Read_nRF24L01_Special(uint8_t Register_Address, uint8_t Register_Address_Width, uint8_t *Register_Content){
	
	PORTB &= ~(1<<CSN);	//Se habilita el pin CSN
	
	SPI_Transfer(R_REGISTER + Register_Address);
	
	for(int i=0; i<Register_Address_Width;i++){
		
		Register_Content[i] = SPI_Transfer(NOP);
	}
	
	PORTB |= (1<<CSN);	//Se deshabilita el pin CSN
}

void Write_nRF24L01_Special(uint8_t Register_Address, uint8_t Register_Address_Width, uint8_t *Register_Content){
	
	PORTB &= ~(1<<CSN);	//Se habilita el pin CSN
	
	_delay_us(10);
	
	SPI_Transfer(W_REGISTER + Register_Address);
	
	for(int i=0; i<Register_Address_Width;i++){
		
		SPI_Transfer(Register_Content[i]);
		_delay_us(10);
	}
	
	_delay_us(10);
	
	PORTB |= (1<<CSN);	//Se deshabilita el pin CSN
	
}

void Read_nRF24L01_RX_Payload(uint8_t *RX_Payload, uint8_t TX_RX_Payload_Width){
	
	PORTB &= ~(1<<CSN);	//Se habilita el pin CSN
	
	SPI_Transfer(R_RX_PAYLOAD);
	
	_delay_us(10);
	
	for(int i=0; i<TX_RX_Payload_Width; i++){
		
		RX_Payload[i] = SPI_Transfer(NOP);
		_delay_us(10);
		
	}
	
	PORTB |= (1<<CSN);	//Se deshabilita el pin CSN
	

}

void Write_nRF24L01_TX_Payload(uint8_t *TX_Payload , uint8_t TX_RX_Payload_Width){
	
	PORTB &= ~(1<<CSN);	//Se habilita el pin CSN
	
	SPI_Transfer(W_TX_PAYLOAD);
	
	for(int i=0; i<TX_RX_Payload_Width; i++){
		
		SPI_Transfer(TX_Payload[i]);
		_delay_us(10);
	}
	
	PORTB |= (1<<CSN);	//Se deshabilita el pin CSN
}


void Reset_nRF24L01(){
	
	PORTB &= ~(1<<CSN);
	SPI_Transfer(FLUSH_TX);
	PORTB |= (1<<CSN);
	
	_delay_us(10);
	
	PORTB &= ~(1<<CSN);
	SPI_Transfer(FLUSH_RX);
	PORTB |= (1<<CSN);
	
	_delay_us(10);
	
	Write_nRF24L01(STATUS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT));
	
}


void Start_RX_Mode_nRF24L01(uint8_t *TX_RX_Address){
		
	PORTB &= ~(1<<CE);	//Deshabilitamos el CE en bajo
	
	Reset_nRF24L01();
	
	Write_nRF24L01(EN_AA, 0x01);
	
	Write_nRF24L01(EN_RXADDR, 0x01);	//Habilito la direccion rx del pipe0
	
	Write_nRF24L01(SETUP_AW, 0x03);		//Ancho de bytes para la designacion de direcciones RX/TX (5 bytes)
	
	Write_nRF24L01(RF_CH, 0x01);		//Frecuencia de 2.4Ghz
	
	Write_nRF24L01(RF_SETUP, 0b00100110);		//Velocidad de transmision y dBm
	
	Write_nRF24L01_Special(RX_ADDR_P0, 5, TX_RX_Address);	//Se asigna la direccion para el canal 0
	
	Write_nRF24L01(RX_PW_P0, 5);	//Se asigna 5 bytes para el payload
	
	Write_nRF24L01(CONFIG, 0b00111111);	//Se habilitan las interrupciones, Modo RX
	
	_delay_us(1500);

};

void Start_TX_Mode_nRF24L01(uint8_t *TX_RX_Address){
	
	PORTB &= ~(1<<CE);	//Deshabilitamos el CE en bajo
	
	Reset_nRF24L01();
	
	Write_nRF24L01(EN_AA, 0x01);
	
	Write_nRF24L01(EN_RXADDR, 0x01);	//Habilito la direccion rx del pipe0
	
	Write_nRF24L01(SETUP_AW, 0x03);		//Ancho de bytes para la designacion de direcciones RX/TX (5 bytes)
	
	Write_nRF24L01(SETUP_RETR, 0x2F);	//Retransmision
	
	Write_nRF24L01(RF_CH, 0x01);		//Frecuencia de 2.4Ghz
	
	Write_nRF24L01(RF_SETUP, 0b00100110);		//Velocidad de transmision y dBm
	
	Write_nRF24L01_Special(RX_ADDR_P0,5, TX_RX_Address);
	
	Write_nRF24L01_Special(TX_ADDR, 5, TX_RX_Address);
	
	Write_nRF24L01(RX_PW_P0, 5);	//Se asigna 5 bytes para el payload
	
	Write_nRF24L01(CONFIG, 0b01001110);	//Se habilitan las interrupciones, Modo TX
	
	_delay_us(1500);
	
}


void Send_Data(uint8_t *TX_Payload, uint8_t TX_RX_Payload_Width){
		
	PORTB &= ~(1<<CSN);
	SPI_Transfer(FLUSH_TX);
	PORTB |= (1<<CSN);
	
	Write_nRF24L01_TX_Payload(TX_Payload, TX_RX_Payload_Width);
	//_delay_us(10);
	
	PORTB |= (1<<CE); // Activate CE to leave Standby I mode and enter TX Mode
	_delay_us(10);
	_delay_us(130);
	PORTB &= ~(1<<CE);
	
	_delay_ms(1000);

}

void Received_Data(uint8_t *Data, uint8_t Data_Width){
	
	PORTB |= (1<<CE); // Activate CE to leave Standby I mode and enter RX Mode
	//_delay_us(130);
	//_delay_us(10);
	_delay_ms(500);
	PORTB &= ~(1<<CE);
	
	_delay_ms(1000);
	
	//_delay_us(10);
//
	//Read_nRF24L01_RX_Payload(Data, Data_Width);
	//_delay_us(10);
	//
	//PORTB &= ~(1<<CSN);
	//SPI_Transfer(FLUSH_RX);
	//PORTB |= (1<<CSN);

}


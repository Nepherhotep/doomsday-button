//#define F_CPU 8000000UL  // 8 MHz

/*Very Important - change F_CPU to match target clock 
  Note: default AVR CLKSEL is 1MHz internal RC
  This program transmits continously on USART. Interrupt is used for 
	Receive charactor, which is then transmitted instead. LEDs are used 
	as a test. RX routine is included but not used.
  Change USART_BAUDRATE constant to change Baud Rate
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED PB0

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

// Define baud rate
#define USART_BAUDRATE 9600   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART_Init(void){
   // Set baud rate
  UBRRL = BAUD_PRESCALE;
  UBRRH = (BAUD_PRESCALE >> 8); 
  UCSRB = ((1<<TXEN)|(1<<RXEN) | (1<<RXCIE));
}


void USART_SendByte(uint8_t u8Data){
  while((UCSRA &(1<<UDRE)) == 0);
  UDR = u8Data;
}

uint8_t USART_ReceiveByte(){
  while((UCSRA &(1<<RXC)) == 0);
  return UDR;
}

int main(void){
   USART_Init();  // Initialise USART
   sei();         // enable all interrupts
   set_output(DDRB, LED);
   output_low(DDRB, LED);
   for(;;){    // Repeat indefinitely
     _delay_ms(1000);
     USART_SendByte('H');
     USART_SendByte('e');
     USART_SendByte('l');
     USART_SendByte('l');
     USART_SendByte('o');
     USART_SendByte(13);
     output_high(DDRB, LED);
     _delay_ms(200);
     output_low(DDRB, LED);
   }
}

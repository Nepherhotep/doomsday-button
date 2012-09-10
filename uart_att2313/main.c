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

void fire_keypress(){
  //start blink
  output_high(DDRB, LED);

  //press button
  USART_SendByte(0xFE);
  USART_SendByte(0x02);
  USART_SendByte(0x02);
  USART_SendByte(0x43);
  
  //release button
  USART_SendByte(0xFE);
  USART_SendByte(0x00);
  
  //delay blink
  _delay_ms(100);

  //stop blink
  output_low(DDRB, LED);
}

//Interrupt Service Routine for INT0
ISR(INT0_vect)
{
  fire_keypress();
  }

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
   USART_Init();// Initialise USART

   // interrupt on INT0 pin falling edge (sensor triggered) 
   MCUCR = (1<<ISC01) | (0<<ISC00);
   // turn on interrupts!
   GIMSK |= (1<<INT0);
   sei();// enable all interrupts
   set_output(DDRB, LED);
   output_low(DDRB, LED);
   
   while(1);
}

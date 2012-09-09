 // this is the header file that tells the compiler what pins and ports, etc.
 // are available on this chip.
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
 
 // define what pins the LEDs are connected to.
 // in reality, PD6 is really just '6'
#define LED PB0
 
 // Some macros that make the code more readable
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

void USART_Init( unsigned int baud )
{
  /* Set baud rate */
  UBRRH = (unsigned char)(baud>>8);
  UBRRL = (unsigned char)baud;

  /* Enable receiver and transmitter */
  UCSRB = ((1<<TXEN)|(1<<RXEN) | (1<<RXCIE));
  //UCSRB = (1<<RXEN)|(1<<TXEN);
  /* Set frame format: 8data, 1stop bit */
  //  UCSRC = (0<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( unsigned int data )
{
  while ( !(UCSRA & (1<<UDRE)) ){
    //nothing do
  };
  UDR = data;
}

int main(void) {

  const unsigned int XTAL = 8000000;    
  const unsigned int BAUDRATE = 9600;  
  const unsigned int BAUDDIVIDER = XTAL/(16*BAUDRATE)-1;
  USART_Init(BAUDDIVIDER);

  // initialize the direction of PORTB
  set_output(DDRB, LED);  
  while(1){
    _delay_ms(50);
    output_high(PORTB, LED); 
    _delay_ms(1000);
    output_low(PORTB, LED);
   
    USART_Transmit('a');
  }
}

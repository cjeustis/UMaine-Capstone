#define F_CPU 16000000UL

//************************************************************************
//  File Name	: Mega8Rs232.c
//  Version	: 1.0
//  Description : AVR ATMega8 UART
//  Author(s)	: RWB
//  Target(s)	: AVR ATmega8 Microcontroller
//  Compiler	: AVR-GCC 4.3.0; avr-libc 1.6.2 (WinAVR 20080610)
//  IDE         : Atmel AVR Studio 4.14
//  Programmer  : AVRJazz Tiny2313 board and AvrOspII
//************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#define BAUD_RATE   9600
#define MAX_GUESS   10
#define LEDPORT     PORTD
#define LED	    PD7
#define OVFTIMES    20
ISR(TIMER0_OVF_vect)
{
  static unsigned int iCount=1;
  iCount++;
  if (iCount >= OVFTIMES) {
    cli();
    LEDPORT |= (1<<LED);
    _delay_ms(50);
    LEDPORT &= ~(1<<LED);
    iCount=1;
    TCNT0=0;
    sei();
  }
}
void uart_init(void)
{
  UBRR0H = (((F_CPU/BAUD_RATE)/16UL)-1)>>8;		// set baud rate
  UBRR0L = (((F_CPU/BAUD_RATE)/16)-1);
  /* Enable Tx and Rx */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set Frame: Data 8 Bit, No Parity and 1 Stop Bit */
  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00) | (1<<USBS0);
}
int uart_putch(char ch, FILE *stream)
{
   if (ch == '\n')
    uart_putch('\r', stream);
   while (!(UCSR0A & (1<<UDRE0)));
   UDR0=ch;
   return 0;
}
int uart_getch(FILE *stream)
{
   unsigned char ch;
   while (!(UCSR0A & (1<<RXC0)));
   ch=UDR0;  

   /* Echo the output back to the terminal */
   uart_putch(ch,stream);      

   return ch;
}
void ansi_cl(void)
{
  // ANSI clear screen: cl=\E[H\E[J
  putchar(27);
  putchar('[');
  putchar('H');
  putchar(27);
  putchar('[');
  putchar('J');
}
void ansi_me(void)
{
  // ANSI turn off all attribute: me=\E[0m
  putchar(27);
  putchar('[');
  putchar('0');
  putchar('m');
}
void ansi_cm(unsigned char row,unsigned char col)
{
  // ANSI cursor movement: cl=\E%row;%colH
  putchar(27);
  putchar('[');
  printf("%d",row);
  putchar(';');
  printf("%d",col);
  putchar('H');
}
int random_number(void)
{
   unsigned char num;   

   num=(unsigned char) rand();
   return num % 99;
}
/* Assign I/O stream to UART */
FILE uart_str = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);
int main(void)
{
  int i,secret,turn;
  /* Define Output/Input Stream */
  stdout = stdin = &uart_str;
  /* Initiatl UART */
  uart_init();
  /* Initial TIMER0 */
  TCCR1B=(1<<CS02)|(1<<CS00);   // Use maximum prescale: Clk/1024
  TCNT0=0;                     // Start counter from 0
  TIMSK1=(1<<TOIE1);            // Enable Counter Overflow Interrupt
  sei();                       // Enable Global Interrupt
  /* Initial Terminal */
  ansi_me();
  ansi_cl();                   // Clear Screen
  /* Initial Port D */
  DDRD = 0xFF;                 // Initial PORT D
  PORTD = 0;                   // Turn Off All PORT D
  /* Initial Variable Used */
  turn=MAX_GUESS;
  secret=random_number();
  for(;;) {
    if (turn == MAX_GUESS) {
      ansi_cl();
      secret=random_number();	 

      ansi_cm(1,12);
      printf("Welcome to Atmel AVR ATMega8 Microcontroller RS232 Project");
      ansi_cm(2,12);
      printf("----------------------------------------------------------");
      ansi_cm(4,1);
      printf("I have a secret number between 0 to 99\n");
      printf("You have %d times to guess my secret number!\n\n",turn);
    }
    printf("\n(%d) Enter your Number: ",turn);
    scanf("%d",&i);
    if (i > secret) {
      printf("\n\nYour number %d is higher then mine ! \n",i);
      turn--;
    }
    if (i < secret) {
      printf("\n\nYour number %d is lower than mine ! \n",i);
      turn--;
    }
    if (i == secret) {
      printf("\n\nCongratulation you found my secret number %d in %d times !\n",i,MAX_GUESS - turn);
      turn=MAX_GUESS;
      cli();
      _delay_ms(2000);
      sei();
    }
    if (turn <= 0) {
      printf("\n\nYou failed, my secret number is %d !\n",secret);
      turn=MAX_GUESS;
      cli();
      _delay_ms(2000);
      sei();
    }
  }
  return 0;	                  // Standard Return Code
}
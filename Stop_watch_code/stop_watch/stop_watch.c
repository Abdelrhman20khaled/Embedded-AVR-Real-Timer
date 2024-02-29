/*
 * Name : Abdelrhman Khaled Sobhi Ali
 * Group : 79
 *
 * */
#include <avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
//declare 6 variable for counting seconds and minute and hours
unsigned char second_unit=0;
unsigned char second_tens=0;
unsigned char minute_unit=0;
unsigned char minute_tens=0;
unsigned char hours_unit=0;
unsigned char hours_tens=0;
void timer1_count(void){
	SREG |=(1<<7);
	TCCR1A |= (1<<COM1A1);
	TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);
	//choose the compare mode
	TIMSK |=(1<<OCIE1A);
	TCNT1 =0;
	//put this value 15625 to give exact one second for one interrupt
	OCR1A = 15625;
}
void interrupt0_reset(void){
	//for enable I_bit
	SREG |= (1<<7);
	//falling edge interrupt
	MCUCR |= (1<<ISC01);
	//for enable the external pin interrupt
	GICR |= (1<<INT0);

}
void interrupt1_paused(void){
	//for enable I_bit
	SREG |= (1<<7);
	//raising edge interrupt
	MCUCR |= (1<<ISC10) | (1<<ISC11);
	//for enable the external pin interrupt
	GICR |= (1<<INT1);
}
void interrupt2_resumed(void){
	//for enable I_bit
	SREG |= (1<<7);
	//falling edge interrupt
	MCUCSR |= (1<<ISC2);
	//for enable the external pin interrupt
	GICR |= (1<<INT2);
}
int main(){
	//the declaration of the timer 1 and interrupt 0,1,2 functions
	timer1_count();
	interrupt0_reset();
	interrupt1_paused();
	interrupt2_resumed();
	//for making the first 4 pin in portC O/P
	DDRC =0X0F;
	//for making the first 6 pin in portA O/P
	DDRA =0X3F;
	//for making a falling edge of INT0
	DDRD &=~(1<<2);
	//put 1 in PORTD pin 2 this will enable the internal pull up resistor
	PORTD |=(1<<2);
	//for making the pin 3 in portD input (raising edge for INT1)
	DDRD &=~(1<<3);
	//for putting initial value on button B2 equal 0
	PORTD &=~(1<<3);
	//for making a falling edge of INT2
	DDRB &=~(1<<2);
	//put 1 in PORTB pin 2 this will enable the internal pull up resistor
	PORTB |=(1<<2);
	//for clear first 6 bits in portA
	PORTA &=0xC0;
	//set the first 4 pin portC initial value equal 1
	PORTC = 0xF0;
	while(1){
		//know i will clear the first 6 pin in portA to make it contain the value of it is common
		PORTA =(PORTA & 0xC0)| 0X01;
		//then clear first 4 pin in portC to include the value you need to display it
		PORTC = (PORTC & 0XF0) | (second_unit & 0X0F);
		//using delay 1 mile second for making display very smooth
		_delay_ms(1);
		//know i will clear portA to make it contain the value of it is common
		PORTA =(PORTA & 0xC0)| 0X02;
		//then clear first 4 pin in portC to include the value you need to display it
		PORTC = (PORTC & 0XF0) | (second_tens & 0X0F);
		//using delay 1 mile second for making display very smooth
		_delay_ms(1);
		//know i will clear portA to make it contain the value of it is common
		PORTA =(PORTA & 0xC0)| 0X04;
		//then clear first 4 pin in portC to include the value you need to display it
		PORTC = (PORTC & 0XF0) | (minute_unit & 0X0F);
		//using delay 1 mile second for making display very smooth
		_delay_ms(1);
		//know i will clear portA to make it contain the value of it is common
		PORTA =(PORTA & 0xC0)| 0X08;
		//then clear first 4 pin in portC to include the value you need to display it
		PORTC = (PORTC & 0XF0) | (minute_tens & 0X0F);
		//using delay 1 mile second for making display very smooth
		_delay_ms(1);
		//know i will clear portA to make it contain the value of it is common
		PORTA =(PORTA & 0xC0)| 0X10;
		//then clear first 4 pin in portC to include the value you need to display it
		PORTC = (PORTC & 0XF0) | (hours_unit & 0X0F);
		//using delay 1 mile second for making display very smooth
		_delay_ms(1);
		//know i will clear portA to make it contain the value of it is common
		PORTA =(PORTA & 0xC0)| 0X20;
		//then clear first 4 pin in portC to include the value you need to display it
		PORTC = (PORTC & 0XF0) | (hours_tens & 0X0F);
		//using delay 1 mile second for making display very smooth
		_delay_ms(1);
	}

}
ISR(TIMER1_COMPA_vect){
	second_unit++;
	if(second_unit >9){
		second_unit=0;
		second_tens++;
	}
	if(second_tens == 6){
		second_tens=0;
		second_unit=0;
		minute_unit++;
	}
	if(minute_unit >9){
		minute_unit=0;
		minute_tens++;
	}
	if(minute_tens == 6){
		minute_tens=0;
		minute_unit=0;
		hours_unit++;
	}
	if(hours_unit >9){
		hours_unit=0;
		hours_tens++;
	}
	if(hours_tens>9){
		second_unit=0;
		second_tens=0;
		minute_unit=0;
		minute_tens=0;
		hours_unit=0;
		hours_tens=0;
	}
}
ISR(INT0_vect){
	//reset all values for all variables
	second_unit=0;
	second_tens=0;
	minute_unit=0;
	minute_tens=0;
	hours_unit=0;
	hours_tens=0;
	//reset the value in TCNT1 for start again from zero when reset
	TCNT1 =0;
}
ISR(INT1_vect){
	//stop clock to paused the timer
	TCCR1B &=~(1<<CS10) & ~(1<<CS11);
	//put this value to zero to make a timer count again from 0 to 15625
	TCNT1 =0;
}
ISR(INT2_vect){
	//enable clock to resumed the timer
	TCCR1B |=(1<<CS10) | (1<<CS11);
}

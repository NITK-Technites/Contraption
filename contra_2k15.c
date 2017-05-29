#include <msp430g2553.h>

#define INTP1 BIT1                                   
#define INTP2 BIT2                                   
#define INTP3 BIT3                                   


#define OUT1 BIT0
#define OUT2 BIT6
#define OUT3 BIT0

void main()
{
	WDTCTL = WDTPW + WDTHOLD;               // Stop WDT
	BCSCTL1 = CALBC1_1MHZ;                  // Set DCO to 1MHz
	DCOCTL = CALDCO_1MHZ;

	P1DIR = OUT1+OUT2+OUT3;
	P1OUT=0;

	P2DIR &=~(INTP1 + INTP2+INTP3);
	P2IE |= (INTP1 + INTP2+INTP3);
	P2IES &= ~(INTP1 + INTP2+INTP3);
	P2REN|= (INTP1 + INTP2+INTP3);
	P2IFG =0;
	P2OUT&=~(INTP1 + INTP2+INTP3);

	_enable_interrupt();

	while(1)
	{
		_bis_SR_register(GIE+LPM0_bits);
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	 int i;
//	_delay_cycles(300);

	if(P2IFG&BIT1)       //WATER_PUMP
	{
		for(i=3000;i>0;i--)
	           _delay_cycles(1000);
		P1OUT = BIT6;
		for(i=10000;i>0;i--)
		   _delay_cycles(2000);
	}
	else if(P2IFG&BIT2)      //BLOWER
	{
		for(i=3000;i>0;i--)
		   _delay_cycles(1000);
		P1OUT = OUT2;
		for(i=5000;i>0;i--)
	           _delay_cycles(1000);
	}
	else if(P2IFG&BIT3)     //LIFT
	{
		for(i=2000;i>0;i--)
		   _delay_cycles(1000);
                P1OUT = BIT6;
		for(i=10000;i>0;i--)
			_delay_cycles(1000);
	}
	 P1OUT &=~ (BIT0+BIT6);
		P2IFG = 0;
	_bic_SR_register_on_exit(LPM0_bits);

   							  // Clear P2IFG
}

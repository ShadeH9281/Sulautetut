#include "msp430G2553.h"
#define MASK 0x38
/* 
#define MASK1 0x08
#define MASK2 0x10
#define MASK3 0x18
#define MASK4 0x20
#define MASK5 0x28
#define MASK6 0x30
*/
# include "stdio.h"
# include "math.h"
#define STOP 1
#define TIGHTL 2
#define DRIVE 3
#define SOFTL 4
#define TIGHTR 5
#define LAP 6
#define SOFTR 7
#define CROSS 8
#define SLOCK 9



int STATE;

int L = 0;
int K = 0;
int Ants;
int A = 0;
int muisti[200];
int i = 0;
long temp;
long IntDegC;

unsigned int b = 0xc430;        // Keston osoiteen apumuuttuja... also known as the starting location for the memory 
int a;


unsigned char *Time_ptr;

main()
{

  //flash reading
   volatile unsigned int g = 0xc430;			// Apumuuttujia Flash-pointtereille
  
  unsigned char *Driv_ptrG;					// Pointterit
  
 STATE = STOP;
WDTCTL = WDTPW + WDTHOLD; 

int u = 0;
 
  
  int e = 0;
  int d = 0;
  int h = 0;
 if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)                                     
  {  
    while(1);                               // If calibration constants erased
                                            // do not load, trap CPU!!
  } 
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator



       char *Flash_ptr;
   unsigned int c;

  Flash_ptr = (char *) 0xc430;              // Alustetaan Flash pointteri
  FCTL1 = FWKEY + ERASE;                    // Asetetaan Erase bit
  FCTL3 = FWKEY;                            // Poistetaan Lock bit
  FCTL1 = FWKEY + WRT;                      // WRT bitillä sallitaan kirjoitus

  for (c=0; c<5000; c++)			    // Toistetaan char-kirjoitus koko alueelle
  {
    *Flash_ptr++ = 0xEE;                    // Kirjoitetaan char
  }

  FCTL1 = FWKEY;                            // Poistetaan WRT bit
  FCTL3 = FWKEY + LOCK;                     // Asetetaan LOCK bit

  /*
    //NTC piirin kelloasetukset
     ADC10CTL1 = INCH_10 + ADC10DIV_3;         // Temp Sensor ADC10CLK/4
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
  __enable_interrupt();                   // Enable interrupts.
  TACCR0 = 30;                              // Delay to allow Ref to settle
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
  LPM0;                                     // Wait for delay.
  TACCTL0 &= ~CCIE;                         // Disable timer Interrupt
  __disable_interrupt();
  */   
    
    
    while(1) 
    {
   
     /* {
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled

    // oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
    temp = ADC10MEM;
    IntDegC = ((temp - 673) * 423) / 1024;
    if (IntDegC > 29 || L == 1) {
      
      if (IntDegC > 29) {
            L = 1;
              }
    //puts ("aja");
      
      */

        
 ////KANAVA 1.2 TIMERI ALKAA
    P1DIR |= 0x44;                            // P1.2 
   // P1SEL |= 0x44;                            // P1.2 
    
    TA0CCR0 = 0xFF;         //NopeusMAX
    TA0CCTL1 = OUTMOD_7;   
    TA0CTL = TASSEL_2 + MC_1;  //Jaa-a?    
    ////KANAVA 1.2 TIMERI LOPPUU
 
    
      
      if (K == 3 || (A == 1 && K == 0))
      {
        
      
      if (K == 3)
      {
      i = 0;
      }
      K = 0;
      A = 1;
      
      if (d != 1 && (A == 1 && K == 0)) {
      g = 0xc430;
      d = 1;
      }
      
      if (g < 0xd7b8)
  {
    
      Driv_ptrG = (unsigned char *) g;		// Alustetaan pointterien alkuosotteet
      
            e = *Driv_ptrG;					// Haetaan apumuuttujille arvot pointtereilla
      
          Ants = e&MASK;
         
    
  }
  
      
      
      
      
      } else {     
    Ants = P2IN&MASK;
      }
      
  
  
  
  

  //STATE = START; 

   
   switch (STATE)
   {
   
   case STOP:
      {
   // puts("STOP"); 
    if (Ants == 0x00) {
    TA0CCR1 = 0x0000; //nopeus1
    P1OUT = 0x00;
      STATE = STOP;
    } else {
    STATE = DRIVE;
    }
    //muisti(i) = 1; //MUISTIINLUKUA   
    
     break;
     }    

case DRIVE:
     {
        
                              
   // puts("DRIVE");
    
    if (Ants == 0x10 || Ants == 0x00) {
      TA0CCR1 = 0x00F0; //nopeus1
    P1OUT = 0x44;
 
    STATE = DRIVE;
    
    } else {
     STATE = CROSS;
     
    }
     break;
     }

case CROSS:
     {
                               
   // puts("CROSS");
    
    if (A == 1000) {
    TA0CCR1 = 0x0078; //nopeus1
    P1OUT = 0x04;   
    STATE = CROSS;
   
    } else {
      STATE = TIGHTR;}
     break;
     }

case TIGHTR:
     {
                             
   // puts("TIGHTR");
    
    if (Ants == 0x20 || Ants == 0x00) {
    TA0CCR1 = 0x0078;
    P1OUT = 0x04;  
    STATE = TIGHTR;
   
    } else {
      STATE = TIGHTL;}
     break;
     }
 
 case TIGHTL:
     {
                              
   // puts("TIGHTL");
    
    if (Ants == 0x08 || Ants == 0x00) {
    TA0CCR1 = 0x0078;
    P1OUT = 0x40;  
    STATE = TIGHTL;
    
    } else {
      STATE = SOFTR; }
     break;
     }
  
 case SOFTR:
     {
                              
   // puts("SOFTR"); 
    
    if (Ants == 0x30 || Ants == 0x00) {
    TA0CCR1 = 0x00B6;
    P1OUT = 0x04;  
    STATE = SOFTR;
   
    } else {
      STATE = SOFTL; }
     break;
     }

 case SOFTL:
     {
 
  //  puts("SOFTL");
       
    if (Ants == 0x18 || Ants == 0x00) {
    TA0CCR1 = 0x00B6;
    P1OUT = 0x40;  
    STATE = SOFTL;
    
    } else {
      STATE = LAP;}
     break;
     }

 case LAP:
     {
    
  //  puts("LAP");
    
    
        
    if (Ants == 0x38) {
    TA0CCR1 = 0x00F0; //nopeus1
    P1OUT = 0x44;
    K = K + 1;
    STATE = SLOCK;
    
      
    } else {
    STATE = DRIVE;
    }
    
    if (A == 1 && K == 1) {
    P1OUT = 0x00;
    Ants = 0x00;
    STATE = LAP;
    }
     break;
     }

   case SLOCK:
      {
   
  //  puts("SLOCK");
       
    if (Ants == 0x38) {
     TA0CCR1 = 0x00F0; //nopeus1
    P1OUT = 0x44;
    STATE = SLOCK;
    
    } else {
      STATE = DRIVE; }
     break;
     }  
 
    
      }//end of Switch  
    Time_ptr = (unsigned char *) b;
    a = Ants;
    if (K == 0 && A == 0) {
    //flash
        
             
 

  if (b < 0xd7b8 || Ants != 0x28)
  {
    FCTL1 = FWKEY + ERASE;
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + WRT;
    
    //mitä vittua?
    if (a != 0x38){
    *Time_ptr = a;
    } else {
    *Time_ptr = 0x3c;
    }
    
    
    h = h+1;
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
  }
  
    }
    
    u = 25000;                          // SW Delay
      do u--;
      
      while (u != 0);
    
    i = i+1;
    g = g+1;
    b = b+1;

 /*  } //lämmönluku loppuu
  } //lämmönlukuloppuu
   */

    }// end of while
    } //end of main

/*
// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{
  TACTL = 0;
  LPM0_EXIT;                                // Exit LPM0 on return
}

*/
  
  
  

//jotain testailuja
/*
 
    ////KANAVA 1.2 TIMERI ALKAA
    P1DIR |= 0x04;                            // P1.2 
    P1SEL |= 0x04;                            // P1.2 
    
    TA0CCR0 = 0xFF;         //NopeusMAX
    TA0CCTL1 = OUTMOD_7;   
    TA0CCR1 = 0x00F0; //nopeus1
    TA0CTL = TASSEL_2 + MC_1;  //Jaa-a?    
    ////KANAVA 1.2 TIMERI LOPPUU
    
    ////KANAVA 1.2 TIMERI ALKAA
    P1DIR |= 0x40;                            // P1.2 
    P1SEL |= 0x40;                            // P1.2 
    
    TA0CCR0 = 0xFF;         //NopeusMAX
    TA0CCTL1 = OUTMOD_7;   
    TA0CCR1 = 0x00F0; //nopeus1
    TA0CTL = TASSEL_2 + MC_1;  //Jaa-a?    
    ////KANAVA 1.2 TIMERI LOPPUU    
    
 
    
    while(1){} // main loop

*/
  
  
//Makes the lights switch constantly

/*
    P1DIR |= 0x44;                        // Set P1.1 and P1.6 to output direction

    
    for (;;)
    {
      Ants = P2IN&MASK;

      if (Ants == 0x38) {
    volatile  unsigned int i;            // volatile to prevent optimization

      P1OUT ^= 0x44;                      // Toggle P1.1 and P1.6 using exclusive-OR

      i = 20000;                          // SW Delay
      do i--;
      
      while (i != 0);
      }  else {
        P1OUT = 0x00;
      }
      
       
        }
    
*/


  // Makes the engines slower
  
  /*
WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
P1DIR |= 0x04;                            // P1.2 
  P1SEL |= 0x04;                            // P1.2 
  CCR0 = 0xff;                             // PWM Period
  CCTL1 = OUTMOD_7;                         // CCR1 reset/set
  CCR1 = 0x08;                              // CCR1 PWM duty cycle
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, up mode

while(1)
  {
  
  }
*/

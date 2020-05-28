// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset          RST connected to PA7
// SSI0Fss        CE  connected to PA3
// Data/Command   DC  connected to PA6
// SSI0Tx         Din connected to PA5
// SSI0Clk        Clk connected to PA2
// 3.3V           Vcc power 3.3V
// back light     BL  not connected, consists of 4 white LEDs which draw ~80mA total
// Ground         Gnd ground

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Sound.h"
#include "ADC.h"

void PortF_Init(void);
void DisableInterrupts(void); 
void EnableInterrupts(void);  
void SysTick_Init(void);
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long TimerCount;
unsigned long Semaphore;

int ADC0_rescale(int val);

void SysTick_Wait10ms(unsigned long delay); 
void SysTick_Wait(unsigned long delay);

int play(int pos , int player); 
void gameResult(int p); 


void gameShow();
void testVertical(void); 
void testHorizontal(void); 
void testDiagonal(void); 
int  RanNum(int move ) ;



const unsigned char blackCircle[] ={
 0x42, 0x4D, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x0F, 0xFF,
 0xF0, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,

};

const unsigned char AI_square[] ={
 0x42, 0x4D, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xF0, 0x00,
 0x0F, 0x00, 0xF0, 0x00, 0x0F, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,

};


const unsigned char cursor[] ={
 0x42, 0x4D, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF,
 0xFF, 0x00, 0xFF,

};


const unsigned char gameBorder[] ={
 0x42, 0x4D, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00,
 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

};

const unsigned char nullImage[] ={0};


int p_ver[3] , p_hor[3] , p_dia[3];  
int p_win[3] ; 
 

int roww , roww2 , temp = 0 ; 	
unsigned long S_1,S_2; 

int col = 0 , row = 0  ; 
int cursorPosition = 0 ; 
int S_1_Flag = 1 ;
int S_2_Flag = 1 ; 
int gameBody[6][7] ;
int nextcycle = 0 ; 
int x = 0 ;
int curserFlag = 0 ;



typedef struct gameObject {
	  const unsigned char* spirit ;
}GameObject;

GameObject Chips[3] = {{nullImage},{blackCircle},{AI_square}}; 
int  move , is = 0  , lock = 0;
//int i , j ; 
int main(void){
	int i = 8; 
	SysTick_Init() ; 
	PortF_Init(); 
	TExaS_Init(SSI0_Real_Nokia5110_Scope); 
	ADC0_Init();    
  Random_Init(1);
  Nokia5110_Init();
	EnableInterrupts();           
  Nokia5110_ClearBuffer();
	for(col=0; col < 7 ; col++){
		  Nokia5110_PrintBMP(12 + (col * 8) , 8       , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + i   , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + 2*i , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + 3*i , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + 4*i , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + 5*i , gameBorder , 0);
	}
  
  while(1){ 	
		
	 for(col=0; col < 7 ; col++){
		
		for(row=0 ; row < 6 ; row++)
		{
			if (gameBody[row][col] > 0 )
			Nokia5110_PrintBMP(13 +(8*col) ,7 +(8*row), Chips[gameBody[row][col]].spirit, 0); 
			}
	 }
	 //  	Nokia5110_DisplayBuffer();
		//cursorPosition = ADC0_rescale( ADC0_In());
	    S_1 = (GPIO_PORTF_DATA_R&0x10);    
      S_2 = (GPIO_PORTF_DATA_R&0x01); 
	 
      if(!S_1 && S_2 && lock == 1){              
         if(S_1_Flag)
				 {
					 int k = 1 ;
				   k = play(cursorPosition , 1); 
					 gameShow();
					 do{
				    	 move = RanNum(2) ;
						   is++;
					 }while(gameBody[is][move] > 0);
					 if(k == 1){
					  play(move , 2); 
						 gameShow();
			  	 }
					 
					 S_1_Flag = 0 ; 
					 nextcycle = 0 ;  
				 }
      }
			else if( S_1 ){ 
					S_1_Flag = 1 ; 
		   	}			 
	 
	    if(!S_2 && S_1){              
         if(S_2_Flag)
				 {
					  curserFlag = 1 ;
				    gameShow();
					  cursorPosition ++;
					 if(cursorPosition == 7)
					 {
						cursorPosition = 0;	
					 }			
					   Nokia5110_PrintBMP( 13 + 8* cursorPosition , 4, cursor, 0);
					 for(col=0; col < 7 ; col++){
							for(row=0 ; row < 6 ; row++)
						   	{
								 if (gameBody[row][col] > 0 )
								 Nokia5110_PrintBMP(13 +(8*col) ,7 +(8*row), Chips[gameBody[row][col]].spirit, 0); 
								}
	            }
					  gameShow();
            S_2_Flag = 0 ;
            lock   = 1 ;					 
				 } 
			 }
			else{ 
					S_2_Flag = 1 ; 
		     	}
						nextcycle = !nextcycle ;			
						testVertical();
						testHorizontal();
						testDiagonal() ; 
			 
			for (x = 1 ; x < 3 ; x++)
			{		
			if(p_win[x] == 1 ) 
			{ 
				gameResult(x);
			}
			}

}
}
void gameShow_boarder()
{
	
	 
}
	
void gameResult(int p){
	while(1){
				int x ; 
		if( p == 1 ){
		for (x =0 ; x <2 ; x++)
			{	Nokia5110_Clear();		
				Nokia5110_SetCursor(1, 0);
				Nokia5110_OutString("YOU WON");
				SysTick_Wait10ms(40);
				Nokia5110_Clear();

			}
		}
		else
    {
			Nokia5110_Clear();		
				Nokia5110_SetCursor(1, 0);
				Nokia5110_OutString("YOU LOSE");
		
				SysTick_Wait10ms(40);
				Nokia5110_Clear();
		}
		
    }

}

void gameShow(){
	   int i = 8 ;
			Nokia5110_DisplayBuffer();
	if( curserFlag == 1  )
	{
			Nokia5110_ClearBuffer();
	  	curserFlag = 0 ;
		for(col=0; col < 7 ; col++){
		  Nokia5110_PrintBMP(12 + (col * 8) , 8       , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + i   , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + 2*i , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + 3*i , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + 4*i , gameBorder , 0);
		  Nokia5110_PrintBMP(12 + (col * 8) , 8 + 5*i , gameBorder , 0);
	}
	}
	

		
}

void testVertical(void){
					
		for(col=0; col < 7 ; col++){
		for(row=0 ; row < 6 ; row++)
		{
		if(row > 0 && gameBody[row-1][col] != gameBody[row][col] ) //on change reset counter
		{p_ver[gameBody[row-1][col]] = 0 ; }
		 p_ver[gameBody[row][col]]++ ; 
		
			if (p_ver[gameBody[row][col]] >= 4 )
			{p_win[gameBody[row][col]] = 1 ; }
		}		
		p_ver[0] = 0 ;
		p_ver[1] = 0 ;
		p_ver[2] = 0 ;
		
		if(p_win[1] || p_win[2] )
					break ; 
		}
}

void testHorizontal(void){

		for(row=0; row < 6 ; row++){
		for(col=0 ; col < 7 ; col++)
		{
		if(col > 0 && gameBody[row][col-1] != gameBody[row][col] ) //on change reset counter
		{p_hor[gameBody[row][col-1]] = 0 ; }
		 p_hor[gameBody[row][col]]++ ; 
		
	
		if(	p_hor[gameBody[row][col]] >= 4 )
				p_win[gameBody[row][col]] = 1 ; 
		
		
		}
		p_hor[0] = 0 ;
		p_hor[1] = 0 ;
		p_hor[2] = 0 ;
		
	if(p_win[1] || p_win[2] )
					break ; 	
		}
}

int play(int pos , int player){
		int i = 0 ;
		for(row=5 ; row > 0 ; row--)
		{	
			if (gameBody[row][pos] > 0  ) 
					continue ;
			
			gameBody[row][pos] = player ; 
			Nokia5110_PrintBMP( 13 +(8 * pos) ,7 + (8 * row) , Chips[player].spirit, 0); // player ship middle bottom 
			Nokia5110_DisplayBuffer();
			i++;
			break ;
		}
		if(i == 0)
		{
			return 0 ;
		}
		return 1 ;
}
void testDiagonal(void){
	temp = 0; 
	
	for (row = 0 ; row <  6 ; row++)
	{ roww = row ;
		for(col=0; col < 7 ; col++){ 
			
			if (roww > 5  || roww <0) 
				continue ; 
			
			
			if ( temp != gameBody[roww][col])
			p_dia[temp] = 0 ; 
			
			
			p_dia[gameBody[roww][col]]++ ; 

			if (p_dia[gameBody[roww][col]] >= 4 )
			{p_win[gameBody[roww][col]] = 1 ;}		
			temp = gameBody[roww][col] ; 
			roww--;
		}
		p_dia[0] = 0 ;
		p_dia[1] = 0 ;
		p_dia[2] = 0 ;
		temp = 0 ; 
	
		if(p_win[1] || p_win[2] )
					break ; 
	}

		for (row = 0 ; row <  6 ; row++)
		{ roww = row ; 
		for(col= 6; col >= 1 ; col--){
		if (roww > 5  || roww <0) 
				continue ; 
			
		
			if (temp != gameBody[roww][col])
				p_dia[temp] = 0 ;
			
			p_dia[gameBody[roww][col]]++ ; 

			if (p_dia[gameBody[roww][col]] >= 4 )
			{p_win[gameBody[roww][col]] = 1 ; }
			
			temp = gameBody[roww][col] ;
			roww++;
			 
		}
		p_dia[0] = 0 ;
		p_dia[1] = 0 ;
		p_dia[2] = 0 ;
		temp = 0 ; 
		if(p_win[1] || p_win[2] )
					break ;
	}

	for (row = 5 ; row >= 0  ; row--)
	{	roww2 = row ; 
		for(col=0; col < 7 ; col++){
	if (roww2 > 5  || roww2 <0) 
				continue ; 
					
			if (temp != gameBody[roww][col])
				p_dia[temp] = 0 ; 
			
			
			p_dia[gameBody[roww2][col]]++ ; 

			if (p_dia[gameBody[roww2][col]] >= 4 )
			{p_win[gameBody[roww2][col]] = 1 ; }
			
			temp = gameBody[roww2][col] ;
			roww2++;
			 
		}
		p_dia[0] = 0 ;
		p_dia[1] = 0 ;
		p_dia[2] = 0 ;
		temp = 0 ; 
		if(p_win[1] || p_win[2] )
					break ;
	}
		
		
	for (row = 5 ; row >= 0  ; row--)
	{	roww2 = row ; 
	for(col= 6; col >= 1 ; col--){
	if (roww2 > 5  || roww2 <0) 
				continue ; 
			
		if (temp != gameBody[roww2][col])
			p_dia[temp] = 0 ; 

			
			p_dia[gameBody[roww2][col]]++ ; 

			if (p_dia[gameBody[roww2][col]] >= 4 )
			{p_win[gameBody[roww2][col]] = 1 ; }
			
			temp = gameBody[roww2][col] ;
			roww2--;
			 
		}
		p_dia[0] = 0 ;
		p_dia[1] = 0 ;
		p_dia[2] = 0 ;
		temp = 0 ; 
		if(p_win[1] || p_win[2] )
					break ;
	
	}
	
	}


void SysTick_Init(void){

  NVIC_ST_CTRL_R = 0;              
	NVIC_ST_CTRL_R = 0x00000005;    	
}


void SysTick_Wait(unsigned long delay){

  NVIC_ST_RELOAD_R = delay-1;  
  NVIC_ST_CURRENT_R = 0;      

  while((NVIC_ST_CTRL_R&0x00010000)==0){ 

  }

}
int RanNum(int move )
{
	int i = 0;
	do{
		move = (Random()%7)+1 ;
		i++;
	}while ( gameBody[i][move] > 0 );
	return move ;
}

void SysTick_Wait10ms(unsigned long delay){

  unsigned long i;
	
  for(i=0; i<delay; i++){

    SysTick_Wait(800000);  

  }

}



int ADC0_rescale(int val){

if (val < 585 )
	return 0 ; 
else if (val < 1170)
	return 1 ; 
else if (val < 1745)
	return 2 ; 
else if (val < 2330)
	return 3 ; 
else if (val < 2905)
	return 4 ; 
else if (val < 3490) 
	return 5 ; 
else if (val < 4100)
	return 6 ; 
else 
	return 6 ; 
}


void Timer2_Init(unsigned long period){ 

  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}
void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCount++;
  Semaphore = 1; // trigger
}


void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

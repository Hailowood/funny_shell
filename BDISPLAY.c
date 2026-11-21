/*
 * make a bit !!! goto control!!!!!
 *
 *
 * use the unsigned int: 4byte!!!
 *
 *
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *	0000 0000 0000 0000 0000 0000 0000 0000
 *
 *
 * PROGRAM NAME : BYTE_DISPLAY
 * COMPLETE DATE: 2026.11.07
 * FINAL_UPDATE : 2026.11.21
 * AUTHOR       : Hailowood
 *
 * Description  : 
 * 		if push the keyborad's (h,j,k,l), it will show Display with Bits. 
 *		h is left,
 *		j is up, 
 *		k is down,
 *		l is right;
 * 
 *
*/



#include<stdio.h>
#include<stdint.h>
#include<termios.h> // control terminal
#include<unistd.h> // system call
#include<fcntl.h> // file control 
#include<stdlib.h> // 
/*
 * ADD :
 *	Author : Hailwood_11_12_07_42
 *	:: terminal_control_part.
 */
struct termios orig_termios;


/*
 * DESCRIPTION :
 *	Author : Hailwood_11_12_07_42
 *	:
 * 	it overcomes terminal setting. (ex), trace the output. )
 */
void disableRawMode(){
	printf("\033[?25h");
	fflush(stdout);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}



/*
 * DESCRIPTION :
 *	Author : Hailwood_11_12_07_42
 *	:
 *	it changes terminal setting(ex), don't trace the output.)
 */
void enableRawMode(){
	tcgetattr(STDIN_FILENO,&orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	
	printf("\033[2J");
	fflush(stdout);
}


/*
 * ADD :
 *	Author : Hailwood_11_12_07_42
 *	:: DISPLAY_control_part.
 */
typedef struct{
	uint8_t high8_1;
	uint8_t high8_2;
	uint8_t low8_1;
	uint8_t low8_2;

}DIC; // Display_Interface_Control;


static inline void Reload_DIC(DIC* dic, uint32_t* display){
	dic->high8_1 = (*display >> 24);
	dic->high8_2 = ((*display >> 16) & 0xFF);
	dic->low8_1  = ((*display >> 8) & 0xFF); 
	dic->low8_2  = (*display & 0xFF);
}



/* show(uint32_t* display, int size)
 *
 *  Display with using Bits
 * */
void show(uint32_t* display, int size){
	DIC dic = {0};
	Reload_DIC(&dic,display);

	for(int i = 0; i< size; ++i){
		printf("%08b %08b %08b %08b \n", dic.high8_1, dic.high8_2, dic.low8_1, dic.low8_2);
		fflush(stdout);
		display++;
		Reload_DIC(&dic,display);
	}
}
/*
 * REMOVE_show(uint32_t* display)
 *
 *  Display with reset_all bits
 * */
void RESET_show(uint32_t* display){
	display[0]  = 0;
	display[1]  = 0;
	display[2]  = 0;
	display[3]  = 0;
	display[4]  = 0;
	display[5]  = 0;
	display[6]  = 0;
	display[7]  = 0;
	display[8]  = 0;
	display[9]  = 0;
}
typedef struct{
	int size;
	int line;
	uint32_t move;
	uint8_t ch;
	char KBD;


}Initial_value;

/*
 * ADD :
 * 	while(fgetc(stdin) == 'Q')
 * 	JUMP : 104,106,107,108
 *	Author : Hailwood_11_8_00_36
 *	:: I think it needs terminal_control_part.
 *
 *	change_ while(1);
 *	:: terminal_ANSI_control_
 *
 *	change_ if(read(STDIN_FILENO, &KBD, 1))
 *	:: remove the input buffer.
 *
 *
 *	add_ using struct Initial_value and conditional statement.
 *	:: ..
 */
int main(int argc,const char* argv[]){
	uint32_t DISPLAY[10] = {0};
	Initial_value Iv = {
		.size = sizeof(DISPLAY)/ sizeof(DISPLAY[0]),
		.line = 0,
		.move = 2147483648,
		.ch = 0,
		.KBD = 0
	
	
	};

	// seting (0,0) of DISPLAY
	DISPLAY[0] = Iv.move;

	enableRawMode();
	printf("\033[H");
	show(DISPLAY, Iv.size);

	
	while (1) {
		 if(read(STDIN_FILENO, &(Iv.KBD), 1)){
			 
			 if(Iv.KBD =='q'){break;}
      		  	 Iv.ch = (uint8_t)Iv.KBD;
        	         printf("\033[H\033[?25l");
			 fflush(stdout);
			 

			 
			 if (Iv.ch == 0x68 && ((Iv.move << 1) != 0x00)) {
				 Iv.move <<= 1;
				 DISPLAY[Iv.line] = Iv.move;
				 show(DISPLAY, Iv.size);
				 printf("104\033[K\n");
        	         } else if (Iv.ch == 0x6A ) {
				 RESET_show(DISPLAY);
				 --Iv.line;
				 Iv.line = (Iv.line >=0 && Iv.line <Iv.size) ? Iv.line: ++Iv.line;
				 
				 DISPLAY[Iv.line] = Iv.move;
				 show(DISPLAY,Iv.size);
        	             	 printf("106\033[K\n");
        	         } else if (Iv.ch == 0x6B) {
				 RESET_show(DISPLAY);
				 ++Iv.line;
				 Iv.line = (Iv.line >=0 && Iv.line <Iv.size) ? Iv.line : --Iv.line;

				 DISPLAY[Iv.line] = Iv.move;
				 show(DISPLAY,Iv.size);
        	             	 printf("107\033[K\n");
        	         } else if (Iv.ch == 0x6C &&((Iv.move >> 1) != 0x00)) {
				 Iv.move >>= 1;
				 DISPLAY[Iv.line] = Iv.move;
        	             	 show(DISPLAY,Iv.size);
				 printf("108\033[K\n");
        	         } else {
				 Iv.line = 0;Iv.move =  2147483648;
				 RESET_show(DISPLAY);
        	             	 fputs("[Warning!] inner_error\033[K\n", stderr); //An ERROR occurs when using independent keys h,j,k,I. 
        	         }
		         usleep(50000);
		         }
		 else{
			 fputs("outter_error\033[K\n", stderr);
		 	 break;
		 }
        	  
	}
return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
	

/* 
 * PROGRAMM : proc_runtime.c
 * Author : Hailowood
 * COMPLETE DATE : 2026_9_27
 *
 * Description : 
 *   A lightweight C utility that calculates the total duration of a process from start to finish.
 * */

int main(int argc, const char* argv[]){

	if(argc < 2){
		perror("Do not access\n");
		exit(1);
	}
	double T = 0;
	struct timespec t_start, t_end;
	t_start.tv_sec = 0, t_start.tv_nsec = 0;
	t_end.tv_sec = 0, t_end.tv_nsec = 0;
	
	int check_error = 0;
	char* arguv[argc+1];
	
	// make the argument_vector = arguv
	memcpy(arguv, argv + 1, (argc-1)*sizeof(char*)); 
	arguv[argc-1] = NULL;
	// Generate Muti-proccess and execuate the file
	pid_t PID = fork();
	if(PID == 0){
		check_error = execvp(*arguv, arguv);
		exit(1);
	
	}
	else{

                // check the t_start, t_end
                clock_gettime(CLOCK_REALTIME, &t_start);
		wait(NULL);
		clock_gettime(CLOCK_REALTIME, &t_end);
                // check the time between t_start to t_end;
                long seconds = t_end.tv_sec - t_start.tv_sec;
                long nanoseconds = t_end.tv_nsec - t_start.tv_nsec;
                
                if (nanoseconds <0){
                	seconds--;
                	nanoseconds += 1000000000;
                }
                
                
                double micro_seconds = (seconds * 1000000.0) + (nanoseconds / 1000.0);
                
		if(micro_seconds > 1000000.0){
		   double total_seconds = micro_seconds / 1000000.0;
		   int minutes = (int)(total_seconds/60);
		   double remain_seconds = total_seconds - (minutes *60);

                   
		   printf("WHAT check_error : if [$error -eq $check]; then  echo -1; else echo 0; fi\n");
		   printf("\n");
                   printf("check_error : %d \n" , check_error); 
		   printf("TIME : %d [min] %lf [sec]\n", minutes, remain_seconds);
		
		
		
		}

		else{
		   printf("WHAT check_error : if [$error -eq $check]; then  echo -1; else echo 0; fi\n");
                   printf("check_error : %d \n", check_error); 
                   printf("TIME : %lf [micro]\n", micro_seconds);
		
		}
	
	}
	

return 0;
}

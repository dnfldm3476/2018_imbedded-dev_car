#include "init.h"

//int count;
uint32_t shot_tick;


void init() {

	/*
	   init camera module and pin
	   init car module and pin
	   register callback function
	*/
	printf("init()\n");
	if(gpioInitialise() < 0){
		fputs("pigpio initialisation failed.\n",stderr);
		exit(1);
	}
	
	gpioSetMode(SWITCH_PINNO,PI_INPUT);
	gpioSetPullUpDown(SWITCH_PINNO,PI_PUD_UP);
	gpioSetISRFunc(SWITCH_PINNO,RISING_EDGE,0,cb_rising_func);
	
	getchar(); //test code
	//gpioTerminate();
	return;
}

void cb_rising_func(int gpio,int level,uint32_t tick)
{
	int status,pid;

	//3second delay
	if(tick - shot_tick < 3000000) return;
	//printf("%d %d \n",++count,tick - shot_tick);
	shot_tick = tick;

	//camera shot and store file
	if((pid=fork())<0){
		fputs("camera fork failed.\n",stderr);
		return;
	}
	if(pid==0){
		status = execlp("raspistill","raspistill",
				"-t","1","-o",USER_FILENAME,0);
		exit(status);
	}
	printf("Take a picture......\n");
	wait(&status);
	if(status!=0){
		printf("error detected! Repush the button!\n");
		return;
	}

	//picture identificatoning code
	printf("identificationing......\n");
}


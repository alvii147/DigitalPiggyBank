#include "gpiolib_addr.h"
#include "gpiolib_reg.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <sys/time.h> 
#include <stdbool.h>

//Macro that will allow us to output appropriate logging messages
//This came from the Lab4 Sample Code
#define PRINT_MSG(file, time, programName, str) \
	do{ \
			fprintf(logFile, "%s : %s : %s", time, programName, str); \
			fflush(logFile); \
	}while(0)

//Macro that allows us to output what coin has passed through the laser (and states its value)
#define PRINT_MSG_COIN(file, time, programName, str, coin) \
	do{ \
			fprintf(logFile, "%s : %s : %s %d\n", time, programName, str, coin); \
			fflush(logFile); \
	}while(0)

//Macro that allows us to state the balance of the piggy bank
#define PRINT_TOTAL_BALANCE (file, time, programName, str, coin) \
	do{ \
			fprintf(logFile, "%s : %s : %s $%.2f\n", time, programName, str, coin); \
			fflush(logFile); \
	}while(0)



//This function will get the current time using the gettimeofday function
//This came from the Lab4 Sample Code
void getTime(char* buffer)
{
	//Create a timeval struct named tv
  	struct timeval tv;

	//Create a time_t variable named curtime
  	time_t curtime;

	//Get the current time and store it in the tv struct
  	gettimeofday(&tv, NULL); 

	//Set curtime to be equal to the number of seconds in tv
  	curtime=tv.tv_sec;

	//This will set buffer to be equal to a string that in
	//equivalent to the current date, in a month, day, year and
	//the current time in 24 hour notation.
  	strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));

}



void readCFG (FILE* configFile, int* dime, int* nickel, int* quarter, int* loonie, int* toonie, int* tolerance, char* logFileName, int* timeout, bool useDefaults) {

    //NOT A STATE MACHINE BECAUSE THIS WAY IS MUCH LESS CONVOLUTED AND STRAIGHTFOWRWARD. STILL USES A SWITCH TABLE THOUGH
	
	enum Focus {TIMEOUT = 0, LOGFILE = 1, TOLERANCE = 2, NICKEL = 3, DIME = 4, QUARTER = 5, LOONIE = 6, TOONIE = 7};
	
	int max, n;
	enum Focus f = TIMEOUT;
	
	if (useDefaults) 
    {
		
		max = 7;
		
	} 
    else 
    {
		max = 2;
	}
	
	int loopNum = 0;
	
	char line[300];
	
	*timeout = 0;
	*dime = 0;
	*nickel = 0;
	*quarter = 0;
	*loonie = 0;
	*toonie = 0;
	
	int i = 0;
	
	while (fgets(line, 300, configFile) != NULL && f <= max) {
		
		i = 0;
		
		if (line[i] != '/' && line[i + 1] != '/') {
			
			while (line[i] != 0) {
				
				if (line[i] == '=') {
					
					switch(f) {
						
						case TIMEOUT:
						
							while (line[i] != 0) {
								
								if (line[i] >= '0' && line[i] <= '9') {
									
									*timeout = (*timeout * 10) + (line[i] - '0');
									
								}
								
								++i;
							}
							
							++f;
							
							break;
						case LOGFILE:
						
							n = 0;
							
							while (line[i] != 0 && line[i] != '\n') {
								
								if (line[i] != ' ' && line[i] != '=') {
									
									logFileName[n] = line[n];
									
									++n;
									
								}
								
								++i;
								
							}
							++f;
							break;
						case TOLERANCE:
						
							while (line[i] != 0) {
								
								if (line[i] >= '0' && line[i] <= '9') {
									
									*tolerance = (*nickel * 10) + (line[i] - '0');
									
								}
								
								++i;
							}
							
							++f;
						
							break;
						case NICKEL:
						
							while (line[i] != 0) {
								
								if (line[i] >= '0' && line[i] <= '9') {
									
									*nickel = (*nickel * 10) + (line[i] - '0');
									
								}
								
								++i;
							}
							
							++f;
						
							break;
						case DIME:
						
							while (line[i] != 0) {
								
								if (line[i] >= '0' && line[i] <= '9') {
									
									*dime = (*dime * 10) + (line[i] - '0');
									
								}
								
								++i;
							}
							
							++f;
						
							break;
						case QUARTER:
						
							while (line[i] != 0) {
								
								if (line[i] >= '0' && line[i] <= '9') {
									
									*quarter = (*quarter * 10) + (line[i] - '0');
									
								}
								
								++i;
							}
							
							++f;
						
							break;
						case LOONIE:
						
							while (line[i] != 0) {
								
								if (line[i] >= '0' && line[i] <= '9') {
									
									*loonie = (*loonie * 10) + (line[i] - '0');
									
								}
								
								++i;
							}
							
							++f;
						
							break;
						case TOONIE:
						
							while (line[i] != 0) {
								
								if (line[i] >= '0' && line[i] <= '9') {
									
									*toonie = (*toonie * 10) + (line[i] - '0');
									
								}
								
								++i;
							}
							
							++f;
						
							break;

					}
					
				} else {
					
					++i;
					
				}
				
				
			}
			
		}

	}
}

int main(const int argc, const char* const argv[])
{

    //This string will contain the program name
    const char* argName = argv[0];

    //These variables will store the length of the program name
    int i = 0;
    int namelength = 0;

    while (argName[i] != 0)
    {
        namelength++;
        i++;
    }

    char programName[namelength];

    i = 0;

    //Copy the name of the program without ./ that is at the beginning of argv[0]
    while (argName[i + 2] != 0)
    {
        programName[i] = argName[i + 2];
        i++;
    }

    //Creating the file pointer for the config file
    FILE* configFile;
    configFile = fopen("/home/pi/coin.cfg", "r"); 

    //Error message if the config file cannot be opened
    if (!configFile)
    {
        perror("The config file could not be opened");
        return -1;
    }

    //These are the variables that will be passed to the readConfig function
    int dime, nickel, quarter, loonie, toonie, tolerance, timeOut;
	dime = 5;
	nickel = 10;
	quarter =25;
	loonie = 1;
	toonie = 2;
    char logFileName[50];
	float totalBalance = 0; //This will store the total balance that the coins

    //Calling the readCFG function
    readCFG(configFile, &dime, &nickel, &quarter, &loonie, &toonie, &tolerance, logFileName, &timeOut, 1);
	
	printf("nickel is: ");
	printf("%d\n", nickel);
	printf("dime is: ");
	printf("%d\n", dime);
	printf("quarter is: ");
	printf("%d\n", quarter);
	printf("loonie is: ");
	printf("%d\n", loonie);
	printf("toonie is: ");
	printf("%d\n", toonie); 
	printf("timeout is: ");
	printf("%d\n", timeOut);

    //Close the configFile after reading from it
    fclose(configFile);

    //Creating a file pointer that will point to the log file
    FILE* logFile;
    logFile = fopen ("/home/pi/coin.log", "a");

    //Check if the logFile was opened properly
    if (!logFile)
    {
        perror("The log file could not be opened");
        return -1;
    }


    //This char array will store the the current time
    char time[30];
    getTime(time);
    
	//These variables store the # of times a particular coin has passed through the laser
    int numDime = 0; 
    int numNickel = 0;
    int numQuarter = 0;
    int numLoonie = 0;
    int numToonie = 0;

    
    //The count variable is the way we are measuring the time the laser is broken
    int count = 0;

	//The prevCount variable is the way we are storing the last count (which is utilized in our clear function)
	int prevCount = 0;

    //Initializing GPIO Pins
	GPIO_Handle gpio;
	gpio = gpiolib_init_gpio();
	if(gpio == NULL)
	{
		perror("Could not initialize GPIO");	//Print if there is an error
	}

    getTime(time);  //Getting the current time

    //Printing that the GPIO pins have been initialized
    PRINT_MSG(logFile, time, programName, "The GPIO pins have been initialized\n\n");

    //This watchdog variable
    int watchdog;

    //Opening the watchdog file. If it is not opened properly, an error has occured.
    if ((watchdog = open("/dev/watchdog", O_RDWR | O_NOCTTY)) < 0) 
	{
		printf("Error: Couldn't open watchdog device! %d\n", watchdog);
		return -1;
	} 


    PRINT_MSG(logFile, time, programName, "The Watchdog file has been opened\n\n");

    ioctl(watchdog, WDIOC_SETTIMEOUT, &timeOut); //Setting the watchdog timer

    PRINT_MSG(logFile, time, programName, "The Watchdog time limit has been set\n\n");
	
	uint32_t level_reg = gpiolib_read_reg(gpio, GPLEV(0));

	//Calling ioctl to prevent system from rebooting
    ioctl(watchdog, WDIOC_KEEPALIVE, 0);
	getTime(time);

	//Log that the Watchdog was kicked
	PRINT_MSG(logFile, time, programName, "The Watchdog was kicked\n\n");
	sleep(0.2);

	int l = 0; //We use this to disable to watchdog after we start the "main" part of our program

	while(1) //We want the program to always be running
	{
			
		level_reg = gpiolib_read_reg(gpio, GPLEV(0));

		//We close the watchdog once we start the main part of the rogram
		if (l == 0)
		{
			//Writing a V to the watch dog file to disable. Then we log this in the logfile
			write(watchdog, "V", 1);
			getTime(time);
			PRINT_MSG(logFile, time, programName, "The Watchdog was disabled\n\n");
				
			//Closing watchdog to ensure it is not accidentally edited
			close(watchdog);
			getTime(time);
			//Log that the Watchdog was closed
			PRINT_MSG(logFile, time, programName, "The Watchdog was closed\n\n");
			l++;		
		}
	
		while (!(level_reg & (1 << 4))) //Increment count if the laser is broken. This will serve as our arbitrary timer
		{
			usleep(1);
			level_reg = gpiolib_read_reg(gpio, GPLEV(0));
			count++;
		}

		if (!(level_reg & (1 << 2)))	//If the reset button is pressed.
		{
			numNickel = 0;
			numDime = 0;
			numQuarter = 0;
			numLoonie = 0;
			numToonie = 0;
			totalBalance = 0;
			PRINT_MSG (logFile, time, programName, "The balance of the piggy bank has been reset\n\n");
			usleep(1);
			printf ("The balance of the piggy bank has been reset\n");
			
			while (!(level_reg & (1 << 2))) //While loop to help with button bouncing
			{
				level_reg = gpiolib_read_reg(gpio, GPLEV(0));
				usleep(10);
			}
		}

		if (!(level_reg & (1 << 3))) //If the clear button is pressed
		{
			if (prevCount < nickel && prevCount > dime) //If the coin was a nickel
			{
				totalBalance -= 0.05;
				numNickel--;
				PRINT_MSG (logFile, time, programName, "The previous coin (a nickel) has been cleared\n\n");
				usleep(1);
				printf ("The previous coin (a nickel) has been cleared\n");
				PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);				
			}
			else if (prevCount < dime && prevCount > 0) //If the coin is a dime
			{
				totalBalance -= 0.10;
				numDime--;
				PRINT_MSG (logFile, time, programName, "The previous coin (a dime) has been cleared\n\n");
				usleep(1);
				printf ("The previous coin (a dime) has been cleared\n");
				PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);		
			}
			else if (prevCount < quarter && prevCount > nickel) //If the coin is a quarter
			{
				totalBalance -= 0.25;
				numQuarter--;
				PRINT_MSG (logFile, time, programName, "The previous coin (a quarter) has been cleared\n\n");
				usleep(1);
				printf ("The previous coin (a quarter) has been cleared\n");
				PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);		
			}
			else if (prevCount < loonie && prevCount > quarter) //If the coin is a loonie
			{
				totalBalance -= 1.00;
				numLoonie--;
				PRINT_MSG (logFile, time, programName, "The previous coin (a loonie) has been cleared\n\n");
				usleep(1);
				printf ("The previous coin (a loonie) has been cleared\n");
				PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);
			}
			else if (prevCount < toonie && prevCount > loonie) //If the coin is a toonie
			{
				totalBalance -= 2.00;
				numToonie--;
				PRINT_MSG (logFile, time, programName, "The previous coin (a toonie) has been cleared\n\n");
				usleep(1);
				printf("The previous coin (a toonie) has been cleared\n");
			    PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);	
			}
			while (!(level_reg & (1 << 3)))	//While loop to help with button bouncing
			{
				level_reg = gpiolib_read_reg(gpio, GPLEV(0));
				usleep(10);
			}
		}


        if (count < nickel && count > dime) //If the coin is a nickel
        {
			prevCount = count;
            numNickel++;
            usleep(10);
            count = 0;
            PRINT_MSG (logFile, time, programName, "A nickel has passed the laser\n\n");
	    	PRINT_MSG_COIN (logFile, time, programName, "The # of nickels is now:", numNickel);
			usleep(1);
			printf("$0.05 added\n");
			totalBalance += 0.05;
			PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);
        }
        else if (count < dime && count > 0) //If the coin is a dime
        {
			prevCount = count;
            numDime++;
            usleep(10);
            count = 0;
            PRINT_MSG (logFile, time, programName, "A dime has passed the laser\n\n");
			PRINT_MSG_COIN (logFile, time, programName, "The # of dimes is now:", numDime);
			usleep(1);
			printf("$0.10 added\n");
			totalBalance += 0.10;
			PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);
        }
        else if (count < quarter && count > nickel) //If the coin is a quarter
        {
			prevCount = count;
            numQuarter++;
            usleep(10);
            count = 0;
            PRINT_MSG (logFile, time, programName, "A quarter has passed the laser\n\n");
			PRINT_MSG_COIN (logFile, time, programName, "The # of quarters is now:", numQuarter);
			usleep(1);
			printf("$0.25 added\n");
			totalBalance += 0.25;
			PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);
        }
        else if (count < loonie && count > quarter) //If the coin is a loonie
        {
			prevCount = count;
            numLoonie++;
            usleep(10);
            count = 0;
            PRINT_MSG (logFile, time, programName, "A loonie has passed the laser\n\n");
			PRINT_MSG_COIN (logFile, time, programName, "The # of loonies is now:", numLoonie);
			usleep(1);
			printf("$1.00 added\n");
			totalBalance += 1.00;
			PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);
        }
        else if (count < toonie && count > loonie)  //If the coin is a toonie
        {
			prevCount = count;
            numToonie++;
            usleep(10);
            count = 0;
            PRINT_MSG (logFile, time, programName, "A toonie has passed the laser\n\n");
			PRINT_MSG_COIN (logFile, time, programName, "The # of toonies is now:", numToonie);
			usleep(1);
			printf("$2.00 added\n");
			totalBalance += 2.00;
			PRINT_TOTAL_BALANCE (logFile, time, programName, "The total balance is now:", totalBalance);
        }
		usleep(10);

	}

}

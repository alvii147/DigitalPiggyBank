

void readCFG (FILE* configFile, int* dime, int* nickel, int* quarter, int* loonie, int* toonie, int* tolerance, char* logFileName, int* timeout, bool useDefaults) {

	//NOT A STATE MACHINE BECAUSE THIS WAY IS MUCH LESS CONVOLUTED AND STRAIGHTFOWRWARD. STILL USES A SWITCH TABLE THOUGH

	enum State {FGETS, DONE, GET_TIMEOUT, GET_LOGFILE, GET_NICKEL, GET_DIME, GET_QUARTER, GET_LOONIE, GET_TOONIE};
	
	enum Focus {TIMEOUT = 0, LOGFILE = 1, TOLERANCE = 2, NICKEL = 3, DIME = 4, QUARTER = 5, LOONIE = 6, TOONIE = 7}
	
	State s = FGETS;
	
	Focus f = TIMEOUT;
	
	if (useDefaults) {
		
		int max = 7;
		
	} else {
		
		int max = 2;
		
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
						
							int n = 0;
							
							while (line[i] != 0 && line[i] != '\n') {
								
								if (line[i] != ' ' && line[i] != '=') {
									
									logFileName[n] = line[n];
									
									++n;
									
								}
								
								++i;
								
							}
						
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
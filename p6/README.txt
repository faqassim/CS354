mySigHandler.c initiates a SIGALRM using an infinite loop and handles SIGINT and SIGUSR1
sendsig.c uses information input by user to send a signal to a specified running process 
division.c uses an infinite loop to ask for two numbers to divide, also handles SIGFPE by changing how divide by zero gets handled
  prints out error message and calls exit(0) instead of crashing.

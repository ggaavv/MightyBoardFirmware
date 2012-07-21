
void startup_delay(void){
	volatile unsigned long i;
	for (i = 0; i < 5000; i++) { ; }
}

void checkSensor(){
    AD1CON1bits.SAMP = 1;
    
    while(AD1CON1bits.DONE == 0){

    }
}

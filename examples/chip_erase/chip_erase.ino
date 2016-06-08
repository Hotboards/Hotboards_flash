/*
  Hotboards eeprom Library - Erase
 Demonstrates the use a spi flash memory. The Hotboards_flash
 library works with all Microchip SST25vfxxxx flash memory series 
 like the one presented flash board (http://www.hotboards.org).
 This sketch write a single byte into memory, then read it back
 and display the value on the serial port.
 The circuit:
 *  VDD   -->  3.3v
 *  SO    -->  MISO (ICSPs conector)
 *  SI    -->  MOSI (ICSPs conector)
 *  SCLK  -->  CLK  (ICSP conector)
 *  CS    -->  D7
 *  GND   -->  GND
 
 Library and example created by Diego from Hotboards
 
 This example code is in the public domain.
 */
#include <Hotboards_flash.h>
#include <SPI.h>
/* lets create an flash instance, the frist paramter is the CS pin and th second one 
   is the memory density (4Mb),  */
Hotboards_flash flash( 7, FLASH_SST25VF_4Mb );

uint8_t data[4] = { 1, 2, 3, 4 };
uint8_t dataR[4];
void setup( void ) 
{
    /* variable to hold the read it value from the memory */
    uint8_t var;
    /*open serial port and send welcome message*/
    Serial.begin( 9600 );
    /* since the driver needs the spi, we need to configure it before */
    SPI.begin( );
    /* the flash works up to 50MHz */
    SPI.beginTransaction( SPISettings( 50000000, MSBFIRST, SPI_MODE0 ) );

    /* initialize flash driver */
    flash.begin();
   /*First, to write a byte is necessary erase it*/
   /*You have to erase a complete sector  or all the memory, cannot erase only one byte*/
   /*Erase all memory*/
    flash.chipErase();
    delay(100);
    
    /*Read the address before to write it, if var = 0xFF means is erased.*/
    Serial.println("Before to write");
    flash.read( 0 ,dataR, 4);
    printData(dataR,4);
    
   /* write a byte with 0xBB value at address 0x00 */
    Serial.println( "Writing" );
    flash.write( 0x00, data, 4 );
    
    /* read the same value already store it at eeprom */
    flash.read( 0x00 ,dataR, 4 );
    printData(dataR,4);
    
    flash.chipErase();

    /* read the same value already store it at eeprom */
    flash.read( 0x00,dataR,4 );
    Serial.println("All memory erased");
    printData(dataR,4);
}

void loop( void ) 
{
    /* since this example only requires one time excution, is not necesary 
       to put anything here */
}

void printData(uint8_t * data,uint8_t size)
{
    Serial.println( "Addr | value" );
    uint8_t i = 0;
    for( i=0 ; i< size ; i++ ) 
    {
        Serial.print( "0x" );
        Serial.print( i, HEX );
        Serial.print( "     " );
        Serial.println( data[ i ],HEX );
    }  
}


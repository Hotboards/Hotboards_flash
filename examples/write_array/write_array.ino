/*
  Hotboards flash Library - Byte array
 Demonstrates the use a spi flash memory. The Hotboards_flash
 library works with all Microchip SST25VFxxxx serial memory series 
 like the one presented flash board (http://www.hotboards.org).
 This sketch write an array of bytes into memory, then read it back
 and display the array on the serial port.
 The circuit:
 *  VDD   -->  3.3v
 *  SO    -->  MISO (ICSPs conector)
 *  SI    -->  MOSI (ICSPs conector)
 *  SCLK  -->  CLK (ICSP conector)
 *  CS    -->  D7
 *  GND   -->  GND
 
 Library and example created by Diego from Hotboards
 
 This example code is in the public domain.
 */
#include <Hotboards_flash.h>
#include <SPI.h>

/* arrays to test eeprom driver */
uint8_t write_array[ 9 ] = {1,2,3,4,5,6,7,8,9};
uint8_t read_array[ 9 ];

/* lets create an eeprom instance, the frist paramter is the CS pin and th second one 
   is the memory density (32Kb),  */
Hotboards_flash flash( 7, FLASH_SST25VF_4Mb );

void setup( void ) 
{
    uint8_t i;
    uint8_t registerStatus;
    
    /*open serial port and send welcome message*/
    Serial.begin( 9600 );
    
    /* since the driver needs the spi, we need to configure it before */
    SPI.begin( );
    
    /* don't forget the Flash only works up to 50MHz */
    SPI.beginTransaction( SPISettings( 5000000, MSBFIRST, SPI_MODE0 ) );
    
    /* initialize flash driver */
    flash.begin( );
    
    /*Read status register*/
    registerStatus = flash.readStatus( );
    delay( 100 );
    
    /* If the Status register is greater than 0x04, probabbly some sectors from the 
    memory are locked.*/
    /*to unlock all the memory sectors we have to write this status
    register with 0x00*/
    if( registerStatus > 0x04 ) 
    {
      flash.writeStatus( 0x00 );
    }
    /*First, to write a byte is necessary erase it*/
   /*You have to erase a complete sector in the memory, cannot erase only one byte*/
   /*First parameter is the sector size, the second is the address where begin*/
    flash.eraseSector(ERASE_4K,0x00);
    delay(100);
    
    /* write an array of data, starting at address 0x00 */
    Serial.println( "Writing array of bytes" );
    flash.write( 0, write_array, 9 );
    
    /* read the same array already store it at eeprom */
    Serial.println( "Reading array of bytes" );
    flash.read( 0, read_array, 9 );
    
    /* this for loop is only to display the array using the serial port */
    Serial.println( "Addr | value" );
    
    for( i=0 ; i<9 ; i++ ) 
    {
        Serial.print( "0x" );
        Serial.print( i, HEX );
        Serial.print( "     " );
        Serial.println( read_array[ i ],HEX );
    }
}

void loop( void ) 
{
    /* since this example only requires one time excution, is not necesary 
       to put anything here */
}

/*
  Hotboards eeprom Library - Write byte
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

void setup( void ) 
{
    /* variable to hold the read it value from the memory */
    uint8_t var;
    uint8_t registerStatus;
    /*open serial port and send welcome message*/
    Serial.begin( 9600 );
    /* since the driver needs the spi, we need to configure it before */
    SPI.begin( );
    /* the flash works up to 50MHz */
    SPI.beginTransaction( SPISettings( 50000000, MSBFIRST, SPI_MODE0 ) );

    /* initialize flash driver */
    flash.begin();
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
    
    /*Read the address before to write it, if var = 0xFF means is erased.*/
    var = flash.read( 0 );
    Serial.print( "var = " );
    Serial.println( var, HEX );
    
   /* write a byte with 0xAA value at address 0x00 */
    Serial.println( "Writing an single byte on addres 0x00" );
    flash.write( 0x00, 0xAA );
    
    /* read the same value already store it at eeprom */
    var = flash.read( 0x00 );
    Serial.print( "var = " );
    Serial.println( var, HEX ); 
}

void loop( void ) 
{
    /* since this example only requires one time excution, is not necesary 
       to put anything here */
}


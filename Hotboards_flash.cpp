#include "Hotboards_flash.h"
#include <SPI.h>
#include <inttypes.h>


const uint32_t flashSize[] = { 65535, 131071, 262143, 524287, 1048575, 2097151, 4194303, 8388607};

Hotboards_flash::Hotboards_flash( uint8_t cs, uint8_t type )
{
    _cs_pin = cs;
    _type   = type;
    _density = flashSize[ type ];
}


void Hotboards_flash::begin( void )
{
   pinMode( _cs_pin, OUTPUT );
   digitalWrite( _cs_pin, HIGH );
}


void Hotboards_flash::write( uint32_t address, uint8_t data )
{
    write( address, &data, 1 );
}


void Hotboards_flash::write( uint32_t address, uint8_t *data, uint32_t size )
{
    uint16_t temp;
    /* just to cover your ass, check if the address is a valid direction */
    if( ( address < _density ) && ( size != 0 ) )
    {
        /* check if the amount of bytes to be read from the requested address do not exced the eeprom
        memory map if so, then only read the bytes left*/
        if( ( address + size ) > _density )
        {
            size = _density - address;
        }
        
        /*Write the data */
        writeFlash( address, data, size );
    }
}


uint8_t Hotboards_flash::read( uint32_t address )
{
    uint8_t data;
    read( address, &data, 1 );
    return data;
}


void Hotboards_flash::read( uint32_t address, uint8_t *data, uint32_t size )
{
    uint16_t i;
    /* just to cover your ass, check if the address is a valid direction */
    if( ( address < _density ) && ( size != 0 ) )
    {
        /* check if the amount of bytes to be read from the requested address do not exced the eeprom
        memory map if so, then only read the bytes left*/
        if( ( address + size ) > _density )
        {
            size = _density - address;
        }

        /* operation begins, select memory */
        digitalWrite( _cs_pin, LOW );

        /* Send command Read and address, depend on device is the number of address bytes neccesary */
        sendAddress( READ, address );

        for( i = 0 ; i<size ; i++ )
        {
            /* Read data from memory */
            *data = SPI.transfer( 0xAA );
            data++;
        }
        /* operation ended, unselect memory */
        digitalWrite( _cs_pin, HIGH );
    }
}

void Hotboards_flash::sendAddress( uint8_t cmd, uint32_t address )
{
        /* Send command plus three byte address */
        SPI.transfer( cmd );
        SPI.transfer( ( uint8_t )( address >> 16 ) );
        SPI.transfer( ( uint8_t )( address >> 8 ) );
        SPI.transfer( ( uint8_t )address );
}

void Hotboards_flash::writeFlash( uint32_t address, uint8_t *data, uint32_t size )
{
        uint8_t mod;
         /*Unlock memory*/
        writeStatus( UNLOCK );
        /* Select memory */
        digitalWrite( _cs_pin, LOW );
        /* write eneable latch */
        SPI.transfer( WREN );
        /* Unselect flash */
        digitalWrite( _cs_pin, HIGH );
        /*Waiting for the end of command*/
        while( isBusy() == 1 );
        /*Size only can be a number divisible by two*/
        mod = size % 2;
 
        if(( size > 1 ) & ( mod == 0 )) 
        {
            /* Select memory */
            digitalWrite( _cs_pin, LOW );
            /*Set auto address increment to write multiple words*/
            sendAddress( AAI, address);
            /* Auto address increment only write WORDS two bytes size*/
            for( uint32_t i = 0; i < size  ; i++ ) 
            {
                /*Send data Most significative byte*/
                SPI.transfer(*data);
                /*Increment pointer*/
                data++;
                /*Send less significative byte*/
                SPI.transfer(*data);
                /*Increment pointer*/
                data++;
                /*Unselect memory*/
                digitalWrite( _cs_pin, HIGH );
                /*Wait to finish the process*/
                while( isBusy() == 1 );
                /*Prepare to write another WORD*/
                digitalWrite( _cs_pin, LOW );
                /*Auto address incroement*/
                SPI.transfer( AAI );
            }
            /*Unselect memory to cancel last AAI command*/
            digitalWrite( _cs_pin, HIGH );
            /*Wait to finish the process*/
            while( isBusy() == 1 );
            /*Select memory*/
            digitalWrite( _cs_pin, LOW );
            /*Disable write to finish Auto address increment*/
            SPI.transfer(WRDI);
            /*Unselect memory*/
            digitalWrite( _cs_pin, HIGH );
        }
        if(size == 1)/*Write only one byte*/
        {
            /* Select memory */
            digitalWrite( _cs_pin, LOW );
            /* Send comand write and address, depend on device is the number of address bytes neccesary */
            sendAddress( WRITE, address);
            /* Write data to memory */
            SPI.transfer( *data );
            /* Unselect flash */
            digitalWrite( _cs_pin, HIGH );
        }
        /*Wait to finish the process*/
        while( isBusy() == 1 );  
}

uint8_t Hotboards_flash::isBusy(void)
{
    uint8_t state = 0x00;
    /*If state == 1 the flash memory is busy*/
    state = readStatus() & 0x01;
    return state;
}

/*Read memory flash status register*/
uint8_t Hotboards_flash::readStatus( void )
{
    uint8_t statusResult;
    /*Select memory*/
    digitalWrite( _cs_pin, LOW );
    /*Read status register */
    SPI.transfer( RDST );
    /*Get result*/
    statusResult = SPI.transfer( 0xFF );
    /*Unselect memory*/
    digitalWrite( _cs_pin, HIGH );
    
    return statusResult;
}

void Hotboards_flash::writeStatus( uint8_t bitConfig )
{
    /*Select memory*/
  digitalWrite( _cs_pin, LOW );
  /*Enable write status register*/
  SPI.transfer( EWSR );
  /*Unselect memory*/
  digitalWrite( _cs_pin, HIGH );
  /*Select memory*/
  digitalWrite( _cs_pin, LOW );
  /*Write status register*/
  SPI.transfer( WRSR );
  /*Configuration bits*/
  SPI.transfer( bitConfig );
  /*Unselect memory*/
  digitalWrite( _cs_pin, HIGH );
}

void Hotboards_flash::eraseSector( uint8_t sector, uint32_t address )
{
    /*Unlock memory*/
    writeStatus( UNLOCK );
    /*Select memory*/
    digitalWrite( _cs_pin, LOW );
    /*Enable write status register*/
    SPI.transfer( WREN );
    /*Unselect memory*/
    digitalWrite( _cs_pin, HIGH );
    /*Waiting for the end of command*/
    while( isBusy() == 1 );
    /*Select memory*/
    digitalWrite( _cs_pin, LOW );
    /*Size of sector to erase*/
    SPI.transfer( sector );
    /*Address from sector to erase*/
    SPI.transfer((uint8_t)(address >> 16));
    SPI.transfer((uint8_t)(address >> 8));
    SPI.transfer((uint8_t) address);

    /*Unselect memory*/
    digitalWrite( _cs_pin, HIGH );
    while( isBusy() == 1 );
}

void Hotboards_flash::chipErase(void)
{
    /*Unlock memory*/
    writeStatus( UNLOCK );
    /*Select memory*/
    digitalWrite( _cs_pin, LOW );
    /*Enable write status register*/
    SPI.transfer( WREN );
    /*Unselect memory*/
    digitalWrite( _cs_pin, HIGH );
    /*Waiting for the end of command*/
    while( isBusy() == 1 );
    /*Select memory*/
    digitalWrite( _cs_pin, LOW );
  /*Instruction to erase all*/
    SPI.transfer(ERASE_CHIP);
    /*Unselect memory*/
    digitalWrite( _cs_pin, HIGH );
     while( isBusy() == 1 );
}
#ifndef Hotboards_flash_h
#define Hotboards_flash_h

#include <Arduino.h>

#define WRSR        0x01
#define WRITE       0x02
#define READ        0x03
#define WRDI        0x04
#define RDST        0x05
#define WREN        0x06
#define EWSR        0x50
#define ERASE_4K    0x20
#define ERASE_32K   0x52
#define ERASE_64K   0xD8
#define ERASE_CHIP  0x60



typedef enum
{
    FLASH_SST25VF_512Kb = 0,
    FLASH_SST25VF_1Mb,
    FLASH_SST25VF_2Mb,
    FLASH_SST25VF_4Mb,
    FLASH_SST25VF_8Mb,
    FLASH_SST25VF_16Mb,
    FLASH_SST25VF_32Mb,
    FLASH_SST25VF_64Mb
}_eFLash;

class Hotboards_flash
{
    public :
        /** Create Hotboards_flashinstance
          * @param cs pin where to control the chip select
          * @param type eeprom type (use values from _eFlash enumeration)
          *
          * Example:
          * @code
          *   // instance 4Mb memory with CS signal on pin 7
          *   Hotboards_flash mem( 7, HT_FLASH_SST25VF_4Mb );
          * @endcode
          */
        Hotboards_flash( uint8_t ce, uint8_t type);

        /** Just init CS pin
          *
          * Example:
          * @code
          *   // instance 4Mb memory with CS signal on pin 7
          *   Hotboards_flash mem( 7, HT_FLASH_SST25VF_4Mb );
          *   mem.begin( ); // we are now ready to write and read
          * @endcode
          */
        void begin( void );

        /** write a single byte in a given flash address, this operation will take 10ms
          * @param address flash address where tha byte will be written
          * @param data the byte that will be written at the given address
          *
          * Example:
          * @code
          *   // Write character A on address 0x00
          *   mem.write( 0, 'A' );
          * @endcode
          */
        void write( uint32_t address, uint8_t data );

        /** write a given number of bytes into the flash starting at a given address
          * @param address start flash address where tha byte array will be written
          * @param data pointer to array of bytes that need to be written
          * @param size number of bytes to write
          *
          * Example:
          * @code
          *   uint8_t array[] = {0,1,2,3,4,5};
          *   // Write array "array" starting on address 0x00
          *   mem.write( 0, array, 6 );
          * @endcode
          */
        void write( uint32_t address, uint8_t *data, uint32_t size );

        /** read a single byte from a given flash address
          * @param address flash address where the byte will be read
          *
          * Example:
          * @code
          *   // Read a character from address 0x00
          *   uint8_t var = mem.read( 0 );
          * @endcode
          */
        uint8_t read( uint32_t address );

        /** read a given number of bytes from the flash starting at a given address
          * @param address flash address where the bytes will be read it
          * @param data pointer to array where data will be stored
          * @param size the number of bytes to read
          *
          * Example:
          * @code
          *   uint8_t array[6];
          *   // read 6 bytes from address 0x00 and store in array "array"
          *   mem.read( 0, array, 6 );
          * @endcode
          */
        void read( uint32_t address, uint8_t *data, uint32_t size );
        
        /** read the status register from the memory 
          *returns the value from the register used to know if the memory is locked or busy
          * Example:
          * @code
          * uint8_t state;
          * state = mem.readStatus( );
          * @endcode
          */
        uint8_t readStatus( void );
        
        /** Write the register status, used to lock the memory
          * @param bits to configure
          * Example:
          * @code
          * //Unlock all memory
          *   mem.writeStatus( 0 );
          * @endcode
          */
        void writeStatus( uint8_t bitConfig );
        
        /** Erase a sector from the memory starting at the given address
          * the sectors can be : ERASE_4K,ERASE_32K,ERASE_64K
          * @param size of the sector to be erased
          * @param address to begin the erase operation
          *
          * Example:
          * @code
          *   // erase a sector of 4Kb starting from address 0x00
          *   mem.eraseSector( ERASE_4K, 0x00);
          * @endcode
          */
       void eraseSector( uint8_t sector, uint32_t address );
       
         /** Erase all memory
          * this instruction will take 100 ms

          *
          * Example:
          * @code
          *   // erase a sector of 4Kb starting from address 0x00
          *   mem.chipErase( );
          * @endcode
          */
       void chipErase(void);

    private :
        void    sendAddress( uint8_t cmd, uint32_t address );
        void    writeFlash( uint32_t address, uint8_t *data, uint32_t size );
        uint8_t isBusy(void);
        uint8_t _cs_pin;
        uint8_t _type;
        uint32_t _density;
};

#endif
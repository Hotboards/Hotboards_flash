##Hotboards_flash##

Librería de propósito general útil para controlar las memorias flash de microchip SST25VF, leer, escribir y borrar, controla por completo este dispositivo, solo descarga la librería en formato zip, renombra el archivo ( solo quita el -master ) e impórtala a tu Arduino IDE para comenzar a utilizarla.

##API##

**Hotboards_eeprom( uint8_t cs, uint8_t type )**

Constructor para nuestra memoria eeprom. Se puede especificar la densidad de la memoria así como el pin que controlara la señal de chip select ( CS ).

```
// Creamos una memoria de 4Mb con la señal de CS en el pin 7
```

```
Hotboards_flash mem( 7, FLASH_SST25VF_4Mb );```

**void begin( void )**

Inicializa la memoria flash. En realidad solo el pin de CS ya que la memoria no ocupa rutina de inicializacion.

```
// inicializamos
```

``` 
mem.begin( );
```

**void write( uint32_t address, uint8_t data )**

Escribimos un solo byte en la memoria. El parámetro indica la dirección en la cual escribiremos el byte. Debes tomar en cuenta que esta operación puede tomar hasta 10ms.

```
// escribimos el caracter A en la dirección 0x00
```

```
mem.write( 0, 'A' );
```

**void write( uint32_t address, uint8_t *data, uint16_t size )***

Escribimos un cierto numero de bytes a partir de la dirección indicada. La función controla este proceso internamente permitiendo escribir el numero de bytes que queramos.

```
// declaramos un arreglo para escrbirlo en la memoria
```

```
uint8_t array[] = {0,1,2,3,4,5};
```
```
// Escribimos 6 bytes de "array" en la memoria empezando en la dirección 0x00 
```
```
mem.write( 0, array, 6 );
```

**uint8_t read( uint32_t address )**

Lee un solo byte de la memoria flash. El parámetro especifica la dirección de la cual se lee el byte.
```
// Leemos un byte de la dirección 0x00
```

```
uint8_t var = mem.read( 0 );
```

**void read( uint32_t address, uint8_t *data, uint16_t size )***

Leemos un cierto numero de bytes de la memoria.
```
// en este arreglo almacenaremos los datos leídos
```

```
uint8_t array[6];
```

```
// leemos 6 bytes a partir de la direccion 0x00 y los guardamos en el arreglo "array"
```

```
mem.read( 0, array, 6 );
```

**void eraseSector( uint8_t sector, uint32_t address )**

Borramos un sector de la memoria, ya sea de 4Kb, 32Kb o 64Kb.

```//Borrar el un sector de 32Kb desde la dirección 0 
```

```
mem.eraseSector( ERASE_32K, 0x00 )
```
**void chipErase(void)**

Borramos el chip completo.

```
mem.chipErase();
```

##Ejemplos##



- [Escribiendo y leyendo un solo byte en la memoria](https://github.com/Hotboards/Hotboards_flash/blob/master/examples/write_byte/write_byte.ino)
- [Escribiendo y leyendo un arreglo en la memoria](https://github.com/Hotboards/Hotboards_flash/blob/master/examples/write_array/write_array.ino)
- [Borrando el chip completo](https://github.com/Hotboards/Hotboards_flash/blob/master/examples/chip_erase/chip_erase.ino) 
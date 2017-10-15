  /* readerTest: proba de concepto para intentar ler un ficheiro .jef dende un
  / ESp32.
  / A idea é conseguir ler o archivo e imprimir nunha pantalla OLED cada 'línea'
  / do tecido.
  /
  /
  /
  /                                                           areyes 04/10/17 */

  #include <Arduino.h>

  /* O archivo de exemplo ocupa 13KB, polo que o podería almacenar na RAM.
  / Pero non sei como, así que almacenalo na FLASH mediante SPII.
  / Pero SPII todavía non funciona en ESP32, así que usei unha SD
  */

  #include <SPI.h>
  #include <mySD.h>

  #include "stitch.h"
  #include "utils.cpp"

  File meuJef;

  /* Constantes varias*/
  int i; // Un contador xenérico sempre ven ben.
  
  /* Campos da cabeceira*/


  int posicion;

  headerStitch hS;
  puntada p = {0x0,6,0,0};
  /* Sobrecarga de funcións.
  /* A versión que lee un intervalo de bytes non funciona   */



  void setup()
  {
   // Open serial communications and wait for port to open:
    Serial.begin(115200);
     while (!Serial)
     {
      ; // wait for serial port to connect. Needed for Leonardo only
    }

    Serial.print("Initializing SD card...");
    pinMode(SS, OUTPUT);
    /* cs  -> 26
      sck  -> 25
      mosi -> 0
      moso -> 4
      vcc  -> 5V
      gnd  -> gnd
    */
    if (!SD.begin(26, 0, 4, 25))
    {
      Serial.println("initialization failed!");
      return;
    }
    meuJef = SD.open("decomer.jef", FILE_READ);
    if(meuJef)
    {
      lerBytes(hS.stitchOffset, meuJef,0,4, DEC);
      lerBytes(hS.flags, meuJef, 4,4, DEC);
      lerBytes(hS.fecha, meuJef, 8,8, FECHA);
      lerBytes(hS.hora, meuJef, 16,8, HORA);
      lerBytes(hS.threadCount, meuJef, 24,4, DEC);
      lerBytes(hS.stitchCount, meuJef, 28,4, DEC);
      lerBytes(hS.hoopCode, meuJef, 32,4, DEC);
      posicion=hS.stitchOffset[1];
    }
  //  meuJef.close();
    Serial.println("initialization done.");
  }

  void loop()
  {
    /* Abro e cerro o archivo de cada vez. Supoño que en canto a rendemento non pode ser peor.
    */
     meuJef = SD.open("decomer.jef", FILE_READ);
     if(meuJef && posicion< meuJef.size())
     {
      delay(500);
      if (p.escaped==0x80)
      {
        Serial.println("*****");
        Serial.println(posicion);
      }
      if (p.codigo==1)
      {
        Serial.println("Novo fío");
        Serial.println(p.codigo,DEC);
      }
      else if (p.codigo==2)
      {
        Serial.print("X -> ");
        Serial.print(p.desplX,DEC);
        Serial.print(" | ");
        Serial.print("Y -> ");
        Serial.println(p.desplY,DEC);
      }
      if (p.codigo != 16)
      {
        //Serial.println(puntada[0]);
        lerBytes(p.escaped, meuJef,posicion,HEX);
        posicion++;
        lerBytes(p.codigo, meuJef,posicion,DEC);
        posicion++;
        lerBytes(p.desplX, meuJef,posicion,DEC);
        posicion++;
        lerBytes(p.desplY, meuJef,posicion, DEC);
        posicion++;
        Serial.println(p.escaped,DEC);
        Serial.println(p.codigo,DEC);
        Serial.println(p.desplX,DEC);
        Serial.println(p.desplY,DEC);
        Serial.println(DIV);
      }else
      {
        Serial.println("Sacabó");
      }
    meuJef.close();
    }else
    {
    // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
  	// nothing happens after setup
    }
  }

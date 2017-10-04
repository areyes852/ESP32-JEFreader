  /* readerTest: proba de concepto para intentar ler un ficheiro .jef dende un
  / ESp32.
  / A idea é conseguir ler o archivo e imprimir nunha pantalla OLED cada 'línea'
  / do tecido.
  /
  /                                                           areyes 04/10/17 */

#include <Arduino.h>

  /* O archivo de exemplo ocupa 13KB, polo que o podería almacenar na RAM.
  / Pero non sei como, así que almacenalo na FLASH mediante SPII.
  / Pero SPII todavía non funciona en ESP32, así que usei unha SD
  */

  #include <SPI.h>
  #include <mySD.h>

  File myFile;

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
    if (!SD.begin(26, 0, 4, 25)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    myFile = SD.open("decomer.jef", FILE_READ);

    if (myFile) {
      Serial.println("**********************");

      // read from the file until there's nothing else in it:
      while (myFile.available()) {

      	Serial.write(myFile.read());
      }
      // close the file:
      myFile.close();
    } else {
    	// if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
  }

  void loop()
  {
  	// nothing happens after setup
  }

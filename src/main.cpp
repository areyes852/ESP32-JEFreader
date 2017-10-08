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

  File meuJef;

  /* Constantes varias*/
  int i; // Un contador xenérico sempre ven ben.
  const int HORA=1;
  const int FECHA=2;
  const char DIV[] = "-----------------------------------";
  /* Campos da cabeceira*/
  //TODO: Exiten as estructuras en c? Probablemente esto estea mellor nunha estrucutra definida noutro ficheiro. De esa forma podese definir unha variable e despois ler os campos desexados
  char stitchOffset[4];
  char flags[4];
  char fecha[8];
  char hora[16];
  char threadCount[4];
  char stitchCount[4];
  char hoopCode[4];
  char extent1Left[4];
  char extent1Top[4];
  char extent1Right[4];
  char extent1Bottom[4];
  char extent2Left[4];
  char extent2Top[4];
  char extent2Right[4];
  char extent2Bottom[4];
  char extent3Left[4];
  char extent3Top[4];
  char extent3Right[4];
  char extent3Bottom[4];
  char extent4Left[4];
  char extent4Top[4];
  char extent4Right[4];
  char extent4Bottom[4];
  char extent5Left[4];
  char extent5Top[4];
  char extent5Right[4];
  char extent5Bottom[4];
  char threadColour[4];
  char threadType[4];
  char puntada[3];
  int posicion;
  /* Sobrecarga de funcións.
  /* A versión que lee un intervalo de bytes non funciona   */

  void lerBytes(char byteLido[],File archivo,int lonxitude);
  void lerBytes(int byteLido[],File archivo,int lonxitude);
  void lerBytes(char byteLido[],File archivo,int lonxitude, int formato);

  void lerBytes(char byteLido[],File archivo,int lonxitude)
  {
    int i = 0;
    for(i = 0;i<lonxitude;i++)
    {
      byteLido[i]=archivo.read();
    }
  }
  void lerBytes(int byteLido[],File archivo,int lonxitude)
  {
    int i = 0;
    for(i = 0;i<lonxitude;i++)
    {
      byteLido[i]=archivo.read();
    }
  }

  void lerBytes(char byteLido[],File archivo, int lonxitude, int formato)
  {
    int i = 0;
    for(i = 0; i < lonxitude ;i++)
    {
      byteLido[i]=archivo.read();
    }
    if (formato == HORA)
    {
      for(i=0;i<2;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.print(":");
      for(i=2;i<4;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.print(":");
      for(i=4;i<6;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.println();
      Serial.println(DIV);
      return;
    }
    if (formato == FECHA)
    {
      for(i=0;i<4;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.print("/");
      for(i=4;i<6;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.print("/");
      for(i=6;i<8;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.println();
      Serial.println(DIV);
      return;
    }
    if (formato == DEC || formato == HEX)
    {
      for (i=0;i<lonxitude;i++)
      {
        Serial.print(byteLido[i],formato);
        Serial.print(",");
      }
      Serial.println();
      Serial.println(DIV);
    }
    else
    {
      Serial.print(byteLido);
      Serial.println();
      Serial.println(DIV);
    }
  }
  // TODO: Añadir sobrecarga que lea un rango de bytes (probablemente sexa necesario ler todos os bytes anteriores ó rango desexado). Esta implementación non vale porque non volve ó inicio do ficheiro de cada vez.
  void lerBytes(char byteLido[],File archivo, int inicio,int fin, int formato)
  {
    int i = 0;
    int j = 0;
    int lonx = fin-inicio;
    for(i = 0; i < inicio;i++)
    {
      archivo.read();
    }
    for(i = inicio; i < fin ;i++)
    {
      byteLido[j]=archivo.read();
      j++;
    }
    if (formato == HORA)
    {
      for(i=0;i<2;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.print(":");
      for(i=2;i<4;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.print(":");
      for(i=4;i<6;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.println();
      Serial.println(DIV);
      return;
    }
    if (formato == FECHA)
    {
      for(i=0;i<4;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.print("/");
      for(i=4;i<6;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.print("/");
      for(i=6;i<8;i++)
      {
        Serial.print(byteLido[i]);
      }
      Serial.println();
      Serial.println(DIV);
      return;
    }
    if (formato == DEC || formato == HEX)
    {
      for (i=0;i<lonx;i++)
      {
        Serial.print(byteLido[i],formato);
        Serial.print(",");
      }
      Serial.println();
      Serial.println(DIV);
    }
    else
    {
      Serial.print(byteLido);
      Serial.println();
      Serial.println(DIV);
    }
  }


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
    meuJef = SD.open("decomer.jef", FILE_READ);

    if (meuJef) {
      Serial.println("**********************");
      puntada[0]=1;puntada[1]=0;puntada[2]=0; /* 0- escape code;1-desplazamentoX;2-desplazamentoY */
      /*Leer cabeceira*/
      lerBytes(stitchOffset, meuJef, 4,DEC);
      lerBytes(flags, meuJef, 4,DEC);
      lerBytes(fecha,meuJef,8,FECHA);
      lerBytes(hora,meuJef,16,HORA);
      Serial.println("threadCount");
      lerBytes(threadCount,meuJef,4,DEC);
      lerBytes(stitchCount,meuJef,4,DEC);
      lerBytes(hoopCode,meuJef,4,DEC);
      lerBytes(extent1Left,meuJef,4,DEC);
      lerBytes(extent1Bottom,meuJef,4,DEC);
      lerBytes(extent1Right,meuJef,4,DEC);
      lerBytes(extent1Top,meuJef,4,DEC);
      lerBytes(extent2Left,meuJef,4,DEC);
      lerBytes(extent2Bottom,meuJef,4,DEC);
      lerBytes(extent2Right,meuJef,4,DEC);
      lerBytes(extent2Top,meuJef,4,DEC);
      lerBytes(extent3Left,meuJef,4,DEC);
      lerBytes(extent3Bottom,meuJef,4,DEC);
      lerBytes(extent3Right,meuJef,4,DEC);
      lerBytes(extent3Top,meuJef,4,DEC);
      lerBytes(extent4Left,meuJef,4,DEC);
      lerBytes(extent4Bottom,meuJef,4,DEC);
      lerBytes(extent4Right,meuJef,4,DEC);
      lerBytes(extent4Top,meuJef,4,DEC);
      /*Leer ThreadColourList*/
      posicion = 116;
      Serial.println("It's ThreadColourListRick!");
      for (i=0;i<threadCount[0];i++)
      {
        lerBytes(threadColour,meuJef,4,HEX);
        posicion++;
      }
      /*Leer ThreadTypeList*/
      Serial.println("It's ThreadTypeListRick!");
      for (i=0;i<threadCount[0];i++)
      {
        lerBytes(threadType,meuJef,4,DEC);
        posicion++;
      }
      posicion=stitchOffset[0]+1;
      posicion=181;
      // close the file:
      meuJef.close();
    } else
    {
    	// if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
  }

  void loop()
  {
    /* Abro e cerro o archivo de cada vez. Supoño que en canto a rendemento non pode ser peor.
    */
    meuJef = SD.open("decomer.jef", FILE_READ);
    if(meuJef)
    {
      delay(1000);
      if (puntada[0]==1)
      {
        Serial.println("Novo fío");
      }
      else if (puntada[0]==2)
      {
        Serial.print("X -> ");
        Serial.print(puntada[1]);
        Serial.print(" | ");
        Serial.print("Y -> ");
        Serial.println(puntada[2]);
      }
      if (puntada[0] != 16)
      {
        Serial.println(puntada[0]);
        lerBytes(puntada,meuJef,posicion,posicion+3,DEC);
        posicion=posicion+3;
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

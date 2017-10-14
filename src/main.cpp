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

  struct headerStitch
  {
    char stitchOffset[4];
    char flags[4];
    char fecha[8];
    char hora[8];
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
  };
  struct puntada
  {
    byte escaped;
    byte codigo;
    byte desplX;
    byte desplY;
  };

  long jefColourLU[0x4e] =
  {
    0x000000, // "002","Black",  0x01, "
    0xF0F0F0, //", "001","White",0x02, "#
    0xFFFF17, //", "204","Yellow",0x03, "#
    0xFF6600, //", "203","Orange",0x04, "#
    0x2F5933, //", "219","Olive Green",0x05, "#
    0x237336, //", "226","Green",0x06, "#
    0x65C2C8, //", "217","Sky",0x07, "#
    0xAB5A96, //", "208","Purple",0x08, "#
    0xF669A0, //", "201","Pink",0x09, "#
    0xFF0000, //", "225","Red",0x0a, "#
    0x9C6445, //", "214","Brown",0x0b, "#
    0x0B2F84, //", "207","Blue",0c, "#

    0xE4C35D, //", "003","Gold",0d, "#
    0x481A05, //", "205","Dark Brown",0e, "#
    0xAC9CC7, //", "209","Pale Violet",0f, "#
    0xFDF5B5, //", "210","Pale Yellow",10, "#
    0xF999B7, //", "211","Pale Pink",11, "#
    0xFAB381, //", "212","Peach",12, "#
    0xD7BDA4, //", "213","Beige",13, "#
    0x970533, //", "215","Wine Red",14, "#
    0xA0B8CC, //", "216","Pale Sky",15, "#
    0x7FC21C, //", "218","Yellow Green",16, "#
    0xE5E5E5, //", "220","Silver Gray",17, "#
    0x889B9B, //", "221","Gray",18, "#

    0x98D6BD, //", "227","Pale Aqua",19, "#
    0xB2E1E3, //", "228","Baby Blue",1a, "#
    0x98F3FE, //", "229","Powder Blue",1b, "#
    0x70A9E2, //", "230","Bright Blue",1c, "#
    0x1D5478, //", "231","Slate Blue",1d, "#
    0x071650, //", "232","Navy Blue",1e, "#
    0xFFBBBB, //", "233","Salmon Pink",1f, "#
    0xFF6048, //", "234","Coral",20, "#
    0xFF5A27, //", "235","Burnt Orange",21, "#
    0xE2A188, //", "236","Cinnamon",22, "#
    0xB59474, //", "237","Umber",23, "#
    0xF5DB8B, //", "238","Blond",24, "#

    0xFFCC00, //", "239","Sunflower",25, "#
    0xFFBDE3, //", "240","Orchid Pink",26, "#
    0xC3007E, //", "241","Peony Purple",27, "#
    0xA80043, //", "242","Burgundy",28, "#
    0x540571, //", "243","Royal Purple",29, "#
    0xFF0927, //", "244","Cardinal Red",2a, "#
    0xC6EECB, //", "245","Opal Green",2b, "#
    0x608541, //", "246","Moss Green",2c, "#
    0x609418, //", "247","Meadow Green",2d, "#
    0x06480D, //", "248","Dark Green",2e, "#
    0x5BD2B5, //", "249","Aquamarine",2f, "#
    0x4CB58F, //", "250","Emerald Green",30, "#

    0x04917B, //", "251","Peacock Green",31, "#
    0x595B61, //", "252","Dark Gray",32, "#
    0xFFFFDC, //", "253","Ivory White",33, "#
    0xE6651E, //", "254","Hazel",34, "#
    0xE6965A, //", "255","Toast",35, "#
    0xF09C96, //", "256","Salmon",36, "#
    0xA76C3D, //", "257","Cocoa Brown",37, "#
    0xB45A30, //", "258","Sienna",38, "#
    0x6E3937, //", "259","Sepia",39, "#
    0x5C2625, //", "260","Dark Sepia",3a, "#
    0x6231BD, //", "261","Violet Blue",3b, "#
    0x14329C, //", "262","Blue Ink",3c, "#

    0x165FA7, //", "263","Solar Blue",3d, "#
    0xC4E39D, //", "264","Green Dust",3e, "#
    0xFD33A3, //", "265","Crimson",3f, "#
    0xEE71AF, //", "266","Floral Pink",40, "#
    0x843154, //", "267","Wine",41, "#
    0xA39166, //", "268","Olive Drab",42, "#
    0x0C8918, //", "269","Meadow",43, "#
    0xF7F297, //", "270","Mustard",44, "#
    0xCC9900, //", "271","Yellow Ocher",45, "#
    0xC79732, //", "272","Old Gold",46, "#
    0xFF9D00, //", "273","Honey Dew",47, "#
    0xFFBA5E, //", "274","Tangerine",48, "#

    0xFCF121, //", "275","Canary Yellow",49, "#
    0xFF4720, //", "202","Vermilion",4a, "#
    0x00B552, //", "206","Bright Green",4b, "#
    0x0257B5, //", "222","Ocean Blue",4c, "#
    0xD0BAB0, //", "223","Beige Gray",4d, "#
    0xE3BE81 //", "224","Bamboo",4e, "#
  };
  int posicion;

  headerStitch hS;
  puntada p = {0x80,1,0,0};
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


  void lerBytes(char byteLido[],File archivo, int inicio,int fin, int formato)
  {
    int i = 0;
    int j = 0;
    int lonx = fin;
    archivo.seek(inicio);

    for(j = 0; j < fin ;j++)
    {
      byteLido[j]=archivo.read();
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
  void lerBytes(byte byteLido,File archivo, int inicio, int formato)
  {
    int i = 0;
    archivo.seek(inicio);
    byteLido=archivo.read();

    if (formato == DEC || formato == HEX)
    {
      Serial.print(byteLido,formato);
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
      //delay(500);
      if (p.escaped==0x80)
      {
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

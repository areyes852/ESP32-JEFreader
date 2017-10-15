/* Función auxiliares
/
/                                                         a_reyes 2017 */
const int HORA=1;
const int FECHA=2;
const char DIV[] = "-----------------------------------";

void lerBytes(char* byteLido[],File archivo,int lonxitude);
void lerBytes(int* byteLido[],File archivo,int lonxitude);
void lerBytes(char* byteLido[],File archivo,int lonxitude, int formato);
void lerBytes(char* byteLido[],File archivo, int inicio,int fin, int formato);
void lerBytes(char* byteLido,File archivo, int inicio, int formato);

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
void lerBytes(char byteLido,File archivo, int inicio, int formato)
{
  int i = 0;
  archivo.seek(inicio);
  byteLido=archivo.read();

  // if (formato == DEC || formato == HEX)
  // {
  //   Serial.print(byteLido,formato);
  //   Serial.println();
  //   Serial.println(DIV);
  // }
  // else
  // {
  //   Serial.print(byteLido);
  //   Serial.println();
  //   Serial.println(DIV);
  // }
}

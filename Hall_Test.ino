#include <EEPROM.h>
#define hall 2
#define rst 10
#define spd 9
#define pos 8

volatile int long c = 0;
volatile int long cont = 0;
volatile int long cont2 = 0;
volatile int long cont3 = 0;
volatile int long cont4 = 0;

char direction = 'C';
char speed = 'H';

 void setup()
 {
   Serial.begin(9600);
   pinMode(hall,INPUT);
   pinMode(rst,INPUT_PULLUP);
   pinMode(8,INPUT_PULLUP);
   pinMode(9,INPUT_PULLUP);
   pinMode(13,OUTPUT);
   attachInterrupt(digitalPinToInterrupt(hall), magnet, CHANGE);
   
   if (digitalRead (rst) == LOW)
{
         EEPROM.write(509,0);
         EEPROM.write(510,0);
         EEPROM.write(511,0);  
         EEPROM.write(512,0);
}
  
 }

 void loop()
 {

   char tecla;

  
  if (digitalRead (spd) == HIGH)
  {
      speed = 'H';
  }
  if (digitalRead (spd) == LOW)
  {
      speed = 'F';
  } 

 

  tecla = Serial.read();
  //  if (tecla == '\r')
  //     {
  //     Serial.print(c);
  //     Serial.print("-");
      
  //     Serial.print(cont);
  //     Serial.print("-");
      
  //     Serial.println(cont2);
  //     }

  cont = c / 2 + cont4 ;
  delay(200);
  cont2 = c / 2 + cont4 ; 

   if (cont > 359)
    {
      c = 0;
      cont = 0;
      cont2 = 0;
      cont4 = 0;
         EEPROM.write(509,0);
         EEPROM.write(510,0);
         EEPROM.write(511,0);  
         EEPROM.write(512,0);
    }

   if (cont < 0)
    {
      c = 718;
      cont = 359;
      cont2 = 359;
      cont4 = 0;
         EEPROM.write(509,0);
         EEPROM.write(510,0);
         EEPROM.write(511,0);  
         EEPROM.write(512,0);
    }

      if (cont != cont2)
        {
          byte hiByte = highByte(cont2); // a EEPROM só salva valores de 0 a 255, então estamos quebrando o valor do contador em dois bytes.
          byte loByte = lowByte(cont2);
          EEPROM.write(511,hiByte);  // salvando os dois bytes na memória
          EEPROM.write(512,loByte);
        }

        byte highByte = EEPROM.read(511);  // lendo na memória
        byte lowByte = EEPROM.read(512);
        cont3  = word(highByte, lowByte);  // fazendo conversão dos dois bytes em apenas um para gerar o valor com até 5 casas decimais

      if (cont == 0 && cont2 == 0)   //"reset"

        {
         byte hiByte = highByte(cont3);
         byte loByte = lowByte(cont3);
         EEPROM.write(509,hiByte);
         EEPROM.write(510,loByte);
         byte highByte = EEPROM.read(509);
         byte lowByte = EEPROM.read(510);
         cont4  = word(highByte, lowByte);
        }

      if (tecla == '\r')
      {
      Serial.print(direction);
      Serial.print(speed);
      Serial.print(cont);
      }

 }

 void magnet()
  {
  
    if (digitalRead (pos) == HIGH)
    {
    c++ ;
    direction = 'C';
    }
    if (digitalRead (pos) == LOW)
    {
    c-- ;
    direction = 'W';
    }

    digitalWrite(13,HIGH);
    delay(20);
    digitalWrite(13,LOW);
  }
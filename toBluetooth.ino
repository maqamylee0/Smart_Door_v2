#include <Keypad.h>
#include <EEPROM.h>
#include <Servo.h>


#define Password_Lenght 5 
#define buzzer 12
#define sensor 13
#define light 10
#define greenGo 11
Servo myservo;

int count = 0;
int pirPin ;
char Data1[Password_Lenght];
char Data2[Password_Lenght];
char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght];
byte data_count = 0, master_count = 0;

char customKey;
char key;
char FistTimePassword[] ="1234";
String text;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = false;

byte rowPins[ROWS] = {5, 4, 3, 2 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6 }; //connect to the column pinouts of the keypad

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad

void setup()
{
 Serial.begin(9600);

  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  myservo.attach(9);
   myservo.write(0);
   pinMode(buzzer, OUTPUT); 
   pinMode(sensor, INPUT);
   pinMode(light,OUTPUT);
//   pinMode(openDoor,OUTPUT);
   pinMode(greenGo,OUTPUT);
  Check_EEPROM();   


}

void loop()
{
  
 //checkSensor();
 //checkDoor();
  checkPhone();

}
void checkSensor(){
  pirPin = digitalRead(sensor);
  if(pirPin == HIGH){
    alert();
  }
}

void checkPhone(){
  while(Serial.available()){
char c= Serial.read();
text += c;
}
if(text == Master){
  if(door == false){
   myservo.write(90);
      digitalWrite(light,HIGH);
      delay(100);
      digitalWrite(light,LOW); 
      text="";
      Serial.print("OPEN");
      //Serial.print("*");
//       Serial.print(" ");

      door = true;
      //delay(2000);
  }else{
    myservo.write(0); 
    door =false;
    Serial.print("CLOSED");
    //delay(2000);
     text="";
     
  }
      
    }
  }

 void checkDoor(){ 
  if (door == true)
  {
    digitalWrite(greenGo,HIGH);

    customKey = customKeypad.getKey();
  
    if (customKey == '#')
    {   
    digitalWrite(greenGo,LOW);  
      door = false;
      myservo.write(0);
    }
  }

  else Open();
//   digitalWrite(greenGo,LOW);
}

void clearData()
{
  while (data_count != 0)
  { 
    Data[data_count--] = 0; 
  }
  return;
}

void alert(){     
      digitalWrite(buzzer, HIGH);
     delay(3000);//wait for 2ms
      digitalWrite(buzzer, LOW);
      delay(2);//wait for 2ms 
}

void Open()
{
 if(customKey == '*'){
       digitalWrite(greenGo,LOW);
       resetPassword();
      delay(100);
    }
 digitalWrite(greenGo,HIGH);
    // delay(200);

  customKey = customKeypad.getKey();
  if(customKey)   {
    Data[data_count] = customKey;
    data_count++;   
  }
//  digitalWrite(greenGo,LOW);
//   delay(1000);
  if (data_count == Password_Lenght - 1) 
  {
    if (!strcmp(Data, Master))
    {     
      door = true;
      myservo.write(90);
      digitalWrite(greenGo,LOW);
//       delay(1000);
//     digitalWrite(light,HIGH);

    }
    else
    {
      digitalWrite(greenGo,LOW);
      digitalWrite(light,HIGH);
      count++;
      //delay(1000);
      if(count ==3){
        alert();
      }
      door = false;
      myservo.write(0);
      digitalWrite(light,LOW);
    }
    clearData();
  }
}
void resetPassword(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

 digitalWrite(greenGo,HIGH);
   key = customKeypad.getKey();      
    for(int i = 0; i < Password_Lenght; i = i+1){
      Data1[i] = key;
     // data_count++;
      }
digitalWrite(greenGo,LOW);
delay(1000);
digitalWrite(greenGo,HIGH);
   key = customKeypad.getKey();   

   for(int i = 0; i < Password_Lenght; i = i+1){
    Data2[i] = key;
    //  data_count++;
    }
  
    if(strcmp(Data1, Data2)){
   digitalWrite(greenGo,LOW);
   myservo.write(90);
   door = true;
        for(int i=0;i<Password_Lenght;i++){
     EEPROM.write(i, Data[i]);
    }

       for(int i=0;i<Password_Lenght;i++){
          Master[i] = EEPROM.read(i);
         

    }
        delay(500);  
     }else{
       // alert();
       door = false;
    digitalWrite(light,HIGH);
    delay(1000);
    digitalWrite(light,LOW);
      }
    }
  
void Check_EEPROM(){ 
     for(int i=0;i<Password_Lenght;i++){
          Master[i] = EEPROM.read(i);          
    }

    if(Master[0] == 0 && Master[1] == 0 && Master[2] == 0 && Master[3] == 0){ 
    delay(200);
//    EEPROM.write(0,Password_Lenght);
    for(int i=0;i<Password_Lenght;i++){
     EEPROM.write(i, FistTimePassword[i]);
    }
    for(int i=0;i<Password_Lenght;i++){
          Master[i] = EEPROM.read(i);
         

    }


  }
}

#include "Servo.h"

Servo servoCurtains; // Объект Servo
Servo servoCurtains2; // Объект Servo
int Servo1;
int sensePin = 0;
int val1=0;

    void setup(){
      Serial.begin(9600);
      pinMode(sensePin, INPUT);
      servoCurtains.attach(3);
      servoCurtains2.attach(5);
       }

    void loop(){
      delay(1);     
      if (Serial.available()> 0){
      int com = Serial.read();
      
         if (com == 'a'){       
           servoCurtains.write(0);
         }
        else if (com == 'b'){
           servoCurtains.write(90);

         }
        else if (com == 'c'){
          servoCurtains2.write(0);
        }
        else if (com == 'd'){
        servoCurtains2.write(90);
        
        }
        else if (com == 'e')
       {
        while (1)
        {

          int val = analogRead(sensePin);// Создаем переменную val для чтения analogRead
          val= constrain(val,230 ,699);// Ограничиваем диапазон работы датчика посмотрев его на мониторе
          int motorAngl = map(val,699, 230, 0,90);//Преобразуем диапазон выдаваемый датчиком в диапазон углов сервы
          servoCurtains.write(motorAngl);
          servoCurtains2.write(motorAngl);
          delay(1);
          com = Serial.read();
          if (com == 'f')
          break;
          
        }
      }
    }
  }

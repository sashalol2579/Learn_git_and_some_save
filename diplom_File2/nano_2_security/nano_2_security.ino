#include <Servo.h> 
#include <SPI.h>
#include <MFRC522.h> // это скачанная библиотека "RFID".

/*
подключение для Arduino Uno и Mega, производится к разным Pin!
 ----------------------------------------------------- Nicola Coppola
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               51                MOSI
 * SPI MISO   12               50                MISO
 * SPI SCK    13               52                SCK
 
 */

// два Pin (SS и RST) допускают произвольное подключение и конфигурируются в коде.
// !(SS - он же - SDA).
        #define SS_PIN 10
        #define RST_PIN 9
        MFRC522 mfrc522(SS_PIN, RST_PIN);  // объект MFRC522C / reate MFRC522 instance.
        unsigned long uidDec, uidDecTemp;  // для отображения номера карточки в десятичном формате      
        Servo servoMain; // Объект Servo
        int Servo1;
        int sound = 3;
        const int Trig = 4;   
        const int Echo = 5; 
        const int ledPin = 13;  // 13 – если будете использовать встроенный в Arduino светодиод
        int Door_Sensor_Pin = 6;

      void setup(){
        Serial.begin(9600);
        SPI.begin();  //  инициализация SPI / Init SPI bus.
        mfrc522.PCD_Init();     // инициализация MFRC522 / Init MFRC522 card.
        servoMain.attach(7);
        pinMode(Trig, OUTPUT); 
        pinMode(Echo, INPUT); 
        pinMode(ledPin, OUTPUT); 
        pinMode(sound, OUTPUT); //объявляем пин как выход
        pinMode(Door_Sensor_Pin, INPUT);  
      }

        unsigned int time_us=0;
        unsigned int distance_sm=0;
      
      void loop(){ 
        delay(1);
        noTone(sound);
// Проверяем, есть ли что-нибудь в буфере программного последовательного порта
        if ( Serial.available()> 0){
// Читаем один символ из буфера программного последовательного порта и сохраняем его переменную com
        int com = Serial.read();
// Действуем соответственно полученному символу
        if (com == 'g'){
            servoMain.write(0);
             while (1){
            com = Serial.read();
            if (com == 'h')
            break;
            rfid();
            }
           }
  
        else if (com == 'i'){
          servoMain.write(0);
          while (1){
          com = Serial.read();
          if (com == 'j'){
          noTone(sound);
          break;
          }
          Alarms();
          }
         }  
        }
       }
//====================================



          void rfid(){
             if ( ! mfrc522.PICC_IsNewCardPresent()) {
                return;
             }
  // Выбор карточки / Select one of the cards.
             if ( ! mfrc522.PICC_ReadCardSerial()) {
                return;
              }
           uidDec = 0;
  // Выдача серийного номера карточки "UID".
              for (byte i = 0; i < mfrc522.uid.size; i++) 
              {
                uidDecTemp = mfrc522.uid.uidByte[i];
                uidDec = uidDec*256+uidDecTemp;  
              }  
//Serial.println(uidDec); 
  // ------------------------
  // начинаем сравнивать номер "UID", поднесённой к ридеру карты, 
  // с записанным номером "UID" карты в sketch.
  // замените!!! "UID" № "422455350", на номер "UID" № вашей карты. Біла карточка
                  if (uidDec == 632889388) // если "UID" номер совпал.
                  {
                    // поворачиваем servo на 170 градусов.
                   servoMain.write(170);  
                  }
                  // также и с другими номерами карт. для синього жетона
                  else if (uidDec == 4105804028)
                  {
                   servoMain.write(170);    
                  }
                  else  // если "UID" номер карты не совпал.
                  {
                    // включим номер карты не совпал.
                  }
                  delay(3000); // пауза 3 секунды.
                  servoMain.write(10); // возвращаем servo обратно.
                  // и выключим все светодиоды.
                  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
                  
                  }


              void Alarms(){
            
                       int val1;
                    digitalWrite(Trig, HIGH); // Подаем сигнал на выход микроконтроллера 
                    delayMicroseconds(10); // Удерживаем 10 микросекунд 
                    digitalWrite(Trig, LOW); // Затем убираем 
                    time_us=pulseIn(Echo, HIGH); // Замеряем длину импульса 
                    
                    
                    distance_sm=time_us/58; // Пересчитываем в сантиметры 
                    
                    val1 = digitalRead(Door_Sensor_Pin);
                    
                    if (distance_sm<15 || val1==1) // Если расстояние менее 50 сантиметром 
                    { 
                         tone (sound, 500); //включаем на 500 Гц
                    
                    delay(100); //ждем 100 Мс
                    
                    tone(sound, 1000); //включаем на 1000 Гц
                    
                    delay(100); //ждем 100 Мс
                    
                    } 
            
              }









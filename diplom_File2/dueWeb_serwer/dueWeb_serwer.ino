
//zoomkat 8-04-12
//simple button GET server code to control servo and arduino pins 5, 6, and 7
//for use with IDE 1.0
//open serial monitor to see what the arduino receives
//use ' instead of " in the html
//address will look like http://192.168.1.102:84 when submited
//for use with W5100 based ethernet shields
///note that the below bug fix may be required
// http://code.google.com/p/arduino/issues/detail?id=605

#include <SPI.h>
#include <UIPEthernet.h> //change if not using Shield 2
#include <OneWire.h>
#include <SimpleDHT.h>
int pinDHT11 = 22;
SimpleDHT11 dht11;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
static byte ip[] = { 
  192,168,1,222 };
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
boolean PinStatus[7];
OneWire ds(28);

EthernetServer server(80); //server port

String readString;

//////////////////////

void setup(){
  pinMode(23, OUTPUT); //pin selected to control
  pinMode(24, OUTPUT); //pin selected to control
  pinMode(25, OUTPUT); //pin selected to control
  pinMode(26, OUTPUT); //pin selected to control
  pinMode(27, INPUT); //pin selected to control
  pinMode(29, INPUT);
  pinMode(30, INPUT);
  
  for(int i = 0; i <= 7; i++)
  {
    PinStatus[i] = false;
  }
   Serial1.begin(9600);
   Serial2.begin(9600);
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, gateway, subnet);
  server.begin();

  //enable serial data print
}

void loop(){
  boolean RainSensor;
  boolean FireSensor;
  boolean GasSensor;

    if (digitalRead(27)==HIGH){
          RainSensor = true;
    }
    else if (digitalRead(27)==LOW)
           RainSensor = false;

    if (digitalRead(29)==HIGH){
           FireSensor = true;
    }
    else if (digitalRead(29)==LOW)
           FireSensor = false;

    if (digitalRead(30)==HIGH){
           GasSensor = true;
    }
    else if (digitalRead(30)==LOW)
            GasSensor = false;
     

    
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    
    return;
  }

  byte data[2];
  ds.reset(); 
  ds.write(0xCC);
  ds.write(0x44); 
  delay(1);
  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);
  data[0] = ds.read(); 
  data[1] = ds.read();
  int Temp = (data[1]<< 8)+data[0];
  Temp = Temp>>4;
  
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string
          readString += c;
          //Serial.print(c);
        }

        //if HTTP request has ended
        if (c == '\n') {
       
          ///////////////
         
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // Close the connection once the page is fully loaded
          client.println("Refresh: 3");  // Refresh the page automatically every 2 seconds
          client.println(); // End of HTTP Response header

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Smart Home</TITLE>");
          client.println("</HEAD>");
          client. println ("<meta http-equiv='Content-Type' content='text/html ; charset=utf-8'/> "); 
          client.println("<BODY>");

          client.println("<H1>Диплом Полевщиков</H1>");
          client.println("<br>");
       
           client.println("<H1>Показники давачів</H1>");

          client.println("<u>Температува: </u>"); // Set header
          client.println((int)temperature);
          client.println("<br>");
          client.println("<u>Волога: </u>"); // Set header
          client.println((int)humidity);
          client.println("<br>");
          client.println("<u>Температува надворі: </u>"); // Set header
          client.println((int)Temp);
          client.println("<br>");
          client.println("<u>Дощовий сесор: </u>"); // Set header
          if (RainSensor == false)
          client.println("Йде дощ");
          else if (RainSensor == true)
          client.println("Дощу нема");
          client.println("<br>");



          client.println("<u>Газовий сесор: </u>"); // Set header
          if (GasSensor == false)
          client.println("ВИСОКИЙ РІВЕНЬ ГАЗУ");
          else if (GasSensor == true)
          client.println("нормальний рівень газу");
          client.println("<br>");



          client.println("<u>Вогневий сесор: </u>"); // Set header
          if (FireSensor == false)
          client.println("Вогню нема");
          else if (FireSensor == true)
          client.println("ЗАФІКСОВАНО ПОЛУМ'Я");
          client.println("<br>");

          client.println("<br>"); 
          client.println("<br>");  
               
         client.println("<H1>Освітлення</H1>");
         client.println("<br>");
         
           if (PinStatus[0]==false){
          client.print("&nbsp;<input type=submit value=ON style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?2;'>");
          
          }
          else if (PinStatus[0]==true){
          client.print("&nbsp;<input type=submit value=OFF style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?3;'>");
          
          }
            if (PinStatus[1]==false){
          client.print("&nbsp;&nbsp;&nbsp;<input type=submit value=ON style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?4;'><br><br>");
          
          }
          else if (PinStatus[1]==true){
          client.print("&nbsp;&nbsp;&nbsp;<input type=submit value=OFF style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?5;'><br><br>");
          
          }            
           if (PinStatus[2]==false){
          client.print("&nbsp;<input type=submit value=ON style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?6;'>");
          
          }
          else if (PinStatus[2]==true){
          client.print("&nbsp;&nbsp;&nbsp;<input type=submit value=OFF style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?7;'>");
          
          }
           if (PinStatus[3]==false){
          client.print("&nbsp;&nbsp;&nbsp;<input type=submit value=ON style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?8;'><br><br>");
          
          }
          else if (PinStatus[3]==true){
          client.print("&nbsp;&nbsp;&nbsp;<input type=submit value=OFF style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?9;'><br><br>");
          
          }
          
          client.print("&nbsp;<input type=submit value='ALL OFF' style=width:607px;height:135px;font-size:50px onClick=location.href='/?3579;'>");
          
          client.println("<br>");
          client.println("<br>");
          client.println("<H1>Штори</H1>");
          client.println("<br>");
           
          if (PinStatus[4]==false){
          client.print("&nbsp;<input type=submit value=ON style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?a;'>");
          
          }
          else if (PinStatus[4]==true){
          client.print("&nbsp;<input type=submit value=OFF style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?b;'>");
          
          }
            if (PinStatus[5]==false){
          client.print("&nbsp;&nbsp;&nbsp;<input type=submit value=ON style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?c;'><br><br>");
          
          }
          else if (PinStatus[5]==true){
          client.print("&nbsp;&nbsp;&nbsp;<input type=submit value=OFF style=width:300px;height:135px;border-radius:50px;font-size:50px onClick=location.href='/?d;'><br><br>");
          
          }            
            
          client.print("&nbsp;<input type=submit value='Автоматичні штори' style=width:607px;height:135px;font-size:50px onClick=location.href='/?e;'>");
          client.println("<H1>Систеама безпеки</H1>");
          if (PinStatus[6]==false){
          client.print("&nbsp;<input type=submit value='Вкл RFID' style=width:607px;height:135px;font-size:50px onClick=location.href='/?g;'>");
          
          }
          else if (PinStatus[6]==true){
          client.print("&nbsp;<input type=submit value='Викл RFID' style=width:607px;height:135px;font-size:50px onClick=location.href='/?h;'>");
          
          }
          client.println("<br>");
          client.println("<br>");
            if (PinStatus[7]==false){
          client.print("&nbsp;<input type=submit value='Вкл сигналізацію' style=width:607px;height:135px;font-size:50px onClick=location.href='/?i;'>");
          
          }
          else if (PinStatus[7]==true){
          client.print("&nbsp;<input type=submit value='Викл сигналізацію' style=width:607px;height:135px;font-size:50px onClick=location.href='/?j;'>");
          
          }            
          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          //stopping client
          client.stop();

          ///////////////////// control arduino pin
          if(readString.indexOf('2') >0)//checks for 2
          {
            digitalWrite(23, HIGH);    // set pin 5 high
            PinStatus[0]=true;
          }
          if(readString.indexOf('3') >0)//checks for 3
          {
            digitalWrite(23, LOW);    // set pin 5 low
            PinStatus[0]=false;
          }
         
          if(readString.indexOf('4') >0)//checks for 4
          {
            digitalWrite(24, HIGH);    // set pin 6 high
            PinStatus[1]=true;
          }
          if(readString.indexOf('5') >0)//checks for 5
          {
            digitalWrite(24, LOW);    // set pin 6 low
            PinStatus[1]=false;
          }
         
           if(readString.indexOf('6') >0)//checks for 6
          {
            digitalWrite(25, HIGH);    // set pin 7 high
            PinStatus[2]=true;
          }
          if(readString.indexOf('7') >0)//checks for 7
          {
            digitalWrite(25, LOW);    // set pin 7 low
            PinStatus[2]=false;
          }     
         
            if(readString.indexOf('8') >0)//checks for 8
          {
            digitalWrite(26, HIGH);    // set pin 8 high
            PinStatus[3]=true;
          }
          if(readString.indexOf('9') >0)//checks for 9
          
          {
            digitalWrite(26, LOW);    // set pin 8 low
            PinStatus[3]=false;
          }   



             if(readString.indexOf('a') >0)//checks for 8
          {
            Serial1.write('a');
            Serial1.write('f');
            PinStatus[4]=true;
          }
          if(readString.indexOf('b') >0)//checks for 9
          
          {
            Serial1.write('b');
            Serial1.write('f');
            PinStatus[4]=false;
          }     
          
            if(readString.indexOf('c') >0)//checks for 8
          {
           Serial1.write('c');
           Serial1.write('f');
            PinStatus[5]=true;
          }
          if(readString.indexOf('d') >0)//checks for 9
          
          {
           Serial1.write('d');
           Serial1.write('f');
            PinStatus[5]=false;
          }    
               
         if(readString.indexOf('e') >0)//checks for 8
          {
           Serial1.write('e');
          } 


            if(readString.indexOf('g') >0)//checks for 8
          {
           Serial2.write('g');
            PinStatus[6]=true;
          }
           if(readString.indexOf('h') >0)//checks for 9
          
          {
            Serial2.write('h');            
            PinStatus[6]=false;
          }     
          
            if(readString.indexOf('i') >0)//checks for 8
          {
            Serial2.write('i');     
            PinStatus[7]=true;
          }
          if(readString.indexOf('j') >0)//checks for 9
          
          {
           Serial2.write('j');         
            PinStatus[7]=false;
          }    
    
    //clearing string for next read
          readString="";

        }
      }
    }
  }
}



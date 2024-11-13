/*
 * HC-06  >>  Arduino
 * 
 * RX     >>  11
 * TX     >>  3
 * GND    >>  GND
 * VCC    >>  5V
 * 
 * 
 * Commands:
 * 
AT : Check the connection.
AT+NAME : See default name
AT+ADDR : see default address
AT+VERSION : See version
AT+UART : See baudrate
AT+ROLE: See role of bt module(1=master/0=slave)
AT+RESET : Reset and exit AT mode
AT+ORGL : Restore factory settings
AT+PSWD: see default password
AT+PIN: set pin code

1 set to 1200bps
2 set to 2400bps 
3 set to 4800bps 
4 set to 9600bps (Default) 
5 set to 19200bps 
6 set to 38400bps 
7 set to 57600bps 
8 set to 115200bps
 */

#include <SoftwareSerial.h>
#include <Servo.h> // Servo.h library can't use pins 9 and 10 on Arduino

SoftwareSerial BT(3,11); //9,10

int led = 6;
int servo_pin = 2;

int servo_pos = 90;

Servo myservo;

void setup()  
{
  
  pinMode(led, OUTPUT);
  myservo.attach(servo_pin);
  
  digitalWrite(led, HIGH);
  myservo.write(90);
  
  Serial.begin(9600);
  BT.begin(9600);
  Serial.println("Hello from Arduino");
}

//format needed to read is "<h>" command or servo "<t125>" 3 digit number
bool processingData = false;
void loop(){ 
  if (Serial.available()){
    char ch = (char)Serial.read();
    processCommand(ch);
  }

  if (BT.available()){
    char ch = (char)BT.read();
    if(processingData == false){
      processingData = true;
      processCommand(ch);
      processingData = false;
    }
  }
}

char rdx[8];
int index = 0;
char startRead = '<';
char endRead = '>';
bool isReadingData = false;


void processCommand(char a){
  if(a == '<'){
      isReadingData = true;
      rdx[index] = a;
      index++;
    } else if(a == '>' && index > 0){
      isReadingData = false;
      rdx[index] = a;
      index = 0;
      sendCommand();
    } else if(isReadingData == true){
      rdx[index] = a;
      index++;
    }
}

void sendCommand(){
          switch(rdx[1]){
            case 'h':
              Serial.println("LED On");
              BT.print("LED On#");             
              digitalWrite(led, HIGH);
              break;
            case 'o':
              Serial.println("LED Off");
              BT.print("LED Off#");             
              digitalWrite(led, LOW);
              break;
            case 'l':
              Serial.println("Servo Left");
              BT.print("Servo Left#");
              myservo.write(30);
              //delay(500);
              break; 
            case 'r':
              Serial.println("Servo Right");
              BT.print("Servo Right#");
              myservo.write(150);
              //delay(500);
              break;
            case 'm':
              Serial.println("Servo Center");
              BT.print("Servo Center#");
              myservo.write(90);
              //delay(500);
              break;
            case 't':
              Serial.println("Servo Move");
              BT.print("Servo Move ");
              char move[3];
              move[0] = rdx[2];
              move[1] = rdx[3];
              move[2] = rdx[4];
              if(isDigit(move[0]) && isDigit(move[1]) && isDigit(move[2])){
                myservo.write(atoi(move));
                Serial.println(move);
                BT.print(move);
                BT.print("#");
              }
              break;        
   }
}
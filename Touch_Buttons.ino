// Setup for Uno and TouchScreen Sheild ELEGOO UNO R3 2.8" 
//TFT with small white CD bought on Amazon in Nov 6,2021
//Not sure who I got this from, so I assume it's copyrighted
//Use only for personal use until I can find who to give proper credit to.
//I'm using an UNO, an ELEGOO UNO R3 2.8 Inches TFT Touch Screen
//This will display a few on off buttons which can be enabled to turn on relays or led's or....?

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

unsigned long previousMillis = 0;
const long interval = 1000;   

const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=912,TS_RT=116,TS_TOP=85,TS_BOT=903;

  int pixel_x, pixel_y;     //Touch_getXY() updates global vars
  int texty[] = {15, 65, 115, 165, 215};
  int buttony[] = {20, 70, 120, 170, 220};
  int rect[] = {10,60,110,160,210};
  char* labels[]={"START","STOP","WATER","OZONE","RTD"};
      bool firstrun = 1;
    
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button on_btn, off_btn;
//Adafruit_INA219 sensor219; // Declare and instance of INA219

bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());   // Determine where touch is X axis
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());  // Determine where touch is Y axis
    }
    else {pixel_x=0; pixel_y=0;
}
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
 
///////////////////////////  CHECK FOR TOUCH and ACT
  int TouchCheck(bool pressed, int line)
{
    bool down = Touch_getXY();

    tft.setCursor(45, texty[line]);
    tft.setTextColor(YELLOW); tft.setTextSize(2);
    tft.println(labels[line]);

  //   X, Y, W, H, OUTLINE, FILL, TEXTCOLOR, LABEL_TXT, TEXT_SIZE    
     on_btn.initButton(&tft, 140, buttony[line], 50, 40, WHITE, CYAN, BLACK, "ON", 2);   //Position of buttons
    off_btn.initButton(&tft, 200, buttony[line], 50, 40, WHITE, CYAN, BLACK, "OFF", 2);   //Position of buttons

    if(firstrun)  { tft.fillRect(10, rect[line], 30, 30, RED);
    on_btn.drawButton(false);
   off_btn.drawButton(false); } 

    on_btn.press(down && on_btn.contains(pixel_x, pixel_y));
   off_btn.press(down && off_btn.contains(pixel_x, pixel_y));
    
    Serial.print(pixel_x);
        Serial.print(",");
    Serial.println(pixel_y);  
      
    if (on_btn.justReleased())
        on_btn.drawButton();
        
    if (off_btn.justReleased())
        off_btn.drawButton();
        
    if (on_btn.justPressed()) {
        on_btn.drawButton(false);
        tft.fillRect(10, rect[line], 30, 30, GREEN);
        digitalWrite(31+(line*2),LOW);
    }
    if (off_btn.justPressed()) {
        off_btn.drawButton(false);
        tft.fillRect(10, rect[line], 30, 30, RED);
        digitalWrite(31+(line*2),HIGH);
    }    
    }


void setup(void)
{
    Serial.begin(9600);

//    sensor219.begin();  // INA219
    
      pinMode(31, OUTPUT); //  Head Light
      pinMode(33, OUTPUT); //  Tail Light
      pinMode(35, OUTPUT); //  Cops Light
      pinMode(37, OUTPUT); //  12V Bus
      pinMode(39, OUTPUT); //  5V USB Power  all but Arduino
      digitalWrite(31,HIGH);
      digitalWrite(33,HIGH);
      digitalWrite(35,HIGH);
      digitalWrite(37,HIGH);
      digitalWrite(39,HIGH);   
                     
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    tft.fillScreen(BLACK);
} // =========== SETUP ==============



void loop(void)
{
  unsigned long currentMillis = millis();
  int line;  bool pressed;
  for (line = 0; line < 5; line++)  TouchCheck(pressed,line);
  firstrun = 0;
     
  float busVoltage = 0;
  float current = 0; // Measure in milli amps
  float power = 0;

//  busVoltage = sensor219.getBusVoltage_V();
//  current = sensor219.getCurrent_mA();
//  power = busVoltage * (current/1000); // Calculate the Power
  
  tft.setCursor(10, 262);
  tft.setTextColor(GREEN); tft.setTextSize(3);
  tft.print("BRIAN MATHIS");
//  tft.print("V ");
//  tft.print(current,1);
//  tft.println("A");
//  tft.setCursor(60, 365);
//  tft.setTextColor(WHITE); tft.setTextSize(3);
//  power = busVoltage * current;
//  tft.print(power,2);
//  tft.println("W");
  
  /*
  Serial.print("Bus Voltage:   "); 
  Serial.print(busVoltage); 
  Serial.println(" V");  
  
  Serial.print("Current:       "); 
  Serial.print(current); 
  Serial.println(" mA");
  
  Serial.print("Power:         "); 
  Serial.print(power); 
  Serial.println(" W");  
  
  Serial.println("");  
*/ 

  if (currentMillis - previousMillis >= 2000) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
 tft.fillRect(0, 290, 400, 30, BLACK);
  tft.fillRect(0, 290, busVoltage * 6, 30, BLUE);
   tft.fillRect(0, 325, 400, 30, BLACK);
  tft.fillRect(0, 325, power * -6, 30, WHITE);
  }
  } // =========== LOOP ==============
  // TFT Touch Screen (Amazon)
  //  https://www.amazon.com/gp/product/B07D2FR5MJ/ref=ox_sc_act_title_1?smid=A1EJT7E0QSYEZM&psc=1
  //
  //  DB-25      Panel to Guts Box:
  //  Pins
  //
  //1, 2, 3       GND
  //4, 5, 6       Vcc (+5V from USB adapter)
  //7  HEAD       RELAY 1     BRN  I/O 31
  //8  TAIL       RELAY 2     ORG  I/O 33
  //9  COPS       RELAY 3     YEL  I/O 35
  //10 +12V       RELAY 4     GRN  I/O 37
  //11 +5V        RELAY 5     BLU  I/O 39

#include <Time.h>
#include <TimeLib.h>
#include <Adafruit_RGBLCDShield.h>
#include <EEPROM.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7 

int Minutes = 0;//initiating the variables for the alarm time
int Hours = 0;
int Seconds = 0;
int State = 0; //initialising state to make it so that i can use the same button more than once


void setup() {
  
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  // Print a message to the LCD.
  lcd.print("SELECT FOR");

  lcd.setCursor(0, 1);//Start up screen, giving instruction
  
  lcd.print("ALARM SETTINGS");
  delay(3000); //delay for 3000, so user can read

  setTime(00, 00, 00, 00, 00, 00);

  time_t t = now();
}

void loop() {
  
  if (State == 0){
    
      clockgo(); //getting the class clockgo to run through the code

         uint8_t buttons = lcd.readButtons();  //initiating the buttons  

      if (buttons & BUTTON_SELECT) {

        State =1; //once the button SELECT is pressed, the state becomes 1 allowing a seperate lcd screen be visible
        lcd.clear();
        lcd.print("Press DOWN to");
        lcd.setCursor(0,1);
        lcd.print("To go back.");
        delay(2000);
        lcd.clear();
        }
        while(State ==1){ //while the state is 1, print out the following:
                
        lcd.setCursor(0,0);
        lcd.print("Alarm:");
        lcd.setCursor(8,0);
        lcd.print("U Sec:");
        lcd.setCursor(0,1);
        lcd.print("L Hr: ");
        lcd.setCursor(8,1);
        lcd.print("R Min:");
        lcd.setCursor(14,1);
        lcd.print(Minutes);
        lcd.setCursor(14,0);
        lcd.print(Seconds);
        lcd.setCursor(5,1);
        lcd.print(Hours);
        lcd.setCursor(0,0);
        
        
  uint8_t buttons = lcd.readButtons(); //initiating buttons again for the while loop for state = 1
  if (buttons) {
    if (buttons & BUTTON_UP) {

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Alarm:");
        lcd.setCursor(8,0);
        lcd.print("U Sec:");
        lcd.setCursor(0,1);
        lcd.print("L Hr: ");
        lcd.setCursor(8,1);
        lcd.print("R Min:");
        lcd.setCursor(14,1);
        lcd.print(Minutes);
        lcd.setCursor(14,0);
        lcd.print(Seconds);
        lcd.setCursor(5,1);
        lcd.print(Hours);

        Seconds++; //if the button UP is pressed, it increases the the value by 1
        lcd.setCursor(14,0);
        lcd.print(Seconds);
        if(Seconds>59){ //Once it reaches 59, it resets the value back to 0
          lcd.clear();
          lcd.print("Secs Cannot Go");
          lcd.setCursor(0,1);
          lcd.print("Beyond 59!"); //gives a warning as well
        }
    }

    if (buttons & BUTTON_LEFT) { //for when LEFT is pressed
      
       lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Alarm:");
        lcd.setCursor(8,0);
        lcd.print("U Sec:");
        lcd.setCursor(0,1);
        lcd.print("L Hr: ");
        lcd.setCursor(8,1);
        lcd.print("R Min:");
        lcd.setCursor(14,1);
        lcd.print(Minutes);
        lcd.setCursor(14,0);
        lcd.print(Seconds);
        lcd.setCursor(5,1);
        lcd.print(Hours);

        Hours++; //Increase the hour value by 1 
        lcd.setCursor(5,1);
        lcd.print(Hours);
        if(Hours>23){
          Hours=0; //if the value of hours is >23, it will reset it back to 0 and give a message
          lcd.clear();
          lcd.print("Hours Cannot Go");
          lcd.setCursor(0,1);
          lcd.print("Beyond 23!");
        }delay(100);
            
    }
    if (buttons & BUTTON_RIGHT) { //For button RIGHT
      
      lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Alarm:");
        lcd.setCursor(8,0);
        lcd.print("U Sec:");
        lcd.setCursor(0,1);
        lcd.print("L Hr: ");
        lcd.setCursor(8,1);
        lcd.print("R Min:");
        lcd.setCursor(14,1);
        lcd.print(Minutes);
        lcd.setCursor(14,0);
        lcd.print(Seconds); //display the values
        lcd.setCursor(5,1);
        lcd.print(Hours);

        Minutes++;
        lcd.setCursor(14,1);
        lcd.print(Minutes);
        if(Minutes>59){ // if the number of minutes is > 59, reset it to 0
          Minutes=0;
          lcd.clear();
          lcd.print("Mins Cannot Go");
          lcd.setCursor(0,1);
          lcd.print("Beyond 59!");
        }delay(100);
       
    }
   
        if (buttons & BUTTON_DOWN) {
      lcd.setBacklight(WHITE);

      State=0; //to return back to the main screen, make DOWN button = the back button. Returns back to main screen, as state becomes 0
      }
  }

}
  }

  if(Hours == hour()& Minutes == minute() & Seconds == second()){ //if the value Hours, Minutes and Seconds is identical to the time
        State = 2; //State will become 2
        lcd.setBacklight(RED); 
        lcd.clear();
        lcd.print("ALERT! DOWN=OFF "); //this will then produce another screen on the lcd and give instructions to the user
        lcd.setCursor(0,1);
        lcd.print("L=SNOOZE");
        

        while(State ==2){
         uint8_t buttons = lcd.readButtons();
        if (buttons & BUTTON_DOWN) {
      lcd.setBacklight(WHITE); // while the state is = 2. make it so that the DOWN button turns off the alarm, and sets it back to main screen with clock
      
      
      State=0;
      clockgo(); // going back to clockgo class to print out clock values again
        }

        if(buttons & BUTTON_LEFT){
          lcd.clear();
          lcd.setBacklight(TEAL); //if LEFT is pressed, it also goes onto state 2
          lcd.print("snoozed");
          delay(3000);
          Seconds = Seconds + 20; //but instead of turning alarm off completely, the value of the alarm will be increased by 20 secon ds
          if (Seconds > 59){
            Seconds = 15; // if the value of seconds is greater than 59
            Minutes = Minutes + 1; //the minutes will be increased by 1 and the seconds will become 15, making it so
            //when the alarm snoozes beyond 59, it resets to 0 and increases minutes by 1
          }
          if (Minutes >59){
            Seconds = 15; //The same goes for minutes, if the minutes is 59, then the hour will be increased by 1,
            Minutes = 0; //minutes set to 0
            Hours = Hours + 1; // and the number of seconds increased to 15 just like before
          }
   
          State=0; //Once the snooze button is pressed, it will return to state 0 and will
          clockgo();//initiate clockgo() to resume back to normal
        }

    
      }
      }
      
}


 void printDigits(int digits) {
    // utility function for digital clock display: prints preceding colon and leading 0 from the TIME library
      lcd.print(":");
      if (digits < 10)
      lcd.print('0');
      lcd.print(digits);
}

void clockgo(){ //clockgo() is created to make it easier for me to set the alarm on the main clock screen.

  delay(900);
      lcd.clear();
      lcd.print(" Time: ");
       
      lcd.print(hour());
      printDigits(minute());
      printDigits(second());
      lcd.setCursor(0, 1);
      lcd.print("Alarm: ");
      lcd.print(Hours);
      printDigits(Minutes); //calling printDigits to make the values of minutes and seconds appear in a time format, xx:xx:xx etc.
      printDigits(Seconds);
}


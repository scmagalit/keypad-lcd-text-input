
/*
 16x2 LCD + 4x3 keypad with Arduino Uno to make old phone-ish keypad inputs
 
  The circuit:
 * LCD RS pin to digital pin 13
 * LCD Enable pin to digital pin 12
 * LCD D4 pin to digital pin 11
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 9
 * LCD D7 pin to digital pin 8
 * LCD R/W pin to ground
 * LCD Vee to ground
 
 * Keypad R1 to digital pin 1
 * Keypad R2 to digital pin 2
 * Keypad R3 to digital pin 3
 * Keypad R4 to digital pin 4
 * Keypad C1 to digital pin 5
 * Keypad C2 to digital pin 6
 * Keypad C3 to digital pin 7

*/

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;

//keypad setup
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {1, 2, 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 6, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//current cursor position
int thisRow = 0;
int thisCol = 0;

//for delay before cursor change
unsigned long press_start = 0;
bool key_press = false;

//fresh start/reset key state
int prev_key = 99;
int key_cycle = 0;

//for number keys only, special functions for '*' and '#' key
const byte KEY_NUM = 10;
const byte CYCLE_NUM = 9;
char keyset[KEY_NUM][CYCLE_NUM] = {
  {'0','+','-','(',')','<','>','*','#'},
  {'1',',','.','?','!',';',':','/',' '},
  {'a','b','c','A','B','C','2',' ',' '},
  {'d','e','f','D','E','F','3',' ',' '},
  {'g','h','i','G','H','I','4',' ',' '},
  {'j','k','l','J','K','L','5',' ',' '},
  {'m','n','o','M','N','O','6',' ',' '},
  {'p','q','r','s','P','Q','R','S','7'},
  {'t','u','v','T','U','V','8',' ',' '},
  {'w','x','y','z','W','X','Y','Z','9'},
};

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(numCols, numRows);
  lcd.blink();
}

void loop() {
  char key = keypad.getKey();
  
  if (key){
    //process key pressed
    get_key(key);
  }   
  
  if ((millis() - press_start >= 1000) && key_press) { /* waiting time for input expired */
    cursor_next();
    key_press = false;
    prev_key = 99;
  }
  
  lcd.setCursor(thisCol, thisRow);
}

void cursor_next() {
  thisCol++;
  
  if (thisCol >= numCols) {
    thisCol = 0;
    thisRow++;
    
    if (thisRow >= numRows) {
      thisRow = 0;
    }
  }
}

void cursor_prev() {
  thisCol--;
  
  if (thisCol < 0) {
    thisCol = numCols - 1;
    thisRow--;
    
    if (thisRow < 0) {
      thisRow = numRows - 1;
    }
  }
}

void set_wait() {
  press_start = millis();
  key_press = true;
}

void get_key(char key) {
  if (prev_key == key) { /* press same key within waiting time */
    key_cycle++;

    if (key_cycle >= CYCLE_NUM) {
      key_cycle = 0;
    }
  } else if (prev_key == 99) { /* reset key*/
    key_cycle = 0;
    key_press = false;
  } else { /*bypass cursor waiting if another key is pressed*/
    key_cycle = 0;
    key_press = false;
    cursor_next();
    lcd.setCursor(thisCol, thisRow);
  }
  
  switch (key) {
     case '1':
      lcd.write(keyset[1][key_cycle]);
      set_wait();
      break;
     case '2':
      lcd.write(keyset[2][key_cycle]);
      set_wait();
      break;
     case '3':
      lcd.write(keyset[3][key_cycle]);
      set_wait();
      break;
     case '4':
      lcd.write(keyset[4][key_cycle]);
      set_wait();
      break;
     case '5':
      lcd.write(keyset[5][key_cycle]);
      set_wait();
      break;
     case '6':
      lcd.write(keyset[6][key_cycle]);
      set_wait();
      break;
     case '7':
      lcd.write(keyset[7][key_cycle]);
      set_wait();
      break;
     case '8':
      lcd.write(keyset[8][key_cycle]);
      set_wait();
      break;
     case '9':
      lcd.write(keyset[9][key_cycle]);
      set_wait();
      break;
     case '0':
      lcd.write(keyset[0][key_cycle]);
      set_wait();
      break;
     case '*': /* backspace */
      cursor_prev();
      lcd.setCursor(thisCol, thisRow);
      lcd.write(' ');
      key = 99;
      break;
     case '#': /* space */      
      cursor_next();
      lcd.setCursor(thisCol, thisRow);
      key = 99;
      break;
  }
  
  prev_key = key;
}

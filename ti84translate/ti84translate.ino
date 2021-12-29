#include <Keyboard.h>
#include <Mouse.h>

/*
 * Pins
 * 
 * CLOCK - Clock input from calculator
 * DATA - Data input from calculator
 * RESET - Reset button
 * PRIMARY - LED indicator for primary key map
 * SECONDARY - LED indicator for secondary key map
 */

#define CLOCK 9
#define DATA 7
#define RESET 1
#define PRIMARY 8
#define SECONDARY 10

byte keyFlags[7] = {0, 0, 0, 0, 0, 0, 0};
byte keyStates[7] = {0, 0, 0, 0, 0, 0, 0};
bool mapState = false;
const byte unusedFlags[7][2] = {{0, 4}, {0, 5}, {0, 6}, {0, 7}, {1, 7}, {2, 7}, {5, 0}};


/*
 * Key Reference Chart
 * 
 * Warning: If two buttons in the same column are pressed at the same time,
 * do not press a third button or else garbage will be read.
 * 
 * IX    0      1      2      3       4    5     6      7     < BIT
 * 0  - [Down ][Left ][Right][Up    ][   ][    ][     ][     ]
 * 1  - [Enter][+    ][-    ][*     ][/  ][^   ][Clear][     ]
 * 2  - [(-)  ][3    ][6    ][9     ][)  ][Tan ][Vars ][     ]
 * 3  - [.    ][2    ][5    ][8     ][(  ][Cos ][Prgm ][Stat ]
 * 4  - [0    ][1    ][4    ][7     ][,  ][Sin ][Apps ][XTOn ]
 * 5  - [     ][Sto  ][Ln   ][Log   ][x^2][x^-1][Math ][Alpha]
 * 6  - [Graph][Trace][Zoom ][Window][Y= ][2nd ][Mode ][Del  ]
*/


/*
 * Keyboard buttons can be mapped below.
 * NULL means an unmapped key.
 * Don't map keyboard inputs and other inputs (such as mouse inputs) to the same button.
 * Two maps can be set and can be freely switched between with a key combo.
 * 
 * Check below for a list of modifier keys:
 * https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
 */
const byte keyboardMap[2][7][8] =
{
  { //Key Map 1
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {KEY_RETURN, KEY_F10, NULL, NULL, NULL, NULL, NULL, NULL},
    {NULL, '3', '6', '9', ' ', KEY_LEFT_CTRL, KEY_LEFT_SHIFT, NULL},
    {NULL, '2', '5', '8', KEY_RIGHT_ARROW, NULL, 'z', 'd'},
    {'0', '1', '4', '7', KEY_DOWN_ARROW, KEY_UP_ARROW, 'f', 's'},
    {NULL, NULL, NULL, NULL, KEY_LEFT_ARROW, NULL, 'c', 'a'},
    {NULL, NULL, KEY_F5, KEY_F3, KEY_ESC, 'q', 'w', 'e'}
  },
  { //Key Map 2
    {KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, NULL, NULL, NULL, NULL},
    {KEY_RETURN, '\'', 'w', 'r', 'm', 'h', KEY_BACKSPACE, NULL},
    {',', '/', 'v', 'q', 'l', 'g', '\t', NULL},
    {'.', 'z', 'u', 'p', 'k', 'f', 'c', '\\'},
    {' ', 'y', 't', 'o', 'j', 'e', 'b', ';'},
    {NULL, 'x', 's', 'n', 'i', 'd', 'a', KEY_LEFT_CTRL},
    {KEY_F5, KEY_F4, KEY_F3, KEY_F2, KEY_F1, KEY_LEFT_SHIFT, '-', '='}
  }
};

/*
 * Determines whether keyboard inputs will be holdable or pressed only once.
 * Scrolling is the only mouse input that can be affected by this.
 * Does not affect slow mode.
 * Two sets are provided to accomodate both key maps
 * 0 = holdable
 * 1 = press once
 */

const bool activateOnce[2][7][8] =
{
  { //Key Map 1
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0}
  },
  { //Key Map 2
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

/*
 * Mouse controls are mapped by register number (IX) followed by BIT number
 * These can be referenced from the above table
 * To unmap a control, set the values to an unused flag such as {0, 7}
 * The first set is for Key Map 1. The second is for Key Map 2
 */

const byte mouseDown[2][2] =    {{0, 0}, {0, 7}};
const byte mouseLeft[2][2] =    {{0, 1}, {0, 7}};
const byte mouseRight[2][2] =   {{0, 2}, {0, 7}};
const byte mouseUp[2][2] =      {{0, 3}, {0, 7}};
const byte mouseLClick[2][2] =  {{6, 1}, {0, 7}};
const byte mouseRClick[2][2] =  {{6, 0}, {0, 7}};
const byte mouseMWUp[2][2] =    {{1, 6}, {0, 7}};
const byte mouseMWDown[2][2] =  {{1, 5}, {0, 7}};

/*
 * Other mappings
 */

/*
 * Slow Mode
 * Slows down mouse movement for more precise control.
 * The first set is for Key Map 1. The second is for Key Map 2
 */
const byte slowMode[2][2] =     {{5, 5}, {0, 7}};

/*
 * Switch Map
 * Switches between two Key Maps.
 * Both buttons must be pressed to trigger.
 * Set both to the same value to trigger on a single key press.
 * Cannot be held.
 */
const byte switchMap[2][2] =    {{1, 0}, {1, 6}};

/*
 * Code
 */

void setup() {
  pinMode(DATA, INPUT);
  pinMode(PRIMARY, OUTPUT);
  pinMode(SECONDARY, OUTPUT);
  digitalWrite(PRIMARY, 1);
  attachInterrupt(digitalPinToInterrupt(RESET), reset, RISING);
  attachInterrupt(digitalPinToInterrupt(CLOCK), receive, CHANGE);
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  //Switch maps
  if(bitRead(keyFlags[switchMap[0][0]], switchMap[0][1]) + bitRead(keyFlags[switchMap[1][0]], switchMap[1][1]) == 2){
    reset();
    mapState = !mapState;
    digitalWrite(PRIMARY, !mapState);
    digitalWrite(SECONDARY, mapState);
  }

  //Mouse control
  byte mouseDx = bitRead(keyFlags[mouseRight[mapState][0]], mouseRight[mapState][1]) - bitRead(keyFlags[mouseLeft[mapState][0]], mouseLeft[mapState][1]);
  byte mouseDy = bitRead(keyFlags[mouseDown[mapState][0]], mouseDown[mapState][1]) - bitRead(keyFlags[mouseUp[mapState][0]], mouseUp[mapState][1]);
  byte mouseDs = bitRead(keyFlags[mouseMWUp[mapState][0]], mouseMWUp[mapState][1]) - bitRead(keyFlags[mouseMWDown[mapState][0]], mouseMWDown[mapState][1]);
  
  if(mouseDx + mouseDy + mouseDs != 0){
    Mouse.move(mouseDx, mouseDy, mouseDs);
    if(activateOnce[mapState][mouseMWUp[mapState][0]][mouseMWDown[mapState][1]]) bitClear(keyFlags[mouseMWUp[mapState][0]], mouseMWUp[mapState][1]);
    if(activateOnce[mapState][mouseMWDown[mapState][0]][mouseMWDown[mapState][1]]) bitClear(keyFlags[mouseMWDown[mapState][0]], mouseMWDown[mapState][1]);
  }
  if(bitRead(keyFlags[mouseLClick[mapState][0]], mouseLClick[mapState][1]) != bitRead(keyStates[mouseLClick[mapState][0]], mouseLClick[mapState][1])){
    keyStates[mouseLClick[mapState][0]] ^= 1 << mouseLClick[mapState][1];
    if(bitRead(keyStates[mouseLClick[mapState][0]], mouseLClick[mapState][1])) Mouse.press(MOUSE_LEFT);
    else Mouse.release(MOUSE_LEFT);
  }
  if(bitRead(keyFlags[mouseRClick[mapState][0]], mouseRClick[mapState][1]) != bitRead(keyStates[mouseRClick[mapState][0]], mouseRClick[mapState][1])){
    keyStates[mouseRClick[mapState][0]] ^= 1 << mouseRClick[mapState][1];
    if(bitRead(keyStates[mouseRClick[mapState][0]], mouseRClick[mapState][1])) Mouse.press(MOUSE_RIGHT);
    else Mouse.release(MOUSE_RIGHT);
  }

  //Keyboard control
  for(byte IX = 0; IX < 7; IX++){
    for(byte BIT = 0; BIT < 8; BIT++){
      if(keyboardMap[mapState][IX][BIT] == NULL) continue;
      if(activateOnce[mapState][IX][BIT]){
        if(bitRead(keyFlags[IX], BIT)){
          Keyboard.write(keyboardMap[mapState][IX][BIT]);
          bitClear(keyFlags[IX], BIT);
        }
      } else {
        if(bitRead(keyFlags[IX], BIT) != bitRead(keyStates[IX], BIT)){
          keyStates[IX] ^= 1 << BIT;
          if(bitRead(keyStates[IX], BIT)) Keyboard.press(keyboardMap[mapState][IX][BIT]);
          else Keyboard.release(keyboardMap[mapState][IX][BIT]);
        }
      }      
    }
  }

  //Slow mode
  if(bitRead(keyFlags[slowMode[mapState][0]], slowMode[mapState][0])) delay(3);
}

byte buf = 0;
byte len = 0;
void receive(){
  buf = (buf << 1) | digitalRead(DATA);
  len++;
  if(len == 8){
    bitWrite(keyFlags[buf >> 4], buf & 0b00000111, bitRead(buf, 3));
    for(byte i = 0; i < 7; i++) if(bitRead(keyFlags[unusedFlags[i][0]], unusedFlags[i][1])) bitClear(keyFlags[unusedFlags[i][0]], unusedFlags[i][1]);
    len = 0;
  }
}

void reset(){
  Keyboard.releaseAll();
  Mouse.release(MOUSE_LEFT);
  Mouse.release(MOUSE_RIGHT);
  for(byte i = 0; i < 7; i++){
    keyFlags[i] = 0;
    keyStates[i] = 0;
  }
}

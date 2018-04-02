#include <Keyboard.h>
#include <Wire.h>

//
#define QWERTY 0
#define WORKMAN_NATIVE 1
#define ZUBACHEV 2
#define LEFT 0
#define RIGHT 1


unsigned byte currentLayout = 0;



byte rows[] = {5,6,7,8,4};//open collector
const int rowCount = sizeof(rows)/sizeof(rows);

byte cols[] = {10,16,14,15,18,19,20,21};//input pullup
const int colCount = sizeof(cols)/sizeof(cols);

unsigned char layouts[][2][5][8]={
  //QWERTY
  {
    //left
    {{KEY_ESC,'1','2','3','4','5','?','?'},
     {KEY_TAB,'q','w','e','r','t','?','?'},
     {KEY_CAPS_LOCK,'a','s','d','f','g',';','\''},
     {KEY_LEFT_SHIFT,'z','x','c','v','b','/','?'},
     {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,'?','?',KEY_RIGHT_SHIFT,KEY_RIGHT_CTRL,KEY_RETURN}},
    //right
    {{'?',254,'6','7','8','9','0',KEY_BACKSPACE},
     {'?','?','y','u','i','o','p','?'},
     {'[',']','h','j','k','l',';','\''},
     {'=','?','n','m',',','.','/','?'},
     {KEY_RETURN,KEY_BACKSPACE,' ',KEY_LEFT_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,KEY_DOWN_ARROW,'?'}}
  }
  ,//WORKMAN NATIVE
  {
    //left
    {{KEY_ESC,'1','2','3','4','5','?','?'},
     {KEY_TAB,'q','w','e','r','t','?','?'},
     {KEY_CAPS_LOCK,'a','s','d','f','g',';','\''},
     {KEY_LEFT_SHIFT,'z','x','c','v','b','/','?'},
     {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,'?','?',KEY_RIGHT_SHIFT,KEY_RIGHT_CTRL,KEY_RETURN}},
    //right
    {{'?',254,'6','7','8','9','0',KEY_BACKSPACE},
     {'?','?','y','u','i','o','p','?'},
     {'[',']','h','j','k','l',';','\''},
     {'=','?','n','m',',','.','/','?'},
     {KEY_RETURN,KEY_BACKSPACE,' ',KEY_LEFT_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,KEY_DOWN_ARROW,'?'}}
  }
  ,//ZUBACHEV'S LAYOUT
  {
    //left
    {{KEY_ESC,'1','2','3','4','5','?','?'},
     {KEY_TAB,'q','w','e','r','t','?','?'},
     {KEY_CAPS_LOCK,'a','s','d','f','g',';','\''},
     {KEY_LEFT_SHIFT,'z','x','c','v','b','/','?'},
     {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,'?','?',KEY_RIGHT_SHIFT,KEY_RIGHT_CTRL,KEY_RETURN}},
    //right
    {{'?',254,'6','7','8','9','0',KEY_BACKSPACE},
     {'?','?','y','u','i','o','p','?'},
     {'[',']','h','j','k','l',';','\''},
     {'=','?','n','m',',','.','/','?'},
     {KEY_RETURN,KEY_BACKSPACE,' ',KEY_LEFT_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,KEY_DOWN_ARROW,'?'}}
  }
};

//array to store current keys state
byte keys[][5][8] = {
   //left
  {{0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0}}
  ,//right
  {{0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0}}
};

//array to store previous keys state
char prevState[][5][8]={
   //left
  {{0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0}}
  ,//right
  {{0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0}}
};

uint8_t matrixSize = sizeof(layout_left);
uint8_t row = 0;
uint8_t col = 0;;


void setup() {
    pinMode(9,INPUT_PULLUP);//emergency stop
    Wire.begin();
    Keyboard.begin();
    
    for(int x=0; x<rowCount; x++) {
        //Serial.print(rows[x]); Serial.println(" as input");
        pinMode(rows[x], INPUT);
    }
 
    for (int x=0; x<colCount; x++) {
        //Serial.print(cols[x]); Serial.println(" as input-pullup");
        pinMode(cols[x], INPUT_PULLUP);
    }
}
void readMatrix() {
    // iterate the columns
    
    Wire.requestFrom(8,40);//while we read state of matrix here, another arduino do
                           //the same thing at its side 
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        byte curCol = cols[colIndex];
        pinMode(curCol, OUTPUT);
        digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
            byte rowCol = rows[rowIndex];
            pinMode(rowCol, INPUT_PULLUP);
            keys[RIGHT][rowIndex][colIndex] = digitalRead(rowCol);
          pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);    
    }
    row = 0;
    col = 0;
    while (Wire.available()) { // TODO:
                               // slave may send less than requested
                               // but unfortunately I don't want control or
                               // fix it now
    if(col>7) {
      col = 0;
      row++;  
    }
    keys[LEFT][row][col++]=Wire.read();
  }
}



void sendPreses(){ // here we send presses to PC
  for(int row=0;row<5;row++){
    for(int col=0;col<8;col++){
      //right part
      if(keys[RIGHT][row][col]<prevState[RIGHT][row][col]){ //if button press after being unpresed
        Keyboard.press(layout[currentLayout][RIGHT][row][col]);
      }else if(keys_right[row][col]>prevState_right[row][col]){
        Keyboard.release(layout[currentLayout][RIGHT][row][col]);
      }

      //left part
      if(keys[LEFT][row][col]<prevState[LEFT][row][col]){ //if button press after being unpresed
        Keyboard.press(layout[currentLayout][LEFT][row][col]);
      }else if(keys[LEFT][row][col]>prevState[LEFT][row][col]){
        Keyboard.release(layout[currentLayout][LEFT][row][col]);
      }
    }
  }
}
void loop() {
    if(digitalRead(9) != LOW){
      readMatrix();
      //readMatrix_left();
      sendPreses();
      //saving current state as previous
      /*
      for(int i = 0; i<5; i++){
        memcpy(prevState_left[i], keys_left[i], sizeof(keys_left[i]));
      }*/
      for(int i = 0; i<5; i++){
        memcpy(prevState_right[i], keys_right[i], sizeof(keys_right[i]));
      }
      for(int i = 0; i<5; i++){
        memcpy(prevState_left[i], keys_left[i], sizeof(keys_left[i]));
      }
    }else{
      Keyboard.releaseAll();
    }
}



#include <Keyboard.h>
#include <Wire.h>
#define LEFT 0
#define RIGHT 1

byte currentLayout = 0; 

//right part
byte rows_right[] = {5,6,7,8,4};//open collector
const int rowCount = sizeof(rows_right)/sizeof(rows_right[0]);

byte cols_right[] = {10,16,14,15,18,19,20,21};//input pullup
const int colCount = sizeof(cols_right)/sizeof(cols_right[0]);

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
  ,//WORKMAN PHONETIC LAYOUT
  {
    //left
    {{KEY_ESC,'1','2','3','4','5','?','?'},
     {KEY_TAB,'i','l','h','d',',','?','?'},
     {KEY_CAPS_LOCK,'f','c','x','n','u',';','\''},
     {KEY_LEFT_SHIFT,'p','[','v','w',';','/','?'},
     {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,'?','?',KEY_RIGHT_SHIFT,KEY_RIGHT_CTRL,KEY_RETURN}},
    //right
    {{'?',254,'6','7','8','9','0',KEY_BACKSPACE},
     {'?','?','m','a','e','g',']','?'},
     {'[',']','s','y','t','j','b','\''},
     {'=','?','r','k',',','.','o','?'},
     {KEY_RETURN,KEY_BACKSPACE,' ',KEY_LEFT_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,KEY_DOWN_ARROW,'?'}}
  }
  ,//WORKMAN PHONETIC QUASI LAYOUT
  {
    //left
    {{KEY_ESC,'1','2','3','4','5','?','?'},
     {KEY_TAB,'i','l','h','d',',','?','?'},
     {KEY_CAPS_LOCK,'z','c','x','n','u',';','\''},
     {KEY_LEFT_SHIFT,'p','[','v','w',';','/','?'},
     {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,'?','?',KEY_RIGHT_SHIFT,KEY_RIGHT_CTRL,KEY_RETURN}},
    //right
    {{'?',254,'6','7','8','9','0',KEY_BACKSPACE},
     {'?','?','m','a','.','g',']','?'},
     {'[',']','s','y','\'','`','b','\''},
     {'=','?','r','k',',','.','o','?'},
     {KEY_RETURN,KEY_BACKSPACE,' ',KEY_LEFT_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,KEY_DOWN_ARROW,'?'}}
  }
};

uint8_t matrixSize = sizeof(layouts[LEFT]);
uint8_t row = 0;
uint8_t col = 0;;


void setup() {
    pinMode(9,INPUT_PULLUP);
    Wire.begin();
    Keyboard.begin();
    
    for(int x=0; x<rowCount; x++) {
        //Serial.print(rows[x]); Serial.println(" as input");
        pinMode(rows_right[x], INPUT);
    }
 
    for (int x=0; x<colCount; x++) {
        //Serial.print(cols[x]); Serial.println(" as input-pullup");
        pinMode(cols_right[x], INPUT_PULLUP);
    }
}
void readMatrix() {
    // iterate the columns
    Wire.requestFrom(8,40);
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        byte curCol = cols_right[colIndex];
        pinMode(curCol, OUTPUT);
        digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
            byte rowCol = rows_right[rowIndex];
            pinMode(rowCol, INPUT_PULLUP);
            keys[RIGHT][rowIndex][colIndex] = digitalRead(rowCol);
          pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);    
    }
    row = 0;
    col = 0;
    while (Wire.available()) { // slave may send less than requested
    if(col>7) {
      col = 0;
      row++;  
    }
    keys[LEFT][row][col++]=Wire.read();
  }
}



void sendPreses(){
  for(int row=0;row<5;row++){
    for(int col=0;col<8;col++){
      //right part
      char trigger = layouts[currentLayout][RIGHT][row][col];
      if(keys[RIGHT][row][col]<prevState[RIGHT][row][col]){ //if button press after being unpresed
        Keyboard.press(trigger);
      }else if(keys[RIGHT][row][col]>prevState[RIGHT][row][col]){
        Keyboard.release(trigger);
      }

      
      //left part
      trigger = layouts[currentLayout][LEFT][row][col];
      if(keys[LEFT][row][col]<prevState[LEFT][row][col]){ //if button press after being unpresed
        Keyboard.press(layouts[currentLayout][LEFT][row][col]);
      }else if(keys[LEFT][row][col]>prevState[LEFT][row][col]){
        Keyboard.release(layouts[currentLayout][LEFT][row][col]);
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
      memcpy(prevState[0], keys[0], sizeof(keys));
    }else{
      Keyboard.releaseAll();
    }
}


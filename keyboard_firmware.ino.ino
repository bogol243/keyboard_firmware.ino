#include <Keyboard.h>

// JP1 is an input
byte rows[] = {5,6,7,8,9};//open collector
const int rowCount = sizeof(rows)/sizeof(rows[0]);
 
// JP2 and JP3 are outputs
byte cols[] = {10,16,14,15,18,19,20,21};//input pullup
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte currentLayout = 0;
 
byte keys[5][8]={{0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0}};

char prevState[5][8]={{0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0}};
unsigned char layout[5][8]={{'?',254,'6','7','8','9','0','?'},
                            {'?','?','y','u','i','o','p','?'},
                            {'[',']','h','j','k','l',';','\''},
                            {'=','?','n','m',',','.','/','?'},
                            {KEY_RETURN,KEY_BACKSPACE,' ',KEY_LEFT_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,KEY_DOWN_ARROW,'?'}
  };

void setup() {
    pinMode(3,INPUT_PULLUP);
    //Serial.begin(9600);
    //while (!Serial) ;
    
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
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        byte curCol = cols[colIndex];
        pinMode(curCol, OUTPUT);
        digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
            byte rowCol = rows[rowIndex];
            pinMode(rowCol, INPUT_PULLUP);
            keys[rowIndex][colIndex] = digitalRead(rowCol);
            /*
            if(keys[rowIndex][colIndex]==0){
              Keyboard.press(layout[rowIndex][colIndex]);
              if(layout[rowIndex][colIndex]==220){
              Keyboard.print("layout changed");  
            }
            }else{
              Keyboard.release(layout[rowIndex][colIndex]);
            }*/
          pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);    
    }
        
}

/*
void printMatrix() {
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        if (rowIndex < 10)
            Serial.print(F("0"));
        Serial.print(rowIndex); Serial.print(F(": "));
 
        for (int colIndex=0; colIndex < colCount; colIndex++) {  
            Serial.print(keys[rowIndex][colIndex]);
            
            if (colIndex < colCount)
                Serial.print(F(", "));
        }   
        Serial.println("");
    }
    Serial.println("");
}*/

void sendPreses(){
  for(int row=0;row<5;row++){
    for(int col=0;col<8;col++){
      if(keys[row][col]<prevState[row][col]){ //if button press after being unpresed
        Keyboard.press(layout[row][col]);
        if(layout[row][col]==254) Keyboard.print("reprogram button");
      }else if(keys[row][col]>prevState[row][col]){
        Keyboard.release(layout[row][col]);
      }
    }
  }
}
void loop() {
    if(digitalRead(3) != LOW){
      readMatrix();
      sendPreses();
      //saving current state as previous
      for(int i = 0; i<5; i++){
        memcpy(prevState[i], keys[i], sizeof(keys[i]));
      }
      
    }else{
      Keyboard.releaseAll() ;  
    }
}


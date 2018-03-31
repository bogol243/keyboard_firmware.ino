#include <Keyboard.h>
#include <Wire.h>

//right part
byte rows_right[] = {5,6,7,8,4};//open collector
const int rowCount = sizeof(rows_right)/sizeof(rows_right[0]);

byte cols_right[] = {10,16,14,15,18,19,20,21};//input pullup
const int colCount = sizeof(cols_right)/sizeof(cols_right[0]);
 
byte keys_right[5][8]={{0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0}};

char prevState_right[5][8]={{0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0}};
unsigned char layout_right[5][8]={{'?',254,'6','7','8','9','0','?'},
                            {'?','?','y','u','i','o','p','?'},
                            {'[',']','h','j','k','l',';','\''},
                            {'=','?','n','m',',','.','/','?'},
                            {KEY_RETURN,KEY_BACKSPACE,' ',KEY_LEFT_ARROW,KEY_UP_ARROW,KEY_RIGHT_ARROW,KEY_DOWN_ARROW,'?'}
  };

//left part
byte keys_left[5][8]={{0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0}};

char prevState_left[5][8]={{0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0}};
                      
unsigned char layout_left[5][8]={{'?','1','2','3','4','5','?','?'},
                                 {'?','q','y','u','i','o','?','?'},
                                 {'?',']','h','j','k','l',';','\''},
                                 {'?','?','n','m',',','.','/','?'},
                                 {'?','?','?','?','?','?','?'}};

uint8_t matrixSize = sizeof(layout_left);
uint8_t row = 0;
uint8_t col = 0;;


void setup() {
    pinMode(3,INPUT_PULLUP);
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
    Wire.request(1,matrix_size);
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        byte curCol = cols_right[colIndex];
        pinMode(curCol, OUTPUT);
        digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
            byte rowCol = rows_right[rowIndex];
            pinMode(rowCol, INPUT_PULLUP);
            keys_right[rowIndex][colIndex] = digitalRead(rowCol);
          pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);    
    }
    row = 0;
    col = 0;
    while(Wire.avaliable()<matrix_size)
    while(Wire.avaliable()){
      if(col>=colCount){
        col = 0;
        row++;
      }
      keys_left[row][col++];
    }
}



void sendPreses(){
  for(int row=0;row<5;row++){
    for(int col=0;col<8;col++){
      //right part
      if(keys_right[row][col]<prevState_right[row][col]){ //if button press after being unpresed
        Keyboard.press(layout_right[row][col]);
      }else if(keys_right[row][col]>prevState_right[row][col]){
        Keyboard.release(layout_right[row][col]);
      }

      //left part
      if(keys_left[row][col]<prevState_left[row][col]){ //if button press after being unpresed
        Keyboard.press(layout_right[row][col]);
      }else if(keys_left[row][col]>prevState_left[row][col]){
        Keyboard.release(layout_left[row][col]);
      }
    }
  }
}
void loop() {
    if(digitalRead(3) != LOW){
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
    }else{
      Keyboard.releaseAll() ;  
    }
}



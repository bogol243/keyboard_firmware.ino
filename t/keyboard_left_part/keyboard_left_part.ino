#include <Wire.h>


byte rows[] = {5,6,7,4,9};//open collector
const int rowCount = 5;

byte cols[] = {10,16,14,15,18,19,20,21};//input pullup
const int colCount = sizeof(cols)/sizeof(cols[0]);
 
uint8_t keys[5][8]={{0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}};
                 
void setup(){
    Wire.begin(8);
    Wire.onRequest(sendPreses);
    //Serial.begin(9600);
    //while(!Serial);
    for(int x=0; x<rowCount; x++) {
       // Serial.print(rows[x]); Serial.println(" as input");
        pinMode(rows[x], INPUT);
    }
 
    for (int x=0; x<colCount; x++) {
       // Serial.print(cols[x]); Serial.println(" as input-pullup");
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
          pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);    
    }
}
void printMatrix() {
    for (int rowIndex=0; rowIndex < 5; rowIndex++) {
        if (rowIndex < 10)
            Serial.print(F("0"));
        Serial.print(rowIndex); Serial.print(F(": "));
 
        for (int colIndex=0; colIndex < 8; colIndex++) {  
            Serial.print(keys[rowIndex][colIndex]);
            
            if (colIndex < 8)
                Serial.print(F(", "));
        }   
        Serial.println("");
    }
    Serial.println("");
}


void sendPreses(){
  //readMatrix();
  //printMatrix();
  Wire.write(keys[0],40);//write 40 bytes to master
  
  
}

void loop() {
  readMatrix();
  //printMatrix();
  //delay(100);
}



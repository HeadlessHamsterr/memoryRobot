/*
  All the values for movement aren't calibrated, positions aren't right.
*/

#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>
#include <stdio.h>
#include <string.h>
#include "mydef.h"

void home();
void homeXDone();
void homeYDone();
void homeZDone();
void move(int x, int y, int z);
void dirNeg(byte pin);
void dirPos(byte pin);
void chooseCard();  //Pick a card from the array of card positions
void readCard(int cardX, int cardY);  //Read a card type and store it in the array
void getCard(int pairX1, int pairY1, int pairX2, int pairY2); //Read the selected cards, if they are the same, collect them
void pickupCard(bool pickup);
bool checkForPairs();
void showCard(int cardX, int cardY);
const char* lookUpCardType(const char* cardUID);  //Take the cardUID and output readable card type
void setSpeed(int speed); //Calculates step time from desired speed in mm/s

MFRC522 rfid(SS_PIN, RST_PIN);

bool firstRun = true;

int pairX1;
int pairY1;
int pairX2;
int pairY2;

void setup() {
  Serial.begin(9600);
  //SPI.begin();
  //rfid.PCD_Init();

  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(X_ENDSTOP_PIN, INPUT);
  pinMode(Y_ENDSTOP_PIN, INPUT);
  pinMode(Z_ENDSTOP_PIN, INPUT);

  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Z_ENABLE_PIN, LOW);

  home();
  Serial.println("Home");
  delay(1000);
  move(X_SIZE/2, Y_SIZE/2, 0);
  Serial.println("Punt 1 bereikt");
  delay(1000);
  move(50, 50, 0);
  Serial.println("Punt 2 bereikt");
/*
  home();
  delay(2000);
  move(X_SIZE/2, Y_SIZE/2, Z_HEIGHT); //Move to the center*/
}
 
void loop() {
  /*
  if(!firstRun){  //If the machine hasn't read a card, no cards will be the same, so skip the checking
    if(checkForPairs){  //Check if there is a pair
      getCard(pairX1, pairY1, pairX2, pairY2);  //Collect the matching cards
    }else{
      chooseCard(); //Read two randomly selected cards
    }
  }else{
    chooseCard();
    firstRun = false;
  }*/
}

void move(int x, int y, int z){
  bool XShouldMove;
  bool YShouldMove;
  bool ZShouldMove;

  int distanceToX = x - currentX;
  int distanceToY = y - currentY;
  int distanceToZ = z - currentZ;

  int stepsToX = STEPS_PER_MM_X * distanceToX;
  int stepsToY = STEPS_PER_MM_Y * distanceToY;
  int stepsToZ = STEPS_PER_MM_Z * distanceToZ;

  int stepsMadeX = 0;
  int stepsMadeY = 0;
  int stepsMadeZ = 0;

  if(stepsToX < 0){   //Destination is below current location
    stepsToX = stepsToX * -1;   //Negative steps can't be executed, so invert the step amount
    dirNeg(X_DIR_PIN);
    XShouldMove = true;   //Set a flag move the X axis
    XMoveFinished = false;    //Set a flag to indicate X is moving
  }else if(stepsToX > 0){   //Destination is above current location
    dirPos(X_DIR_PIN);
    XShouldMove = true;
    XMoveFinished = false;
  }else{  //Destination is current location
    XShouldMove = false;    //Set a flag not to move the X axis
    XMoveFinished = true;   //set a flag to indicate X is at the destination
  }

  if(stepsToY < 0){
    stepsToY = stepsToY * -1;
    dirNeg(Y_DIR_PIN);
    YShouldMove = true;
    YMoveFinished = false;
  }else if(stepsToY > 0){
    dirPos(Y_DIR_PIN);
    YShouldMove = true;
    YMoveFinished = false;
  }else{
    YShouldMove = false;
    YMoveFinished = true;
  }

  if(stepsToZ < 0){
    stepsToZ = stepsToZ * -1;
    dirNeg(Z_DIR_PIN);
    ZShouldMove = true;
    ZMoveFinished = false;
  }else if(stepsToZ > 0){
    dirPos(Z_DIR_PIN);
    ZShouldMove = true;
    ZMoveFinished = false;
  }else{
    ZShouldMove = false;
    ZMoveFinished = true;
  }

  while(!XMoveFinished || !YMoveFinished || !ZMoveFinished){

    if(XShouldMove){  //Move X if X should be moving
      if(stepsMadeX < stepsToX){  //If the amount of steps that have been made are less then the necessary amount of steps, make another steps
        digitalWrite(X_STEP_PIN, HIGH);
        stepsMadeX++;
      }else{  //If enough steps are made, the axis is at the destination
        XShouldMove = false;  //Set a flag that A shouldn't be moving
        XMoveFinished = true; //Set a flag that A is finished moving
      }
    }

    if(YShouldMove){
      if(stepsMadeY < stepsToY){
        digitalWrite(Y_STEP_PIN, HIGH);
        stepsMadeY++;
      }else{
        YShouldMove = false;
        YMoveFinished = true;
      }
    }

    if(ZShouldMove){
      if(stepsMadeZ < stepsToZ){
        digitalWrite(Z_STEP_PIN, HIGH);
        stepsMadeZ++;
      }else{
        ZShouldMove = false;
        ZMoveFinished = true;
      }
    }

    delayMicroseconds(STEP_TIME_Y);
    digitalWrite(X_STEP_PIN, LOW);
    digitalWrite(Y_STEP_PIN, LOW);
    digitalWrite(Z_STEP_PIN, LOW);
    delayMicroseconds(STEP_TIME_Y);
  }

  currentX = x;
  currentY = y;
  currentZ = z;
}

void home(){
  homingX = true; //Set a flag to indicate axis is homing
  homingY = true;
  homingZ = true;

  /*attachInterrupt(digitalPinToInterrupt(X_ENDSTOP_PIN), homeXDone, FALLING); //Attach interrupt to endstop pin of X axis
  attachInterrupt(digitalPinToInterrupt(Y_ENDSTOP_PIN), homeYDone, FALLING);
  attachInterrupt(digitalPinToInterrupt(Z_ENDSTOP_PIN), homeZDone, FALLING);*/

  Serial.print(digitalRead(X_ENDSTOP_PIN));
  Serial.print(" ");
  Serial.print(digitalRead(Y_ENDSTOP_PIN));
  Serial.print(" ");
  Serial.println(digitalRead(Z_ENDSTOP_PIN));

  while(homingX || homingY || homingZ){ //Move axis if not all axis are homed

    if(!digitalRead(X_ENDSTOP_PIN)){
      homingX = false;
    }
    if(!digitalRead(Y_ENDSTOP_PIN)){
      homingY = false;
    }
    if(!digitalRead(Z_ENDSTOP_PIN)){
      homingZ = false;
    }

    if(homingX){
      dirNeg(X_DIR_PIN);
      digitalWrite(X_STEP_PIN, HIGH);
    }
    if(homingY){
      dirNeg(Y_DIR_PIN);
      digitalWrite(Y_STEP_PIN, HIGH);
    }
    if(homingZ){
      dirNeg(Z_DIR_PIN);
      digitalWrite(Z_STEP_PIN, HIGH);
    }
    delayMicroseconds(STEP_TIME_Y);
    digitalWrite(X_STEP_PIN, LOW);
    digitalWrite(Y_STEP_PIN, LOW);
    digitalWrite(Z_STEP_PIN, LOW);
    delayMicroseconds(STEP_TIME_Y);
  }

  currentX = 0; //Set current position of axis to zero position
  currentY = 0;
  currentZ = 0;

  

  /*detachInterrupt(X_ENDSTOP_PIN);
  detachInterrupt(Y_ENDSTOP_PIN);
  detachInterrupt(Z_ENDSTOP_PIN);*/
}

void homeXDone(){
  homingX = false;
  Serial.println("X Homed");
}

void homeYDone(){
  homingY = false;
  Serial.println("Y Homed");
}

void homeZDone(){
  homingZ = false;
  Serial.println("Z Homed");
}

void dirNeg(byte pin){
  digitalWrite(pin, HIGH);
}

void dirPos(byte pin){
  digitalWrite(pin, LOW);
}

void chooseCard(){
  int card1X = random(0, 4);  //Pick a random X and Y coordinate for two cards
  int card1Y = random(0, 4);
  int card2X = random(0, 4);
  int card2Y = random(0, 4);

  while(card1X == card2X && card1Y == card2Y){  //If the same card is selected twice, pick a new X coordinate for one
      card2X = random(0, 4);
  }

  readCard(card1X, card1Y); //Read the selected cards
  readCard(card2X, card2Y);
}

void readCard(int cardX, int cardY){  //fixen dat de z positie word bijgehouden tijdens langzaam omlaag bewegen
  int counter = 0;
  const char* kaartUID;
  const char* kaart;
  String kaartString;

  int x = kaartLocatiesX[cardX];
  int y = kaartLocatiesY[cardY];

  move(x, y, Z_POS_CARD+10);  //Move a little above the card, then move down and pick it up
  move(x, y, Z_POS_CARD);
  pickupCard(true);
  move(x, y, Z_POS_CARD+10);
  move(X_POS_SCANNER, Y_POS_SCANNER, Z_POS_CARD+10);  //Move a little above the scanner

  while(!rfid.PICC_IsNewCardPresent() && counter < 4000){ //While the card isn't detected and not too much steps are made, move down
    digitalWrite(Z_STEP_PIN, HIGH);
    delayMicroseconds(STEP_TIME_Y);
    digitalWrite(Z_STEP_PIN, LOW);
    delayMicroseconds(STEP_TIME_Y);
    counter++;
  }
  if(counter == 4000){  //Too much steps are made, no card detected
    Serial.println("Geen kaart gezien");
  }else{  //Card detected
    Serial.println("Kaart gezien");
    for(uint8_t i = 0; i < 4; i++){
      kaartString[i] = rfid.uid.uidByte[i]; //Store UID
    }
    kaartUID = kaartString.c_str(); //Convert UID to const char
    //KaartUID omzetten naar kaartType en opslaan in kaart
    kaarten[cardX][cardY] = kaart;  //Store readable card type in array
    showCard(cardX, cardY); //Show the card to the user
  }

  move(X_POS_SCANNER, Y_POS_SCANNER, Z_POS_CARD+10);  //Return the card to the board
  move(x, y, Z_POS_CARD+10);
  move(x, y, Z_POS_CARD);
  pickupCard(false);
  move(x, y, Z_POS_CARD+10);
}

void getCards(int pairX1, int pairY1, int pairX2, int pairY2){  //fixen dat de z positie word bijgehouden tijdens langzaam omlaag bewegen
  int counter = 0;
  const char* kaart1;
  const char* kaart2;
  const char* kaartType1;
  const char* kaartType2;
  String kaartString;

  int x1 = kaartLocatiesX[pairX1];  //Look up coordinates for selected cards
  int y1 = kaartLocatiesY[pairY1];
  int x2 = kaartLocatiesX[pairX2];
  int y2 = kaartLocatiesY[pairY2];

  move(x1, y1, Z_POS_CARD+10);  //Scan the first card
  move(x1, y1, Z_POS_CARD);
  pickupCard(true);
  move(x1, y1, Z_POS_CARD+10);
  move(X_POS_SCANNER, Y_POS_SCANNER, Z_POS_CARD+10);

  while(!rfid.PICC_IsNewCardPresent() && counter < 4000){
    digitalWrite(Z_STEP_PIN, HIGH);
    delayMicroseconds(STEP_TIME_Y);
    digitalWrite(Z_STEP_PIN, LOW);
    delayMicroseconds(STEP_TIME_Y);
    counter++;
  }
  if(counter == 4000){
    Serial.println("Geen kaart gezien");
  }else{
    Serial.println("Kaart gezien");
    for(uint8_t i = 0; i < 4; i++){
      kaartString[i] = rfid.uid.uidByte[i];
    }
    kaart1 = kaartString.c_str();
    kaartType1 = lookUpCardType(kaart1);
    showCard(pairX1, pairY2);
  }
  move(X_POS_SCANNER, Y_POS_SCANNER, Z_POS_CARD+10);  //Return the first card
  move(x1, y1, Z_POS_CARD+10);
  move(x1, y1, Z_POS_CARD);
  pickupCard(false);
  move(x1, y1, Z_POS_CARD+10);

  move(x2, y2, Z_POS_CARD+10);  //Scan the second card
  move(x2, y2, Z_POS_CARD);
  pickupCard(true);
  move(x2, y2, Z_POS_CARD+10);
  move(X_POS_SCANNER, Y_POS_SCANNER, Z_POS_CARD+10);

  while(!rfid.PICC_IsNewCardPresent() && counter < 4000){
    digitalWrite(Z_STEP_PIN, HIGH);
    delayMicroseconds(STEP_TIME_Y);
    digitalWrite(Z_STEP_PIN, LOW);
    delayMicroseconds(STEP_TIME_Y);
    counter++;
  }
  if(counter == 4000){
    Serial.println("Geen kaart gezien");
  }else{
    Serial.println("Kaart gezien");
    for(uint8_t i = 0; i < 4; i++){
      kaartString[i] = rfid.uid.uidByte[i];
    }
    kaart2 = kaartString.c_str();
    kaartType2 = lookUpCardType(kaart2);
    showCard(pairX2, pairY2);
  }

  move(X_POS_SCANNER, Y_POS_SCANNER, Z_POS_CARD+10);  //Return the second card
  move(x2, y2, Z_POS_CARD+10);
  move(x2, y2, Z_POS_CARD);
  pickupCard(false);
  move(x2, y2, Z_POS_CARD+10);

  if(kaartType1 == kaartType2){ //If the choosen cards are the same, place them on the collected pile
    move(x1, y1, Z_POS_CARD+10);
    move(x1, y1, Z_POS_CARD);
    pickupCard(true);
    move(x1, y1, Z_POS_CARD+10);
    move(collectX, collectY, Z_POS_CARD+10);
    pickupCard(false);

    move(x2, y2, Z_POS_CARD+10);
    move(x2, y2, Z_POS_CARD);
    pickupCard(true);
    move(x2, y2, Z_POS_CARD+10);
    move(collectX, collectY, Z_POS_CARD+10);
    pickupCard(false);
    kaarten[pairX1][pairY1] = "skip";
    kaarten[pairX2][pairY2] = "skip";
  }else{
    kaarten[pairX1][pairY1] = kaartType1;
    kaarten[pairX2][pairY2] = kaartType2;
  }
}

void pickupCard(bool pickup){
 //Als true, zuiging aan, als false, zuiging uit
 //Of zoiets 
}

bool checkForPairs(){
  bool pairFound = false;

  for(int counterY = 0; counterY < BOARD_SIZE_Y && !pairFound; counterY++){ //Check every card in the array against eachother
    for(int counterX = 0; counterX < BOARD_SIZE_X && !pairFound; counterX++){
      for(int y = 0; y < BOARD_SIZE_Y && !pairFound; y++){
        for(int x = counterX+1; x < BOARD_SIZE_X && !pairFound; x++){
          if(kaarten[counterX][counterY] == kaarten[x][y] && counterX != x || counterY != y){ //Selected cards are the same
            if(kaarten[counterX][counterY] != "skip" && kaarten[x][y] != "skip"){
              pairFound = true;
              pairX1 = counterX;  //Save the coordinates of the pairs
              pairY1 = counterY;
              pairX2 = x;
              pairY2 = y;
            }
          }
        }
      }
    }
  }
  
  if(pairFound){
    return true;
  }else if(!pairFound){
    return false;
  }
}

const char* lookUpCardType(const char* cardUID){
  if(cardUID == cardUID1){
    return cardType1;
  }else if(cardUID == cardUID2){
    return cardType1;
  }else if(cardUID == cardUID3){
    return cardType2;
  }else if(cardUID == cardUID4){
    return cardType2;
  }else if(cardUID == cardUID5){
    return cardType3;
  }else if(cardUID == cardUID6){
    return cardType3;
  }else if(cardUID == cardUID7){
    return cardType4;
  }else if(cardUID == cardUID8){
    return cardType4;
  }else if(cardUID == cardUID9){
    return cardType5;
  }else if(cardUID == cardUID10){
    return cardType5;
  }else if(cardUID == cardUID11){
    return cardType6;
  }else if(cardUID == cardUID12){
    return cardType6;
  }else if(cardUID == cardUID13){
    return cardType7;
  }else if(cardUID == cardUID14){
    return cardType7;
  }else if(cardUID == cardUID15){
    return cardType8;
  }else if(cardUID == cardUID16){
    return cardType8;
  }
}

void setSpeed(int speed){
  switch(speed){
    case 1:
      STEP_TIME_Y = 1000;
    break;
    case 2:
      STEP_TIME_Y = 200;
    break;
    case 3:
      STEP_TIME_Y = 100;
    break;
  }
}
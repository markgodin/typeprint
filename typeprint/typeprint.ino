int character;
int startDelay = 2000;
int pressDelay = 50;
int specialDelay = 300;
int charCount;
int maxChar = 78; // for 12 pitch
int letterDelay = 75;

//pins are choosen by the lookup function.
int pin1;
int pin2;
int pin3;
int pin4;

//MUX 1 pins 0-7
int s00 = 22;
int s01 = 23;
int s02 = 24;
int en0 = 25;
//MUX 2 pins 8-15
int s10 = 30;
int s11 = 31;
int s12 = 32;
int en1 = 33;
//Modifier MUX 1 pins 0-7
int s20 = 38;
int s21 = 39;
int s22 = 40;
int en2 = 41;
//Modifier MUX 2 pins 8-15
int s30 = 46;
int s31 = 47;
int s32 = 48;
int en3 = 49;

int S00val;
int S01val;
int S02val;
int S10val;
int S11val;
int S12val;
int S20val;
int S21val;
int S22val;
int S30val;
int S31val;
int S32val;

boolean flag;
boolean delayFlag;
int lastCharacter;
int lastLastCharacter;
int lineEndCharacter;
boolean waitFlag = false;
int nextCharacter;

void setup(){
  pinMode(s00, OUTPUT);
  pinMode(s01, OUTPUT);
  pinMode(s02, OUTPUT);
  pinMode(en0, OUTPUT);
  pinMode(s10, OUTPUT);
  pinMode(s11, OUTPUT);
  pinMode(s12, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(s20, OUTPUT);
  pinMode(s21, OUTPUT);
  pinMode(s22, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(s30, OUTPUT);
  pinMode(s31, OUTPUT);
  pinMode(s32, OUTPUT);
  pinMode(en3, OUTPUT);
 //Disable all MUX outputs
  digitalWrite(en0, HIGH);
  digitalWrite(en1, HIGH);
  digitalWrite(en2, HIGH);
  digitalWrite(en3, HIGH);
  //Set all MUX selectors to 0 (select channel 0 on all four muxs)
  digitalWrite(s00, LOW);
  digitalWrite(s01, LOW);
  digitalWrite(s02, LOW);
  digitalWrite(s10, LOW);
  digitalWrite(s11, LOW);
  digitalWrite(s12, LOW);
  digitalWrite(s20, LOW);
  digitalWrite(s21, LOW);
  digitalWrite(s22, LOW);
  digitalWrite(s30, LOW);
  digitalWrite(s31, LOW);
  digitalWrite(s32, LOW);
  
  lastCharacter = 0;
  lastLastCharacter = 0;
  charCount = 0;
  nextCharacter = 0;
  
  Serial.begin(115200);

//This makes satisfies the carraige return signal for starting the typewriter.
  flag = true;
  character = 200;
  setPins(character);
  convertPin();
  muxPin(pin1, pin2, pin3, pin4);
  typeChar();
  flag = false;
  
  //Serial.println("Now Ready.");
  
}

void loop() {
  
  character = 0;
  delayFlag = false;
  
  //if (Serial.available() > 2) waitFlag = true;
  
  if (Serial.available() == 0) {
   // if (waitFlag) {
      Serial.write("@");
      //waitFlag = false;
    //}
  }
  
  character = getChar();
  
  if (charCount == 0 && character == 'space') character = getChar();
  
  int myPeek = Serial.peek();
  
  if(myPeek != 32) {
    if (charCount == (maxChar-2) && character != 32) {
      nextCharacter = Serial.read();
      myPeek = Serial.peek();
      if (myPeek != 32){
        lineEndCharacter = character;
        character = 45;
        setPins(character);
        convertPin();
        muxPin(pin1, pin2, pin3, pin4);
        typeChar();
        character = 13;
        setPins(character);
        convertPin();
        muxPin(pin1, pin2, pin3, pin4);
        typeChar();
        charCount = 0;
        character = lineEndCharacter;
        setPins(character);
        convertPin();
        muxPin(pin1, pin2, pin3, pin4);
        typeChar();
        charCount = 1;
        character = nextCharacter;
      }
      else {
        setPins(character);
        convertPin();
        muxPin(pin1, pin2, pin3, pin4);
        typeChar();
        character = nextCharacter;
      }
    }
  }
  
  if (charCount > (maxChar-8)) {
    if (character == 32) {
      character = 13;
      charCount = -1;
    }
  }
  
  if (lastCharacter == character) delayFlag = true;
  //if (lastCharacter == character && lastCharacter != lastLastCharacter) typeChar();
  
  lastLastCharacter = lastCharacter;
  lastCharacter = character;
  
  //Serial.print("character: ");
  //Serial.println();
  //Serial.print(character);
  
  setPins(character);
  
  convertPin();
  
  muxPin(pin1, pin2, pin3, pin4);
  
  if (character != 64) typeChar();
  
  charCount ++;
  if (character == 13) charCount = 0;
  
  delay(letterDelay);
}

// ========================================================================

int getChar(){

  int x;
  if (Serial.available() > 0) {
    x = Serial.read();
    return x;
  }
 else getChar();
 }

// convertPin translates the looked up pin code to pins 0-7
void convertPin(){
  if (pin2 >= 9) {
    pin2 = pin2 - 8;
  }
  if (pin4 >= 9) {
    pin4 = pin4 - 8;
    }
}

void typeChar() {
  
  if (S00val == 1) digitalWrite(s00, HIGH);
  if (S01val == 1) digitalWrite(s01, HIGH);
  if (S02val == 1) digitalWrite(s02, HIGH);
  if (S10val == 1) digitalWrite(s10, HIGH);
  if (S11val == 1) digitalWrite(s11, HIGH);
  if (S12val == 1) digitalWrite(s12, HIGH);
  if (S20val == 1) digitalWrite(s20, HIGH);
  if (S21val == 1) digitalWrite(s21, HIGH);
  if (S22val == 1) digitalWrite(s22, HIGH);
  if (S30val == 1) digitalWrite(s30, HIGH);
  if (S31val == 1) digitalWrite(s31, HIGH);
  if (S32val == 1) digitalWrite(s32, HIGH);
  
  if (pin3 != 0) {
    digitalWrite(en2, LOW);
    digitalWrite(en3, LOW);
    delay (10);
  }

  digitalWrite(en0, LOW);
  digitalWrite(en1, LOW);
  delay (pressDelay);
  
  if (flag) delay(startDelay);
  
  digitalWrite(en0, HIGH);
  digitalWrite(en1, HIGH);
  digitalWrite(en2, HIGH);
  digitalWrite(en3, HIGH);
  
  digitalWrite(s00, LOW);
  digitalWrite(s01, LOW);
  digitalWrite(s02, LOW);
  digitalWrite(s10, LOW);
  digitalWrite(s11, LOW);
  digitalWrite(s12, LOW);
  digitalWrite(s20, LOW);
  digitalWrite(s21, LOW);
  digitalWrite(s22, LOW);
  digitalWrite(s30, LOW);
  digitalWrite(s31, LOW);
  digitalWrite(s32, LOW);
  
  if (delayFlag) delay(specialDelay);
  if (pin3 != 0 && delayFlag == false) delay(specialDelay);
}

void muxPin(int a, int b, int c, int d) {
  
  S00val = 0;
  S01val = 0;
  S02val = 0;
  S10val = 0;
  S11val = 0;
  S12val = 0;
  S20val = 0;
  S21val = 0;
  S22val = 0;
  S30val = 0;
  S31val = 0;
  S32val = 0;
  
  if (a == 2 || a == 4 || a == 6 || a == 8) {
   S00val = 1;
  }
  if (a == 3 || a == 4 || a == 7 || a == 8) {
   S01val = 1;
  }
  if (a >= 5) {
   S02val = 1; 
  }
  if (b == 2 || b == 4 || b == 6 || b == 8) {
   S10val = 1;
  }
  if (b == 3 || b == 4 || b == 7 || b == 8) {
   S11val = 1;
  }  
  if (b >= 5) {
   S12val = 1; 
  }
  if (c == 2 || c == 4 || c == 6 || c == 8) {
   S20val = 1;
  }
  if (c == 3 || c == 4 || c == 7 || c == 8) {
   S21val = 1;
  } 
  if (c >= 5) {
   S22val = 1; 
  }  
  if (d == 2 || d == 4 || d == 6 || d == 8) {
   S30val = 1;
  }
  if (d == 3 || d == 4 || d == 7 || d == 8) {
   S31val = 1;
  }
   if (d >= 5) {
   S32val = 1; 
  } 
}

void setPins(int character) {
  
  pin1 = 0;
  pin2 = 0;
  pin3 = 0;
  pin4 = 0;
  
  if (character == 'a' || character == 'A') { pin1 = 2; pin2 = 14; if (character == 'A') { pin3 = 1; pin4 = 16; }}
  else if (character == 'b' || character == 'B') { pin1 = 8; pin2 = 13; if (character == 'B') { pin3 = 1; pin4 = 16; }}
  else if (character == 'c' || character == 'C') { pin1 = 8; pin2 = 12; if (character == 'C') { pin3 = 1; pin4 = 16; }}
  else if (character == 'd' || character == 'D') { pin1 = 5; pin2 = 14; if (character == 'D') { pin3 = 1; pin4 = 16; }}
  else if (character == 'e' || character == 'E') { pin1 = 3; pin2 = 11; if (character == 'E') { pin3 = 1; pin4 = 16; }}
  else if (character == 'f' || character == 'F') { pin1 = 3; pin2 = 12; if (character == 'F') { pin3 = 1; pin4 = 16; }}
  else if (character == 'g' || character == 'G') { pin1 = 3; pin2 = 14; if (character == 'G') { pin3 = 1; pin4 = 16; }}
  else if (character == 'h' || character == 'H') { pin1 = 4; pin2 = 12; if (character == 'H') { pin3 = 1; pin4 = 16; }}
  else if (character == 'i' || character == 'I') { pin1 = 6; pin2 = 13; if (character == 'I') { pin3 = 1; pin4 = 16; }}
  else if (character == 'j' || character == 'J') { pin1 = 4; pin2 = 14; if (character == 'J') { pin3 = 1; pin4 = 16; }}
  else if (character == 'k' || character == 'K') { pin1 = 6; pin2 = 12; if (character == 'K') { pin3 = 1; pin4 = 16; }}
  else if (character == 'l' || character == 'L') { pin1 = 6; pin2 = 14; if (character == 'L') { pin3 = 1; pin4 = 16; }}
  else if (character == 'm' || character == 'M') { pin1 = 7; pin2 = 13; if (character == 'M') { pin3 = 1; pin4 = 16; }}
  else if (character == 'n' || character == 'N') { pin1 = 7; pin2 = 11; if (character == 'N') { pin3 = 1; pin4 = 16; }}
  else if (character == 'o' || character == 'O') { pin1 = 5; pin2 = 11; if (character == 'O') { pin3 = 1; pin4 = 16; }}
  else if (character == 'p' || character == 'P') { pin1 = 5; pin2 = 13; if (character == 'P') { pin3 = 1; pin4 = 16; }}
  else if (character == 'q' || character == 'Q') { pin1 = 2; pin2 = 11; if (character == 'Q') { pin3 = 1; pin4 = 16; }}
  else if (character == 'r' || character == 'R') { pin1 = 3; pin2 = 13; if (character == 'R') { pin3 = 1; pin4 = 16; }}
  else if (character == 's' || character == 'S') { pin1 = 5; pin2 = 12; if (character == 'S') { pin3 = 1; pin4 = 16; }}
  else if (character == 't' || character == 'T') { pin1 = 4; pin2 = 11; if (character == 'T') { pin3 = 1; pin4 = 16; }}
  else if (character == 'u' || character == 'U') { pin1 = 6; pin2 = 11; if (character == 'U') { pin3 = 1; pin4 = 16; }}
  else if (character == 'v' || character == 'V') { pin1 = 8; pin2 = 11; if (character == 'V') { pin3 = 1; pin4 = 16; }}
  else if (character == 'w' || character == 'W') { pin1 = 2; pin2 = 13; if (character == 'W') { pin3 = 1; pin4 = 16; }}
  else if (character == 'x' || character == 'X') { pin1 = 7; pin2 = 12; if (character == 'X') { pin3 = 1; pin4 = 16; }}
  else if (character == 'y' || character == 'Y') { pin1 = 4; pin2 = 13; if (character == 'Y') { pin3 = 1; pin4 = 16; }}
  else if (character == 'z' || character == 'Z') { pin1 = 2; pin2 = 12; if (character == 'Z') { pin3 = 1; pin4 = 16; }}
  else if (character == '1' || character == '#') { pin1 = 3; pin2 = 9; if (character == '#') { pin3 = 1; pin4 = 16; }}
  else if (character == '2' || character == '"') { pin1 = 3; pin2 = 10; if (character == '"') { pin3 = 1; pin4 = 16; }}
  else if (character == '3' || character == '/') { pin1 = 4; pin2 = 9; if (character == '/') { pin3 = 1; pin4 = 16; }}
  else if (character == '4' || character == '$') { pin1 = 4; pin2 = 10; if (character == '$') { pin3 = 1; pin4 = 16; }}
  else if (character == '5' || character == '%') { pin1 = 6; pin2 = 9; if (character == '%') { pin3 = 1; pin4 = 16; }}
  else if (character == '6' || character == '*') { pin1 = 6; pin2 = 10; if (character == '*') { pin3 = 1; pin4 = 16; }}
  else if (character == '7' || character == '&') { pin1 = 5; pin2 = 9; if (character == '&') { pin3 = 1; pin4 = 16; }}
  else if (character == '8' || character == 39) { pin1 = 5; pin2 = 10; if (character == 39) { pin3 = 1; pin4 = 16; }}
  else if (character == '9' || character == 40) { pin1 = 8; pin2 = 9; if (character == 40) { pin3 = 1; pin4 = 16; }}
  else if (character == '0' || character == 41) { pin1 = 8; pin2 = 10; if (character == 41) { pin3 = 1; pin4 = 16; }}
  else if (character == '-' || character == '_') { pin1 = 7; pin2 = 9; if (character == '_') { pin3 = 1; pin4 = 16; }}
  else if (character == '=' || character == '+') { pin1 = 7; pin2 = 10; if (character == '+') { pin3 = 1; pin4 = 16; }}
  else if (character == ';' || character == ':') { pin1 = 1; pin2 = 12; if (character == ':') { pin3 = 1; pin4 = 16; }}
  else if (character == ',' || character == '?') { pin1 = 1; pin2 = 9; if (character == '?') { pin3 = 1; pin4 = 16; }}
  else if (character == '.') { pin1 = 1; pin2 = 10;}
  else if (character == 13) { pin1 = 2; pin2 = 15;}
  else if (character == '`' || character == '^') { pin1 = 1; pin2 = 14; if (character == '^') { pin3 = 1; pin4 = 16; }}
  else if (character == 'Space') { pin1 = 1; pin2 = 15;}
  else if (character == 'TAB') { pin1 = 8; pin2 = 14;}
  else if (character == 32) {pin1 = 1; pin2 = 15;}
  else if (character == 200) {pin1 = 7; pin2 = 14; pin3 = 0; pin4 = 0;}
  //else if (character == 39) {
  else {pin1 = 7; pin2 = 14; pin3 = 0; pin4 = 0;}
//change pitch with '@' symbol // else if (character == 64) {pin1 = 3; pin2 = 9; pin3 = 5; pin4 = 15;}
}

//***********************************************************************//
//*  University of Nottingham                                           *//
//*  Department of Electrical and Electronic Engineering                *//
//*  UoN Open Day Practical                                             *//
//*                                                                     *//
//*  'NEOBoard' - Nottingham Electronics Outreach Board                 *//
//*  Dice Example                                                       *//
//*                                                                     *//
//*  Created By: Nathaniel (Nat) Dacombe                                *//
//***********************************************************************//
//* this example allows the 'NEOBoard' to be used as an electronic dice *//
//* which displays a random value. The dice mode i.e. amount of faces   *//
//* on the dice can be selected from d4, d6, d8, d10, d12. d20 and d100 *//
//***********************************************************************//

// the example code and all other files relating to the 'NEOBoard' project can be accessed using the QR code on your PCB or from: https://github.com/University-of-Nottingham-EEE-Projects/NEOBoard 
// feel free to modify the example and/or create your own versions, or take the Arduino NANO Every microcontroller and use it in an entirely different project - we would love to see what you end up creating!; share these with us by emailing nathaniel.dacombe@nottingham.ac.uk
// feel free to also contact us by emailing nathaniel.dacombe@nottingham.ac.uk with any queries, questions or feedback

// programming the Arduino NANO Every with the Arduino IDE...
// if you haven't programmed the Arduino NANO Every on the current PC or laptop that you are running the Arduino IDE on, chances are that you will need to follow the below steps - if it has, ignore the steps below, if so, read on
// 1. click 'Tools' --> 'Board' --> 'Boards Manager...' and search 'megaavr'
// 2. install the 'Arduino megaAVR Boards’ by Arduino, once completed if you go into 'Tools' --> 'Board' there should be a selection of Arduino megaAVR boards (use the 'Arduino NANO Every' board for this project)

// all note tones and songs are taken from:	https://github.com/robsoncouto/arduino-songs/tree/master by Robson Couto
// comments are added throughout the code for your understanding to explain key processes or functions - if anything is unclear, feel free to ask, or search for the functions online

// finally, enjoy and happy rolling!

// toggle the NEOBoard functions
bool MUTE = false;  // set to true to mute all sounds, set to false to play all sounds
bool ROLL_EFFECT = true;  // set to true to display the roll effect, set to false to disable the effect

// tone definitions for all notes - not all of these are used for the current sounds but are provided if you wish to create your own song sequences
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

// change this to make the song slower or faster
int tempo = 114;

// pin definition of the speaker
int speaker = 9;

// contains the melody of a song, arranged by Chlorondria (https://musescore.com/chlorondria_5/never-gonna-give-you-up_alto-sax)
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, negative numbers are used to represent dotted notes so -4 means a dotted quarter note i.e. a quarter plus an eighteenth
int melody[] = {
  NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, NOTE_E5,-8, NOTE_E5,-8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,-8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,8, NOTE_A4,8, NOTE_A4,8, NOTE_E5,4, NOTE_D5,2
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits), as there are two values per note (pitch and duration), for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// calculates the duration of a whole note in milliseconds
int wholenote = (60000 * 4) / tempo;

// set the default values of sound related variables
int divider = 0, noteDuration = 0;

// pin definitions of the tri-seven segment display
#define a 7
#define b A2
#define c 5
#define d 3
#define e 2
#define f 8
#define g 6
#define dp 4
#define hundreds 10
#define tens 11
#define ones 12

// pin definitions of the buttons
#define ROLL_INPUT A3
#define MODE_INPUT A4

// dice related variable definitions
int diceModes[] = {4, 6, 8, 10, 12, 20, 100}; // set the different dice modes i.e. number of dice faces
int currentModeIndex = 0; // set the starting mode position i.e. d4
int diceMax = diceModes[currentModeIndex];  // create an array

// default low states of the two buttons i.e. not pressed
bool lastModeState = LOW;
bool lastRollState = LOW;


// Setup Function - only runs once
// -----------------------------------------------------------------------
void setup() {
  Serial.begin(9600); // display text on the serial monitor for debugging purposes

  // set the tri-seven segment display pins as outputs
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(dp, OUTPUT);

  pinMode(ones, OUTPUT);
  pinMode(tens, OUTPUT);
  pinMode(hundreds, OUTPUT);

  // set the button pins as inputs
  pinMode(ROLL_INPUT, INPUT);
  pinMode(MODE_INPUT, INPUT);

  // by default set the display high so the segments do not illuminate
  digitalWrite(ones, HIGH);
  digitalWrite(tens, HIGH);
  digitalWrite(hundreds, HIGH);

  randomSeed(analogRead(0)); // randomness seed
}


// Loop Function - this repeats infinitely
// -----------------------------------------------------------------------
void loop() {
  // read the current state of the two buttons
  bool currentRoll = digitalRead(ROLL_INPUT);
  bool currentMode = digitalRead(MODE_INPUT);

  // if the mode button is pressed
  if (currentMode == HIGH && lastModeState == LOW) {
    if (!MUTE) { // only play a sound if mute is false
      // play the select sound
      tone(speaker, NOTE_E6, 80);  // first tone
      delay(80);
      tone(speaker, NOTE_G6, 60);  // second tone - a bit higher than the first
      delay(60);
      noTone(speaker);
    }

    // cycle through the different dice modes
    currentModeIndex = (currentModeIndex + 1) % (sizeof(diceModes) / sizeof(diceModes[0]));
    diceMax = diceModes[currentModeIndex];

    // print the current dice mode for troubleshooting
    Serial.print("Switched to D");
    Serial.println(diceMax);

    // display the dice mode (dXX) for 300ms
    for (int i = 0; i < 300; i++) {
      displayDiceMode(currentModeIndex);
    }
  }

  // update the previous state of the button
  lastModeState = currentMode;

  // if the roll button is pressed
  if (currentRoll == HIGH && lastRollState == LOW) {
    if (ROLL_EFFECT) {  // only display the roll effect is roll effect is true
      rollEffect();
    }

    int roll = random(1, diceMax + 1); // randomly pick a number within the range

    // print the rolled value for troubleshooting
    Serial.print("Rolled: ");
    Serial.println(roll);

    // display the rolled number for 400ms
    for (int i = 0; i < 400; i++) {
      displayNumber(roll);
    }
    
    if ((roll == 100) && (!MUTE)) { // only play the sound if 100 is rolled and mute is false
      celebrationSound();  
    }
  }

  // update the previous state of the button
  lastRollState = currentRoll;

  delay(20); // add a 20ms delay to stop button bouncing
}


// Display Number Function
// -----------------------------------------------------------------------
void displayNumber(int number) {
  int onesDigit = number % 10; // find the LSB i.e. 1s digit
  int tensDigit = (number / 10) % 10; // find the 10s digit
  int hundredsDigit = (number / 100) % 10;  // find the MSB i.e. 100s digit

  //  display the ones digit regardless of the rolled number
  digitalWrite(hundreds, HIGH);
  digitalWrite(tens, HIGH);
  digitalWrite(ones, LOW);
  displayDigit(onesDigit);
  delay(2);
  reset();

  // display the tens digit as well if the number is only two digits
  if ((hundredsDigit == 0) && (tensDigit != 0)) {
    digitalWrite(hundreds, HIGH);
    digitalWrite(tens, LOW);
    digitalWrite(ones, HIGH);
    displayDigit(tensDigit);
    delay(2);
    reset();
  }

  // display the hundreds digit and the tens digit if the number is three digits
  else if (hundredsDigit != 0) {
    digitalWrite(hundreds, LOW);
    digitalWrite(tens, HIGH);
    digitalWrite(ones, HIGH);
    displayDigit(hundredsDigit);
    delay(2);
    reset();

    digitalWrite(hundreds, HIGH);
    digitalWrite(tens, LOW);
    digitalWrite(ones, HIGH);
    displayDigit(tensDigit);
    delay(2);
    reset();
  }
}


// Display Digit Function
// -----------------------------------------------------------------------
void displayDigit(int digit) {
  reset();

  // depending on the digit in the number, depends which segments are displayed
  switch (digit) {
    case 0: zero(); break;
    case 1: one(); break;
    case 2: two(); break;
    case 3: three(); break;
    case 4: four(); break;
    case 5: five(); break;
    case 6: six(); break;
    case 7: seven(); break;
    case 8: eight(); break;
    case 9: nine(); break;
  }
}


// Display Dice Mode Function
// -----------------------------------------------------------------------
void displayDiceMode(int modeIndex) {
  int value = diceModes[modeIndex];

  // determine the first and second digit value of the dice mode
  int onesDigit = value % 10;
  int tensDigit = (value / 10) % 10;

  if (tensDigit == 2 || tensDigit == 1 || (tensDigit == 0) && onesDigit == 0) { // if the dice mode is more than one digit
    // display 'd' on hundreds digit
    digitalWrite(hundreds, LOW);
    digitalWrite(tens, HIGH);
    digitalWrite(ones, HIGH);
    letter_d();
    delay(2);
    reset();

    // display the tens digit
    digitalWrite(hundreds, HIGH);
    digitalWrite(tens, LOW);
    digitalWrite(ones, HIGH);
    displayDigit(tensDigit);
    delay(2);
    reset();

    // display the ones digit
    digitalWrite(hundreds, HIGH);
    digitalWrite(tens, HIGH);
    digitalWrite(ones, LOW);
    displayDigit(onesDigit);
    delay(2);
    reset();
  }

  else {  // otherwise for single digit dice modes, display 'd' on the tens digit
    // display the tens digit
    digitalWrite(hundreds, HIGH);
    digitalWrite(tens, LOW);
    digitalWrite(ones, HIGH);
    letter_d();
    delay(2);
    reset();

    // display the ones digit
    digitalWrite(hundreds, HIGH);
    digitalWrite(tens, HIGH);
    digitalWrite(ones, LOW);
    displayDigit(onesDigit);
    delay(2);
    reset();
  }
}


// Reset Function
// -----------------------------------------------------------------------
void reset() {
  // turn all of the segments off
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(dp, LOW);
}

// Specific Digit Functions - illuminates the appropriate segments
// -----------------------------------------------------------------------
void zero()  { digitalWrite(a, HIGH); digitalWrite(b, HIGH); digitalWrite(c, HIGH); digitalWrite(d, HIGH); digitalWrite(e, HIGH); digitalWrite(f, HIGH); }
void one()   { digitalWrite(b, HIGH); digitalWrite(c, HIGH); }
void two()   { digitalWrite(a, HIGH); digitalWrite(b, HIGH); digitalWrite(g, HIGH); digitalWrite(e, HIGH); digitalWrite(d, HIGH); }
void three() { digitalWrite(a, HIGH); digitalWrite(b, HIGH); digitalWrite(g, HIGH); digitalWrite(c, HIGH); digitalWrite(d, HIGH); }
void four()  { digitalWrite(f, HIGH); digitalWrite(g, HIGH); digitalWrite(b, HIGH); digitalWrite(c, HIGH); }
void five()  { digitalWrite(a, HIGH); digitalWrite(f, HIGH); digitalWrite(g, HIGH); digitalWrite(c, HIGH); digitalWrite(d, HIGH); }
void six()   { digitalWrite(a, HIGH); digitalWrite(f, HIGH); digitalWrite(g, HIGH); digitalWrite(c, HIGH); digitalWrite(d, HIGH); digitalWrite(e, HIGH); }
void seven() { digitalWrite(a, HIGH); digitalWrite(b, HIGH); digitalWrite(c, HIGH); }
void eight() { digitalWrite(a, HIGH); digitalWrite(b, HIGH); digitalWrite(c, HIGH); digitalWrite(d, HIGH); digitalWrite(e, HIGH); digitalWrite(f, HIGH); digitalWrite(g, HIGH); }
void nine()  { digitalWrite(a, HIGH); digitalWrite(b, HIGH); digitalWrite(f, HIGH); digitalWrite(g, HIGH); digitalWrite(c, HIGH); }


// Display 'd' Function
// -----------------------------------------------------------------------
void letter_d() {
  // illuminate the segments to display 'd'
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(g, HIGH);
}


// Celebration Sound Function - only play when 100 is rolled
// -----------------------------------------------------------------------
void celebrationSound() {
  // play the song
  for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    int playDuration = noteDuration * 0.9;
    tone(speaker, melody[thisNote]);

    // display 100 while the song plays
      int onesDigit = 0;
    int tensDigit = 0;
    int hundredsDigit = 1;

    unsigned long startTime = millis();
    while (millis() - startTime < playDuration) {
      // display the ones digit
      digitalWrite(hundreds, HIGH);
      digitalWrite(tens, HIGH);
      digitalWrite(ones, LOW);
      displayDigit(onesDigit);
      delay(2);
      reset();

      // display the tens digit
      digitalWrite(hundreds, HIGH);
      digitalWrite(tens, LOW);
      digitalWrite(ones, HIGH);
      displayDigit(tensDigit);
      delay(2);
      reset();

      // display the hundreds digit
      digitalWrite(hundreds, LOW);
      digitalWrite(tens, HIGH);
      digitalWrite(ones, HIGH);
      displayDigit(hundredsDigit);
      delay(2);
      reset();
    }

    // add a pause to the notes in the song
    noTone(speaker);
    delay(noteDuration - playDuration);
  }
}


// Roll Effect Function
// -----------------------------------------------------------------------
void rollEffect() {
  // array of the notes to play whilst rolling
  int rollNotes[] = {
    NOTE_E4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4
  };

  int ringSegments[] = {a, b, c, d, e, f};  // array of the outside display segments (ring)

  // determine the size of variables
  int numNotes = sizeof(rollNotes) / sizeof(rollNotes[0]);
  int numSegments = sizeof(ringSegments) / sizeof(ringSegments[0]);

  // define the delay values
  int baseDelay = 60;
  int delayStep = 3;

  for (int i = 0; i < numNotes; i++) {  // for each note

    int seg = ringSegments[i % numSegments]; // loop through ring
    int currentDelay = baseDelay + i * delayStep; // calculate the required delay between notes and segments

    // enable the current ring segment
    digitalWrite(seg, HIGH);

    // enable all three seven segment displays
    digitalWrite(hundreds, LOW);
    digitalWrite(tens, LOW);
    digitalWrite(ones, LOW);

    if (!MUTE) {  // play the sound if mute is false
      tone(speaker, rollNotes[i], currentDelay * 0.9);
    }

    delay(currentDelay);

    // reset the speaker and display
    noTone(speaker);
    reset();
    digitalWrite(hundreds, HIGH);
    digitalWrite(tens, HIGH);
    digitalWrite(ones, HIGH);
  }
}
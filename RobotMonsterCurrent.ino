//All of this code is to be freely distributed in the public domain
/*Teensy 3.2/Audio Board by Paul from pjrc. Code edited by Scotty H                         _______________
 *_____  _                          __  __                 _                               | RAWR! V 1.0.2 ||
 |  __ \| |                        |  \/  |               | |                      _()_    |_______________||
 | |__) | |__   __ _ _ __ _ __ ___ | \  / | ___  _ __  ___| |_ ___ _ __          /      \ / ͞͞ ͞͞ ͞͞ ͞͞ ͞͞ ͞͞ ͞͞ ͞͞ ͞
 |  ___/| '_ \ / _` | '__| '_ ` _ \| |\/| |/ _ \| '_ \/ __| __/ _ \ '__|        | () ()  |
 | |    | | | | (_| | |  | | | | | | |  | | (_) | | | \__ \ ||  __/ |          (/\  ͝    /\)
 |_|    |_| |_|\__,_|_|  |_| |_| |_|_|  |_|\___/|_| |_|___/\__\___|_|             |_||_|      © Public Domain - Scotty H
                                                                        
                                                                        */

/*This code is developed with the following products // - pins
--Teensy 3.2 microcontroller
--Teensy Audio Board - 7, 10, 14
--SparkFun Sound detector - 0, 1, 14, A7
--SparkFun tilt sensor - 3
--SPDT switches - 2, 5
--Photoresistor - 16
--4x LED
*/

//Sections of this code use code documentation from SparkFun.com and pjrc.com
//Make sure to check out and support SparkFun.com and pjrc.com

// To configure the output type, first uncomment one of the three
// output objects.  If not using the audio shield, comment out
// the sgtl5000_1 lines in setup(), so it does not wait forever
// trying to configure the SGTL5000 codec chip.
//
// The SD card may connect to different pins, depending on the
// hardware you are using.  Uncomment or configure the SD card
// pins to match your hardware.


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include "Keyboard.h"
#include "Mouse.h"


//Audio Connection Setup
AudioPlaySdWav           playWav1;
// Use one of these 3 output types: Digital I2S, Digital S/PDIF, or Analog DAC
AudioOutputI2S           audioOutput;
//AudioOutputSPDIF       audioOutput;
//AudioOutputAnalog      audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000     sgtl5000_1;

//#####MIC CONNECTIONS#####
// Define hardware connections
#define PIN_GATE_IN 1
#define IRQ_GATE_IN  0
#define PIN_LED_OUT 13
#define PIN_ANALOG_IN A7

//#####AUDIO ADAPTER BOARD#####
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

//#####PIN DEFINITIONS#####
//Light Sensor
const int lightPin=16;  //define a pin for Photo resistor
//LED Happy
//const int ledPin=11;     //define a pin for LED
//Mic
//const int gate=1;  //digital gate from mic
//const int envelope=22;  //analog ping for envelope

//Button/switch
const int buttonPin=3;
const int switchPin=2;
const int switchPin2=5;
const int buttonPin1=4;
const int buttonPin2=20;
const int ledPin=8;

//Define bounce for buttons/tilt sensors/switches
Bounce button0 = Bounce(buttonPin, 1000);
Bounce switch0 = Bounce(switchPin, 10);
Bounce switch1 = Bounce(switchPin2, 10);
Bounce button1 = Bounce(buttonPin1, 1000);
Bounce button2 = Bounce(buttonPin2, 1000);


//#####DECLARE VARIABLES#####
//Light Sensor
int room_light = 1; //1=light 0=dark
int room_status = 0; //checks for change in light
int roomChange = 0; //checks if lights have gone out before making a noise for light

//Mic
int mic_track = 0;  //checks mic change
int ambientNoise = 0; //keeps average ambient noise in room

//LED
int led_status=LOW; //sets the led on or off

//Switch status saves the position of the switch
int switchStatus=1; //0 off and 1 on
int switchStatus2=0;
int switchChange2 = 0;
int eightBallStatus=0;  //1 = tilt is 8ball phrases
int oregonTrailStatus=0;  //1 = start oregon trail program

int switchCount = 0;
int mouseDestroy = 0;
int haxMode = 0;

//Oregon Trail vars
int charType=0;
int kansasRiver=0;
int points=0;
int bearTrap=0;
int bearTrapType = 0;

int playRandom=0; //1 enable 0 clean

//#####SAMPLE SETUP#####
//Each type of sample has its own array that links to a file name (ALWAYS IN CAPS)
//General---------- Samples to play when the 
const int len = 2;  //number of samples in category
char const* sample[] = {"PARTY.WAV", "CHEER.WAV"};  //file names for sound clips

//Loud----------- Samples to play when the monster is yelled at
const int len_loud = 1;
char const* sample_loud[] = {"HOLD1.WAV"};

//Tilt----------- Samples to play when the monster is held upside down
const int len_tilt = 8;
char const* sample_tilt[] = {"TILT1.WAV", "TILT2.WAV", "TILT3.WAV", "TILT4.WAV", "TILT5.WAV", "TILT6.WAV", "TILT7.WAV", "TILT8.WAV"};

//Bright----------- Samples to play when monster is in the light
const int len_bright = 32;
char const* sample_bright[] = {"LIGHT1.WAV", "LIGHT2.WAV", "LIGHT3.WAV", "LIGHT4.WAV", "LIGHT5.WAV", "LIGHT6.WAV", "LIGHT7.WAV", "LIGHT8.WAV", "LIGHT9.WAV", "LIGHT10.WAV", "LIGHT11.WAV", "LIGHT12.WAV", "LIGHT13.WAV", "LIGHT14.WAV", "LIGHT15.WAV", "LIGHT16.WAV", "LIGHT17.WAV", "LIGHT18.WAV", "LIGHT19.WAV", "LIGHT20.WAV", "LIGHT21.WAV", "LIGHT22.WAV", "LIGHT23.WAV", "LIGHT24.WAV", "LIGHT25.WAV", "LIGHT26.WAV", "LIGHT27.WAV", "LIGHT28.WAV", "LIGHT29.WAV", "LIGHT30.WAV", "LIGHT31.WAV", "LIGHT32.WAV"};

//Dark----------- Samples to play when monster is in the dark
const int len_dark = 27;
char const* sample_dark[] = {"DARK1.WAV", "DARK2.WAV", "DARK3.WAV", "DARK4.WAV", "DARK5.WAV", "DARK6.WAV", "DARK7.WAV", "DARK8.WAV", "DARK9.WAV", "DARK10.WAV", "DARK11.WAV", "DARK12.WAV", "DARK13.WAV", "DARK14.WAV", "DARK15.WAV", "DARK16.WAV", "DARK17.WAV", "DARK18.WAV", "DARK19.WAV", "DARK20.WAV", "DARK21.WAV", "DARK22.WAV", "DARK23.WAV", "DARK24.WAV", "DARK25.WAV", "DARK26.WAV", "DARK27.WAV"};

//Startup Samples
const int len_startup = 1;
char const* sample_startup[] = {"STARTUP.WAV"};

//Oregon trail Startup Samples
const int len_otstartup = 9;
char const* sample_otstartup[] = {"OT1.WAV", "OT2.WAV", "OTQ1.WAV", "BANKER.WAV", "CARP.WAV", "FARM.WAV", "OTQ2.WAV", "FORD.WAV", "FLOAT.WAV"};

//Oregon trail question 1 samples
const int len_otq1 = 4;
char const* sample_otq1[] = {"OTUC2.WAV", "OTBANKER.WAV", "OTCARP.WAV", "OTFARM.WAV"};

//Oregon trail question 2 samples for playback review
const int len_otq2 = 2;
char const* sample_otq2[] = {"OTFORD.WAV", "OTFLOAT.WAV"};

//Oregon Trail question 3 samples
const int len_otq3 = 6;
char const* sample_otq3[] = {"CHOP.WAV", "OPEN.WAV", "LEAVE.WAV", "OTCHOP.WAV", "OTOPEN.WAV", "OTLEAVE.WAV"};

//Oregon Trail travel music
const int len_ottravel = 3;
char const* sample_ottravel[] = {"TRAVEL1.WAV", "TRAVEL2.WAV", "TRAVEL3.WAV"};

//Useless switch songs
const int len_songs = 3;
char const* sample_songs[] = {"SWITCH1.WAV", "SWITCH2.WAV", "TITANIC.WAV"};

//Quote Samples
const int len_fakequotes = 13;
char const* sample_fakequotes[] = {"FQ1.WAV", "FQ2.WAV", "FQ3.WAV", "FQ4.WAV", "FQ5.WAV", "FQ6.WAV", "FQ7.WAV", "FQ8.WAV", "FQ9.WAV", "FQ10.WAV", "FQ11.WAV", "FQ12.WAV", "FQ13.WAV"};

//QUOTE from Samples
const int len_quotefrom = 35;
char const* sample_quotefrom[] = {"Q1.WAV", "Q2.WAV", "Q3.WAV", "Q4.WAV", "Q5.WAV", "Q6.WAV", "Q7.WAV", "Q8.WAV", "Q9.WAV", "Q10.WAV", "Q11.WAV", "Q12.WAV", "Q13.WAV", "Q14.WAV", "Q15.WAV", "Q16.WAV", "Q17.WAV", "Q18.WAV", "Q19.WAV", "Q20.WAV", "Q21.WAV", "Q23.WAV", "Q24.WAV", "Q25.WAV", "Q26.WAV", "Q27.WAV", "Q28.WAV", "Q29.WAV", "Q30.WAV", "Q31.WAV", "Q32.WAV", "Q33.WAV", "Q34.WAV", "Q35.WAV"};

//Quote people samples
const int len_people = 20;
char const* sample_people[] = {"ADAMS.WAV", "ALFREDHITCHCOCK.WAV", "BENFRANKLIN.WAV", "BUFFET.WAV", "CHURCHILL.WAV", "CONFUCIUS", "DARWIN.WAV", "EINSTEIN.WAV", "EMERSON.WAV", "GATES.WAV", "HEMINGWAY.WAV", "ME.WAV", "NICHOLASCAGE.WAV", "OPRAHWINFREY.WAV", "ROBOTMOM.WAV", "TOMCRUISE.WAV", "TOMHANKS.WAV", "WILDE.WAV", "WOZ.WAV", "ZUCKERBERG.WAV"};

//Joke Samples
const int len_jokes = 43;
char const* sample_jokes[] = {"JOKE1.WAV", "JOKE2.WAV", "JOKE3.WAV", "JOKE3.WAV", "JOKE4.WAV", "JOKE5.WAV", "JOKE6.WAV", "JOKE7.WAV", "JOKE8.WAV", "JOKE9.WAV", "JOKE10.WAV", "JOKE11.WAV", "JOKE12.WAV", "JOKE13.WAV", "JOKE14.WAV", "JOKE15.WAV", "JOKE16.WAV", "JOKE17.WAV", "JOKE18.WAV", "JOKE19.WAV", "JOKE20.WAV", "JOKE21.WAV", "JOKE22.WAV", "JOKE23.WAV", "JOKE24.WAV", "JOKE25.WAV", "JOKE26.WAV", "JOKE27.WAV", "JOKE28.WAV", "JOKE29.WAV", "JOKE30.WAV", "JOKE31.WAV", "JOKE32.WAV", "JOKE33.WAV", "JOKE34.WAV", "JOKE35.WAV", "JOKE36.WAV", "JOKE37.WAV", "JOKE38.WAV", "JOKE39.WAV", "JOKE40.WAV", "JOKE41.WAV", "JOKE42.WAV", "JOKE43.WAV"};

//8Ball active Samples
const int len_eightballactive = 5;
char const* sample_eightballactive[] = {"BALL1.WAV", "BALL2.WAV", "BALL3.WAV", "BALL4.WAV", "BALL5.WAV"};

//8Ball answer Samples
const int len_eightballanswer = 20;
char const* sample_eightballanswer[] = {"ANSWER1.WAV", "ANSWER2.WAV", "ANSWER.WAV", "ANSWER4.WAV", "ANSWER5.WAV", "ANSWER6.WAV", "ANSWER7.WAV", "ANSWER8.WAV", "ANSWER9.WAV", "ANSWER10.WAV", "ANSWER11.WAV", "ANSWER12.WAV", "ANSWER13.WAV", "ANSWER14.WAV", "ANSWER15.WAV", "ANSWER16.WAV", "ANSWER17.WAV", "ANSWER18.WAV", "ANSWER19.WAV", "ANSWER20.WAV"};

//story to type
const int len_story = 5;
char const* story[] = {"Hey", "I can't talk long", "They're watching me", "More importantly", "They're watching you too"};


void setup(){
  Serial.begin(9600);  //Begin serial communcation
  //pinMode( ledPin, OUTPUT );

  //START OF SD/AUDIO SETUP
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(8);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  //End of SD/audio adapter setup

  //Sound Detection Setup
  //  Configure LED pin as output
  pinMode(PIN_LED_OUT, OUTPUT);
  
  // configure input to interrupt
  pinMode(PIN_GATE_IN, INPUT);
  //attachInterrupt(IRQ_GATE_IN, fcnSoundISR, CHANGE);  //uncommment for gate input
  //End Sound Detection Setup

  //Button config
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Mouse.begin();
  Keyboard.begin();

  fcn_ambientNoise();



  //Play the startup sound
  char const* playSampleStartup = sample_startup[random(0, len)];
  //Serial.println(playSample);
  fcn_playFile(playSampleStartup);

  // Display status
  Serial.println("Initialized");
  
  
}

void loop() {
  fcn_lightSensor();
  fcn_led();
  fcn_micCheck();
  fcn_tilt8Ball();
  fcn_switch();
  fcn_switch2();
  fcn_buttons();
  //fcn_mouseMove();
}

//#####Functions#####
void fcn_playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);

  // Simply wait for the file to finish playing.
  while (playWav1.isPlaying()){
    // uncomment these lines if your audio shield
    // has the optional volume pot soldered
    //float vol = analogRead(15);
    //vol = vol / 1024;
    //sgtl5000_1.volume(vol);
  }
  // serial message to display that the file has successfully completed playback
  Serial.print(filename);
  Serial.println(" Completed playback");
}

void fcn_lightSensor(){
  //Serial.println(analogRead(lightPin)); //sensor debug output
  //If the value of the is less than 900 it starts counting down with room status
  //it will stop at 400
  if (analogRead(lightPin)<900 and (room_status!=(-(400)))){
    room_status--;
    //Once room status hits -20 the signal of lights off is set
    if (room_status==(-(20))){
      room_light=0;
      roomChange = 1;
      fcn_playSampleDark();
      Serial.println("Ah! It's dark!");
    }
  }
  //If the value of light is greater than 980 it counts up to 400
  if (analogRead(lightPin)>980 and (room_status!=400)){
    room_status++;
    //Once room status hits 20 the lights on is set
    if ((room_status==20) && (roomChange == 1)){
      room_light=1;
      fcn_playSampleBright();
      Serial.println("Whoa! It's bright!");
    }
  }
  //Serial.println(room_status); //debugging for room_status count
  //delay(100);
}

void fcn_led(){
  if((room_light == 0) or (eightBallStatus == 1)){
    digitalWrite(ledPin, HIGH);
    //Serial.println("Nightlight Engaged");
  }
  else{
    digitalWrite(ledPin, LOW);
    //Serial.println("Nightlight Terminated");
  }
}

void fcn_switch(){
  switch0.update();
  
  // Check each button for "falling" edge.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if (switch0.fallingEdge()){
    Serial.println("Switch 1 Active");
    switchStatus = 1;
  }
  
  // Check each button for "rising" edge
  if (switch0.risingEdge()){
    Serial.println("Switch 1 Inactive");
    switchStatus = 0;
  }
}

void fcn_switch2(){
  switch1.update();
  
  // Check each button for "falling" edge.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if(switchChange2 == 1){
    if (switch1.fallingEdge()){
      Serial.println("Switch 2 Active");
      if(switchCount < 3){
        fcn_playFile(sample_songs[switchCount]);
      }
      switchStatus2 = 1;
      switchCount ++;
      mouseDestroy = 0;
      if(switchCount > 3){
        //haxMode = 1;
        //mouseDestroy = 1;
        Serial.println("H4444xX0RRRrrrM0D3_ACTIVE@*(^$(!");
        fcn_playFile("H4X.WAV");
        switchCount = 0;
      }
    }
  }
  // Check each button for "rising" edge
  if (switch1.risingEdge()){
    Serial.println("Switch 2 Inactive");
    switchStatus2 = 0;
      if((switchChange2 == 1) && (haxMode == 1)){
        mouseDestroy = 1;
        fcn_keyboardBook();
      }
    switchChange2 = 1;
  }
}


void fcn_micCheck(){
  int value;
  //Serial.println(mic_track);  //debug for mic_track
  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN);

  if((value > ambientNoise + 10) && (switchStatus==1)){
    Serial.println(value);
    Serial.println("LOUD");
    fcn_playSampleLoud();
    fcn_ambientNoise();
  }

  /*else if( (value > 23) && (switchStatus==1) ){ //checks for the value of >25 and switch is on
    if (mic_track>=200000){
      Serial.println("Loud!");
      mic_track=0;
      fcn_playSampleLoud();
    }
  }*/
}

void fcn_ambientNoise(){
  int valueArray [1000];
  int previousNumber = 0;
  int total = 0;
  for(int i = 0; i < 1000; i++){
    int micCheck = analogRead(PIN_ANALOG_IN);
    valueArray[i] = micCheck;
    Serial.println(valueArray[i]);
    Serial.println(i);
  }
  for(int i = 0; i < 1000; i++){
    if(i == 0){
      previousNumber = valueArray[i];
    }
    else if(i == 1){
      total = total + valueArray[i] + previousNumber;
      Serial.println(total);
    }
    else{
      total = total + valueArray[i];
    }
  }
  ambientNoise = total/1000;
  Serial.println("Ambient Volume: ");
  Serial.println(ambientNoise);
}

void fcn_tilt(){  //8Ball mode
  //Serial.println("8Ballmode ON");
  
  // Check each button for "falling" edge.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if (button0.fallingEdge()){
    Serial.println("Tilt Active");
    fcn_playSampleEightBallAnswer();
  }
  
  // Check each button for "rising" edge
  if (button0.risingEdge()){
    Serial.println("Tilt Inactive");
  }
  button0.update();
}

void fcn_tilt8Ball(){
  button0.update();
  int tilted = 0;
  if (eightBallStatus == 1){
    fcn_tilt();
  }
  else if (button0.fallingEdge()){
    tilted = 1;
    for (int i=0; tilted == 1;){
      i++;
      Serial.println(i);
      button0.update();
      if (i == 200000){
        eightBallStatus=1;
        Serial.println("8BallMode ON");
        fcn_playSampleEightBallActive();
        break;
      }
      // Check each button for "rising" edge
      else if (button0.risingEdge()){
        Serial.println("Tilt Inactive");
        fcn_playSampleTilt();
        break;
      }
    }
  }
}
  
  /*if (button0.fallingEdge()){
    Serial.println("Tilt Active");
    fcn_playSampleTilt();
  }*/


void fcn_oregonTrail(){
  Serial.println("Welcome to the Oregon Trail");
  //Play the startup sound
  char const* playSampleOTStartup = sample_otstartup[1];
  //Serial.println(playSample);
  fcn_playFile(playSampleOTStartup);
  fcn_oregonQuestion1();
  oregonTrailStatus = 0;
}

void fcn_oregonQuestion1(){
  //Play the startup sound
  char const* playSampleOTStartup = sample_otstartup[2];
  //Serial.println(playSample);
  fcn_playFile(playSampleOTStartup);
  fcn_oregonQ1();
}

//FIRST QUESTION OREGON TRAIL
//Banker, carpenter, or farmer
void fcn_oregonQ1(){
  int stopit = 0;
  for (int i=3; stopit == 0;){
    if (i > 5){
      i=3;
      stopit=1;
    }
    //Play the startup sound
    char const* playSampleOTStartup = sample_otstartup[i];
    //Serial.println(playSample);
    fcn_playFile(playSampleOTStartup);
    i++;
    for (int j=0; j<200000;){
      j++;
      //Serial.println(j);
      int value;
      //Serial.println(mic_track);  //debug for mic_track
      // Check the envelope input
      value = analogRead(PIN_ANALOG_IN);
      //if the mic gets a value high enough to accept the answer
      if(value > 30){
        Serial.println(value);
        charType = i - 3; // sets 1 for banker, 2 carpenter, 3 for farmer
        int answer = i - 3;
        Serial.println("Trail Question 1");
        Serial.println("You chose " + answer);
        fcn_playFile(sample_otq1[0]);  //  play 'you chose to,'
        fcn_playFile(sample_otq1[answer]);
        stopit=1;
        j=50000000;
        fcn_oregonQuestion2();
      }
    }
  }
}

//Play second question
void fcn_oregonQuestion2(){
  int i = random(0, len_ottravel);
  fcn_playFile(sample_ottravel[i]);
  //Play the startup sound
  fcn_playFile("OTQ2.WAV");
  fcn_oregonQ2();
}


//SECOND QUESTION OREGON TRAIL
//Ford or float the river
void fcn_oregonQ2(){
  Serial.println("chartype");
  Serial.println(charType);
  int stopit = 0;
  
  for (int i=7; stopit == 0;){
    if (i > 8){
      i=7;
      stopit=1;
    }
    //Play the startup sound
    char const* playSampleOTStartup = sample_otstartup[i];
    //Serial.println(playSample);
    fcn_playFile(playSampleOTStartup);
    i++;
    for (int j=0; j<200000;){
      j++;
      //Serial.println(j);
      int value;
      //Serial.println(mic_track);  //debug for mic_track
      // Check the envelope input
      value = analogRead(PIN_ANALOG_IN);
    
      if(value > 30){
        Serial.println(value);
        kansasRiver=i-6;
        fcn_playFile(sample_otq1[0]);  //  play 'you chose to,'
        fcn_playFile(sample_otq2[kansasRiver - 2]); // play the selection
        stopit=1;
        j=50000000;
        if((kansasRiver=1)){
          points=points+24;
          kansasRiver = random(1, 10);
          Serial.print("Kansas River # = ");
          Serial.println(kansasRiver);
          Serial.println("River Crossing Answered");
          fcn_riverCrossing(kansasRiver);
        }
        else if((kansasRiver=2)){
          points=points+38;
          kansasRiver = random(3, 10);
          Serial.print("Kansas River # = ");
          Serial.println(kansasRiver);
          Serial.println("River Crossing Answered");
          fcn_riverCrossing(kansasRiver);
        }
      }
    }
  }
}

//Outcome of river crossing
void fcn_riverCrossing(int kansasRiver){
  fcn_playFile("CALC.WAV");
  // successful river crossing if not 9 or 10
  if(kansasRiver < 8){
    points = points + 48;
    Serial.println("River Crossing SUCCESS!");
    fcn_playFile("OTOS2.WAV");
    fcn_oregonQuestion3();
  }
  // failed river crossing
  else{
    points = 0;
    Serial.println("River Crossing FAILED");
    fcn_playFile("OTOF2.WAV");
    fcn_oregonTrailEnd(points);
  }
}

//Play third question
void fcn_oregonQuestion3(){
  int i = random(0, len_ottravel);
  fcn_playFile(sample_ottravel[i]);
  //Play the startup sound
  fcn_playFile("OTQ3.WAV");
  fcn_oregonQ3();
}

//THIRD QUESTION OREGON TRAIL
//Chop open or leave the person in the bear trap
void fcn_oregonQ3(){
  int stopit = 0;
  for (int i=0; stopit == 0;){
    if (i > 3){
      i=0;
      stopit=1;
    }
    //Play the startup sound
    char const* playSampleOTStartup = sample_otq3[i];
    //Serial.println(playSample);
    fcn_playFile(playSampleOTStartup);
    i++;
    for (int j=0; j<200000;){
      j++;
      //Serial.println(j);
      int value;
      //Serial.println(mic_track);  //debug for mic_track
      // Check the envelope input
      value = analogRead(PIN_ANALOG_IN);
      //if the mic gets a value high enough to accept the answer
      if(value > 30){
        Serial.println(value);
        int answer = i + 2; // 3 CHOP, 4 OPEN, 5 LEAVE
        fcn_playFile(sample_otq1[0]);  //  play 'you chose to,'
        fcn_playFile(sample_otq3[answer]);
        bearTrap = answer - 2; //1 CHOP, 2 OPEN, 3 LEAVE
        stopit=1;
        j=50000000;
        if(bearTrap == 1){ //chop
          points = points + 20;
          bearTrap = random(25, 100);
          bearTrapType = 1;
          Serial.println("Slice 'n Dice");
          fcn_bearTrap(bearTrap);
        }
        else if(bearTrap == 2){  //open
          points = points + 25;
          bearTrap = random(1, 100);
          bearTrapType = 2;
          Serial.println("Trying to open the bear trap!");
          fcn_bearTrap(bearTrap);
        }
        else if(bearTrap == 3){  //leave
          points = points - 30;
          bearTrap = 0;
          bearTrapType = 3;
          Serial.println("Bai!");
          fcn_bearTrap(bearTrap);
        }
      }
    }
  }
}

// bear trap survival forward
void fcn_bearTrap(int bearTrap){
  fcn_playFile("CALC.WAV");
  // successful bear trap avoidance
  if(bearTrap == 0){  // leave
    fcn_playFile("OTOS3L.WAV");
    fcn_playFile("OTFIN.WAV");
    fcn_oregonTrailEnd(points);
    Serial.println("You left them in the bear trap");
  }
  else if(bearTrap < 50){ //successful chop
    if(bearTrapType == 1){
      fcn_playFile("OTOS3C.WAV");
      fcn_playFile("OTFIN.WAV");
      fcn_oregonTrailEnd(points);
    }
    else if(bearTrapType == 2){ //successful open
      fcn_playFile("OTOS3O.WAV");
      fcn_playFile("OTFIN.WAV");
      fcn_oregonTrailEnd(points);
      Serial.println("You opened the bear trap! SUCCESS!");
    }
  }
  // failed bear trap
  else{
    if(bearTrapType == 1){  //fail chop
      fcn_playFile("OTOF3C.WAV");
      points = 0;
      fcn_oregonTrailEnd(points);
      Serial.println("They bled out! FAILED");
    }
    else if(bearTrapType == 2){ //fail open
      fcn_playFile("OTOF3O.WAV");
      points = 0;
      fcn_oregonTrailEnd(points);
      Serial.println("You got trapped! FAILED");
    }
  }
  
}

// End of the Oregon Trail, score points here and exit
void fcn_oregonTrailEnd(int points){
  int finalScore = points * charType;
  Serial.println("Your Final Oregon Trail Score: " + finalScore);
  fcn_playFile("BELL.WAV");
  fcn_playFile("SCORE.WAV");
  //bad at the game
  if(finalScore < 49){
    fcn_playFile("BAD.WAV");
    Serial.println("Bad score :(");
  }
  else if(finalScore < 77){
    fcn_playFile("BELL.WAV");
    fcn_playFile("BELL.WAV");
    fcn_playFile("GOOD.WAV");
    Serial.println("Good score!");
  }
  else if(finalScore < 121){
    fcn_playFile("BELL.WAV");
    fcn_playFile("BELL.WAV");
    fcn_playFile("BELL.WAV");
    fcn_playFile("GREAT.WAV");
    Serial.println("Great score!");
  }
  else if(finalScore > 120){
    fcn_playFile("BELL.WAV");
    fcn_playFile("BELL.WAV");
    fcn_playFile("BELL.WAV");
    fcn_playFile("BELL.WAV");
    fcn_playFile("CHAMP.WAV");
    Serial.println("You are a TRAIL CHAMPION!");
  }
  fcn_playFile("THANKS.WAV");
  Serial.println("end of the trail");
  Serial.println("...");
  Serial.println("..");
  Serial.println(".");
}



void fcn_buttons(){
  button1.update();
  button2.update();

  
  int tilted = 0;
  if (oregonTrailStatus == 1){
    fcn_oregonTrail();
  }
  else if (button1.fallingEdge()){
    //Disable 8Ball mode
    eightBallStatus=0;
    Serial.println(eightBallStatus);
    
    tilted = 1;
    for (int i=0; tilted == 1;){
      i++;
      Serial.println(i);
      button1.update();
      if (i == 200000){
        oregonTrailStatus=1;
        Serial.println("Oregon Trail Button Held");
        //Play the startup sound
        char const* playSampleOTStartup = sample_otstartup[0];
        fcn_playFile(playSampleOTStartup);
        break;
      }
      // Check each button for "rising" edge
      else if (button1.risingEdge()){
        Serial.println("Button1 Lifted");
        fcn_playSampleQuoteFrom();
        break;
      }
    }
  }

  // Check each button for "falling" edge.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if (button2.fallingEdge()){
    Serial.println("Button2 Pressed");
    //Disable 8Ball mode
    eightBallStatus=0;
    Serial.println(eightBallStatus);
    
    //Play Jokes
    fcn_playSampleJokes();
  }

  // Check each button for "rising" edge
  if (button2.risingEdge()){
    Serial.println("Button2 Lifted");
  }
}

void fcn_keyboardBook(){
  Keyboard.press(KEY_LEFT_GUI);
  delay(15);
  Keyboard.release(KEY_LEFT_GUI);
  delay(5000);
  Keyboard.write("notepad");
  delay(5000);
  Keyboard.press(KEY_ENTER);
  delay(15);
  Keyboard.release(KEY_ENTER);
  delay(3000);
  for (int i=0; (i == 0 or i < len_story);){
    Keyboard.write(story[i]);
    delay(1000);
    i++;
      for(int j = 0; (j == 0 or j < 10);){
      Keyboard.write(" .");
      j++;
      delay(500);
      if(j == 4){
        Keyboard.press(KEY_ENTER);
        delay(15);
        Keyboard.release(KEY_ENTER);
        delay(15);
        Keyboard.press(KEY_ENTER);
        delay(15);
        Keyboard.release(KEY_ENTER);
      }
      }
  }
  mouseDestroy = 0;
  haxMode = 0;
}

void fcn_mouseMove(){
  if(mouseDestroy == 2){
    int randomX = random(-20, 21);
    int randomY = random(-20, 21);
    Mouse.move(randomX, randomY);
  }
}

void fcn_playSample(){
  char const* playSample = sample[random(0, len)];
  //Serial.println(playSample);
  fcn_playFile(playSample);
}

void fcn_playSampleLoud(){
  char const* playSampleLoud = sample_loud[random(0, len_loud)];
  //Serial.println(playSampleLoud);
  fcn_playFile(playSampleLoud);
}

void fcn_playSampleTilt(){
  char const* playSampleTilt = sample_tilt[random(0, len_tilt)];
  //Serial.println(playSampleTilt);
  fcn_playFile(playSampleTilt);
}

void fcn_playSampleBright(){
  char const* playSampleBright = sample_bright[random(0, len_bright)];
  //Serial.println(playSampleBright);
  fcn_playFile(playSampleBright);
}

void fcn_playSampleDark(){
  char const* playSampleDark = sample_dark[random(0, len_dark)];
  //Serial.println(playSampleDark);
  fcn_playFile(playSampleDark);
}

void fcn_playSampleFakeQuotes(){
  char const* playSampleFakeQuotes = sample_fakequotes[random(0, len_fakequotes)];
  //Serial.println(playSamplefakequotes);
  fcn_playFile(playSampleFakeQuotes);
}

void fcn_playSampleQuoteFrom(){
  char const* playSampleQuoteFrom = sample_quotefrom[random(0, len_quotefrom)];
  //Serial.println(playSamplequotefrom);
  fcn_playFile(playSampleQuoteFrom);
}

void fcn_playSamplePeople(){
  char const* playSamplePeople = sample_people[random(0, len_people)];
  //Serial.println(playSamplepeople);
  fcn_playFile(playSamplePeople);
}

void fcn_playSampleJokes(){
  char const* playSampleJokes = sample_jokes[random(0, len_jokes)];
  //Serial.println(playSamplejokes);
  fcn_playFile(playSampleJokes);
}

void fcn_playSampleEightBallActive(){
  char const* playSampleEightBallActive = sample_eightballactive[random(0, len_eightballactive)];
  //Serial.println(playSampleEightBallActive);
  fcn_playFile(playSampleEightBallActive);
}

void fcn_playSampleEightBallAnswer(){
  char const* playSampleEightBallAnswer = sample_eightballanswer[random(0, len_eightballanswer)];
  //Serial.println(playSampleEightBallAnswer);
  fcn_playFile(playSampleEightBallAnswer);
}


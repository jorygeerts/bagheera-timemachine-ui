#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int switchPinStageOne = A2;
int switchPinStageTwo = A2;

int ledPinOne = 9;
int ledPinTwo = 9;
int ledPinThree = 9;
int ledPinFour = 9;

#define COMMON_ANODE

void setup()
{
  Serial.begin(9600);


  pinMode(switchPinStageOne, INPUT_PULLUP);
  pinMode(switchPinStageTwo, INPUT_PULLUP);

  //  Make pins 2 ~ 13 output for our LED's
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }

  lcd.init();
  lcd.clear();

  Serial.println("Setup done");
}

const int StageStart = 0;
const int StagePrepOne = 1;
const int StageWait = 2;
const int StagePrepTwo = 3;
const int StageDone = 4;

int Stage = StageDone;

int tickB = 0;
int tickDir = 1;

int num = 0;

bool turnOnLcd = false;
bool lcdIsOn = false;

void loop()
{
  switch (Stage) {
    case StageStart:
      sStart();
      break;
    case StagePrepOne:
      sPrepOne();
      break;
    case StageWait:
      wWait();
      break;
    case StagePrepTwo:
      sPrepTwo();
      break;
    case StageDone:
      sDone();
      break;
  }

  Serial.print("Stage ");
  Serial.print(Stage, DEC);
  Serial.println();

  delay(10);
}

int startRedLvl = 0;
int startRedLvlDir = 1;

void sStart() {
  int v = digitalRead(switchPinStageOne);

  if (v == LOW) {

    Serial.println("Bring LED1 down to neutral");
    for (; startRedLvl >= 10; startRedLvl--) {
      setColor(ledPinOne, startRedLvl, 10, 0);
      delay(10);
    }

    Serial.println("Pulse LED1 Orange a bit");
    for (int i = 0; i < 1000; i++) {
      startRedLvlDir = ledDirectionToggle(startRedLvl, startRedLvlDir, 50, 255);
      startRedLvl += startRedLvlDir;
      setColor(ledPinOne, startRedLvl, startRedLvl / 2, 0);
      delay(10);
    }

    Serial.println("Bring LED1 down to neutral again");
    for (; startRedLvl >= 10; startRedLvl--) {
      setColor(ledPinOne, startRedLvl, startRedLvl, 0);
      delay(10);
    }

    Serial.println("Bring LED1 up to green");
    for (; startRedLvl <= 255; startRedLvl++) {
      setColor(ledPinOne, 10, startRedLvl, 10);
      delay(10);
    }

    Stage = StagePrepOne;
  } else {

    // Pulse LED1 Red
    startRedLvlDir = ledDirectionToggle(startRedLvl, startRedLvlDir, 25, 125);
    startRedLvl += startRedLvlDir;
    setColor(ledPinOne, startRedLvl, 10, 0);
  }
}

int p1RedLvl = 0;
int p1RedDir = 1;
int p1BlueLvl = 0;
int p1GreenLvl = 0;

void sPrepOne() {
  int v = digitalRead(switchPinStageOne);

  if (v == HIGH) {
    Serial.println("Bring LED2 down to off");
    for (; p1RedLvl >= 0; p1RedLvl--) {
      setColor(ledPinTwo, p1RedLvl, 0, 0);
      delay(10);
    }

    Serial.println("Bring LED2 to blue");
    for (; p1BlueLvl <= 255; p1BlueLvl++) {
      setColor(ledPinTwo, 0, 0, p1BlueLvl);
      delay(20);
    }

    Serial.println("Bring LED2 from blue to green");
    for (; p1GreenLvl <= 255; p1GreenLvl++) {
      p1BlueLvl--;
      setColor(ledPinTwo, 0, p1GreenLvl, p1BlueLvl);
      delay(20);
    }

    delay(20);
    Serial.println("Turn on LCD");
    lcd.backlight();
    scrollInText(0, "Quantum matrix", 100);
    scrollInText(1, "activatie klaar", 100);


    Stage = StageWait;
  } else {

    // Pulse LED1 Red
    p1RedDir = ledDirectionToggle(p1RedLvl, p1RedDir, 0, 50);
    p1RedLvl += p1RedDir;
    setColor(ledPinTwo, p1RedLvl * 5, 0, 0);
  }
}

int wDir = 1;
int wRedLvl = 0;
int wBlueLvl = 0;
int wGreenLvl = 0;

void wWait() {
  int v = digitalRead(switchPinStageTwo);

  if (v == LOW) {

    Serial.println("Bring LED3 down to off");
    for (int i = 255; i >= 0; i--) {
      setColor(ledPinThree, min(wRedLvl, i), min(wBlueLvl, i), min(wGreenLvl, i));
      delay(15);
    }

    setColor(ledPinThree, 0, 200, 200);
    setColor(ledPinFour, 0, 200, 200);
    delay(200);

    setColor(ledPinThree, 0, 200, 0);
    setColor(ledPinFour, 0, 0, 200);
    delay(200);

    setColor(ledPinThree, 0, 200, 200);
    setColor(ledPinFour, 0, 200, 200);
    delay(200);

    setColor(ledPinThree, 0, 200, 0);
    setColor(ledPinFour, 0, 0, 200);
    delay(200);

    setColor(ledPinThree, 0, 200, 200);
    setColor(ledPinFour, 0, 200, 200);
    delay(200);

    setColor(ledPinThree, 0, 200, 0);
    setColor(ledPinFour, 0, 0, 200);
    delay(200);

    setColor(ledPinThree, 0, 200, 200);
    setColor(ledPinFour, 0, 200, 200);
    delay(200);

    setColor(ledPinThree, 100, 200, 200);
    setColor(ledPinFour, 100, 200, 200);
    delay(200);

    setColor(ledPinThree, 50, 250, 50);
    setColor(ledPinFour, 50, 250, 50);
    delay(200);

    setColor(ledPinThree, 0, 250, 0);
    setColor(ledPinFour, 0, 250, 0);
    delay(200);

    Stage = StagePrepTwo;
  } else {

    // Up red, up green, up blue, down red, down green, down blue, repeat
    if (wDir == 1) {
      if (wRedLvl < 255) {
        wRedLvl++;
        setColor(ledPinThree, wRedLvl, wBlueLvl, wGreenLvl);
      }
      if (wRedLvl == 255 && wBlueLvl < 255) {
        wBlueLvl++;
        setColor(ledPinThree, wRedLvl, wBlueLvl, wGreenLvl);
      }
      if (wRedLvl == 255 && wBlueLvl == 255 && wGreenLvl < 255) {
        wGreenLvl++;
        setColor(ledPinThree, wRedLvl, wBlueLvl, wGreenLvl);
      }

      if (wRedLvl == 255 && wBlueLvl == 255 && wGreenLvl == 255) {
        wDir = 0;
      }
    } else {
      if (wRedLvl > 0) {
        wRedLvl--;
        setColor(ledPinThree, wRedLvl, wBlueLvl, wGreenLvl);
      }
      if (wRedLvl == 0 && wBlueLvl > 0) {
        wBlueLvl--;
        setColor(ledPinThree, wRedLvl, wBlueLvl, wGreenLvl);
      }
      if (wRedLvl == 0 && wBlueLvl == 0 && wGreenLvl > 0) {
        wGreenLvl--;
        setColor(ledPinThree, wRedLvl, wBlueLvl, wGreenLvl);
      }

      if (wRedLvl == 0 && wBlueLvl == 0 && wGreenLvl == 0) {
        wDir = 1;
      }
    }
  }
}

int p2GreenLvl = 250;
int p2GreenDir = -1;

void sPrepTwo() {
  int v = digitalRead(switchPinStageTwo);

  if (v == HIGH) {
    Serial.println("Bring LED3 back go full green");
    for (; p2GreenLvl <= 255; p2GreenLvl++) {
      setColor(ledPinThree, 0, p2GreenLvl, 0);
      delay(10);
    }

    Serial.println("Bring LED4 off");
    for (int i = 255; i >= 0; i--) {
      setColor(ledPinFour, 0, i, 0);
      delay(20);
    }

    Serial.println("Turn LED4 Red");
    for (int i = 0; i <= 250; i++) {
      setColor(ledPinFour, i, 0, 0);
      delay(20);
    }

    Serial.println("Turn LED4 Oranje");
    for (int i = 0; i <= 125; i++) {
      setColor(ledPinFour, 250, i, 0);
      delay(20);
    }

    Serial.println("Turn LED4 Green");
    for (int i = 0; i <= 125; i++) {
      setColor(ledPinFour, 250 - (i*2), 125 + i, 0);
      delay(20);
    }


    delay(20);
    Serial.println("Turn on LCD");
    lcd.backlight();

    Stage = StageDone;
  } else {

    // Pulse LED1 Red
    p2GreenDir = ledDirectionToggle(p2GreenLvl, p2GreenDir, 0, 250);
    p2GreenLvl += p2GreenDir;
    setColor(ledPinThree, p2GreenLvl, 0, 0);
  }
}

void sDone() {
  lcd.backlight();
  lcd.clear();
  delay(500);
  scrollInText(0, "Tijdmachine is", 100);
  scrollInText(1, "gereedgemaakt", 100);
  delay(1000);
    
}


void scrollInText(int line, char text[], int t) {
  for (int i = 0; i < strlen(text); i++ ) {
    char c = text[i];
    lcd.setCursor(i, line);
    lcd.print(c);
    delay(t);
  }
}

int ledDirectionToggle(int curLvl, int curDir, int minLvl, int maxLvl) {
  if (curLvl >= maxLvl) {
    return -1;
  }

  if (curLvl <= minLvl) {
    return 1;
  }

  return curDir;
}

void setColor(int startPin, int red, int green, int blue)
{
#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif
  analogWrite(startPin + 2, red);
  analogWrite(startPin + 1, green);
  analogWrite(startPin, blue);
}

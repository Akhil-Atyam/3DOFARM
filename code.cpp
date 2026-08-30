//Hello Shipwright!
// I know that hopefully, youll see this message, and I want to say this
// Although small parts of the code were fixed by ai, and some math was taught to me by ai, all implementation is mine
// Quite simply, I was working on this project but was too lazy to make a repo, and I realized, huh I should prob make a repo
// I do understand, the amount of progress right now from 0-100 looks like i just generated it, but i've been working on it for a whileee (25h as of this commit in total for cad and code)
// Anyways hope you have a great day!
// PS: The website is ai but i have plans to remove it i just wanted to try out the functionalities of my coprocessor



#include <WiFi.h>
#include <WebServer.h>
#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>
#include <TickTwo.h> 


// Lowk u could make this super high
#define MACRO_STEPS 56


const char* macroSequence[MACRO_STEPS] = {
  "X6",
  "W15.3",  
  "P1000",  
  "Y-1",  
  "P500",   
  "C55",    
  "P3000",
  "Y2",
  "X2", 
  "90",
  "P12000",
  "X5",
  "Y-3",
  "C180",
  "P1000",
  "Y2",
  "0",
  "P12000",


// second cube

  "X6",
  "W15.3",  
  "P1000",  
  "Y-2",  
  "P500",  
  "C55",    
  "P3000",
  "Y2",
  "X2",
  "90",
  "P12000",
  "X5",
  "Y-2",
  "C180",
  "P1000",
  "Y2",
  "0",
  "P12000",

  "X6",
  "W15.3",  
  "P1000",  
  "Y-3",  
  "P500",  
  "C55",    
  "P3000",
  "Y2",
  "X2",
  "90",
  "P12000",
  "X5",
  "Y-1",
  "C180",
  "P1000",
  "Y2",
  "0",
  ""
};

 
int currentMacroStep = 0;
bool macroRunning = false;


void processIncomingString(String inputString);


void executeNextMacroStep() {
  if (!macroRunning) return;


  extern TickTwo macroTimer;


  if (currentMacroStep < MACRO_STEPS) {
    String commandToRun = String(*(macroSequence + currentMacroStep));
    commandToRun.trim();


    Serial.print(">>> Macro Step [");
    Serial.print(currentMacroStep);
    Serial.print("] Processing: ");
    Serial.println(commandToRun);

    if (commandToRun.length() > 1 && (commandToRun.charAt(0) == 'p' || commandToRun.charAt(0) == 'P')) {
      String msString = commandToRun.substring(1);
      uint32_t pauseDuration = msString.toInt();
      
      if (pauseDuration > 0) {
        macroTimer.interval(pauseDuration);
      }
      
      currentMacroStep++;
      return;
    }

    macroTimer.interval(2000);


    processIncomingString(commandToRun);
    currentMacroStep++;


  } else {
    macroTimer.stop();
    macroRunning = false;
    currentMacroStep = 0;
    Serial.println("Done");
  }
}


// 2000 between steps for now, PLEASE USE 12K OR ABOVE FOR STEPPER MY LAPTOP ALMOST GOT HIT
TickTwo macroTimer(executeNextMacroStep, 2000, 0, MILLIS);


// Password game from neil.fun reference?
const char* ap_ssid = "Robot-Arm-Control";
const char* ap_password = "12345678";      
const char* www_username = "test";
const char* www_password = "test";

       //sss
// WebServer server(80);


#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7      
#define L1 4.43 // 4.43 OG 4.38
#define L2 3.48 // 3.48 OG 3.37




float baseX = 4.0;
float baseY = 0.0;
float turretAngle = 0.0;
float targetIrlAngleL3 = 0.0;


int starts[5] = {100, 173, 90, 180, 80};


#define RGB_PIN 48
#define NUM_PIXELS 1
Adafruit_NeoPixel rgbLed(NUM_PIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);


#define MotorInterfaceType 4
AccelStepper stepper(MotorInterfaceType, IN1, IN3, IN2, IN4);
const float stepsPerDegree = 2048.0 / 54;


#define Stage1 8
#define Stage2 18
#define Stage3 17
#define Claw 16
#define WristRot 15
#define LEDC_FREQ 50
#define MIN_DUTY 111
#define MAX_DUTY 491


unsigned long servoTimer = 0;
bool servosActive = true;
unsigned long lastServoStepTime = 0;
const unsigned long SERVO_STEP_INTERVAL = 85;
const float SERVO_STEP_SIZE = 5.0;


float currentAngles[5] = {100.0, 140.0, 90.0, 90.0, 80.0};
float targetAngles[5] = {100.0, 140.0, 90.0, 90.0, 80.0};
uint8_t servoPins[5] = {Stage1, Stage2, Stage3, Claw, WristRot};


uint32_t degFunc(float deg){
  uint32_t duty = MIN_DUTY + (uint32_t)((deg / 180.0) * (MAX_DUTY - MIN_DUTY));
  return duty;
}

void cosineMachine(float X, float Y){
  //Portions of math here were ai solely because I do not know this math enough.
  float radL3 = targetIrlAngleL3 * PI / 180.0;
  
  X = X - (0 * cos(radL3) - 4.92 * sin(radL3));
  Y = Y - (0 * sin(radL3) + 4.92 * cos(radL3));
  

  float distance = sqrt((X * X) + (Y * Y));
  if (distance > (L1 + L2) || distance < (fabs(L1 - L2) + 0.05)) {
    Serial.println("Out of bounds");
    return;
  }
  float cosAngle2 = ((X * X) + (Y * Y) - (L1 * L1) - (L2 * L2)) / (2.0 * L1 * L2);
  cosAngle2 = constrain(cosAngle2, -1.0, 1.0);
  float angle2Rad = acos(cosAngle2);
  float angle1Rad = atan2(Y, X) + atan2((L2 * sin(angle2Rad)), (L1 + L2 * cosAngle2));
  float stage1Deg = angle1Rad * 180.0 / PI;
  float stage2Deg = angle2Rad * 180.0 / PI;
  
  float finalStage1 = (float)(*(starts + 0)) + (stage1Deg - 90.0);
  float finalStage2 = (float)(*(starts + 1)) - stage2Deg;
  
  finalStage1 = constrain(finalStage1, 0.0, 180.0);
  finalStage2 = constrain(finalStage2, 0.0, 180.0);
  
  float finalStage3 = targetIrlAngleL3 + (finalStage1 + finalStage2);
  finalStage3 = constrain(finalStage3, 0.0, 180.0);
  
  *(targetAngles + 0) = finalStage1;
  *(targetAngles + 1) = finalStage2;
  *(targetAngles + 2) = finalStage3;
  
  Serial.print("X="); Serial.print(baseX);
  Serial.print(" Y="); Serial.print(baseY);
  Serial.print(" WristIRL="); Serial.println(targetIrlAngleL3);
  //Use for bugtest, wrist irl is kinda cooked rn pls fix akhil
  // Serial.println(targetAngles[0]);
  // Serial.println(targetAngles[1]);
  // Serial.println(targetAngles[2]);
}



// control inputs
void processIncomingString(String inputString) {
  inputString.trim();
  if (inputString.length() == 0) return;


  char firstChar = inputString.charAt(0);
  String valueSubstring = inputString.substring(1);
  float servoDegrees = valueSubstring.toFloat();
  
  if (!servosActive) {
    ledcAttach(Stage1, LEDC_FREQ, 12);
    ledcAttach(Stage2, LEDC_FREQ, 12);
    ledcAttach(Stage3, LEDC_FREQ, 12);
    ledcAttach(Claw, LEDC_FREQ, 12);
    ledcAttach(WristRot, LEDC_FREQ, 12);
    servosActive = true;
  }
  servoTimer = millis();
  bool executeIK = false;
  // TODO: i need to use a switch case im a bum...
  if (firstChar == 'x' || firstChar == 'X') {
    baseX = valueSubstring.toFloat();
    executeIK = true;
  } else if (firstChar == 'y' || firstChar == 'Y') {
    baseY = valueSubstring.toFloat();
    executeIK = true;
  } else if (firstChar == 'w' || firstChar == 'W') {
    targetIrlAngleL3 = valueSubstring.toFloat();
    executeIK = true;
  } else if (firstChar == 'a' || firstChar == 'A') {
    servoDegrees = constrain(servoDegrees, 0, 180);
    *(targetAngles + 0) = servoDegrees;
  } else if (firstChar == 's' || firstChar == 'S'){
    servoDegrees = constrain(servoDegrees, 0, 180);
    *(targetAngles + 1) = servoDegrees;
  } else if (firstChar == 'd' || firstChar == 'D'){
    servoDegrees = constrain(servoDegrees, 0, 180);
    *(targetAngles + 2) = servoDegrees;
    targetIrlAngleL3 = *(targetAngles + 2) - (*(targetAngles + 0) + *(targetAngles + 1));
  } else if (firstChar == 'c' || firstChar == 'C'){
    servoDegrees = constrain(servoDegrees, 0, 180);
    *(targetAngles + 3) = servoDegrees;
  } else if (firstChar == 'f' || firstChar == 'F'){
    servoDegrees = constrain(servoDegrees, 0, 180);
    *(targetAngles + 4) = servoDegrees;
  } else if (firstChar == 'q' || firstChar == 'Q'){
    if (!macroRunning) {
      Serial.println("Running");
      currentMacroStep = 0;
      macroRunning = true;
      macroTimer.interval(2000);
      macroTimer.start();
      executeNextMacroStep();
    }
  } else {
    float degreesInput = inputString.toFloat();
    turretAngle = degreesInput;
    long targetStep = (degreesInput) * stepsPerDegree;
    stepper.enableOutputs();
    stepper.moveTo(targetStep * 48 / 40 * 1);
  }


  if (executeIK) {
    cosineMachine(baseX, baseY);
    
  }


}
void handleRoot() {
  // if (!server.authenticate(www_username, www_password)) {
  //   return server.requestAuthentication();
  // }

}


void handleUpdate() {
  // if (!server.authenticate(www_username, www_password)) {
  //   return server.requestAuthentication();
  // }


  // if (server.hasArg("baseX")) baseX = server.arg("baseX").toFloat();
  // if (server.hasArg("baseY")) baseY = server.arg("baseY").toFloat();
  // if (server.hasArg("wrist")) targetIrlAngleL3 = server.arg("wrist").toFloat();
  
  // if (server.hasArg("turret")) {
  //   turretAngle = server.arg("turret").toFloat();
  //   long targetStep = turretAngle * stepsPerDegree;
  //   stepper.enableOutputs();
  //   stepper.moveTo(targetStep * 48 / 40 * 1);
  // }


  // cosineMachine(baseX, baseY);
  // server.sendHeader("Location", "/");
  // server.send(303);
}


void handleMacroTrigger() {
  // if (!server.authenticate(www_username, www_password)) {
  //   return server.requestAuthentication();
  // }
  
  if (!macroRunning) {
    Serial.println("Web.");
    currentMacroStep = 0;
    macroRunning = true;
    macroTimer.interval(2000);
    macroTimer.start();
    executeNextMacroStep();
  }
  
  // server.sendHeader("Location", "/");
  // server.send(303);
}


void setup() {
  Serial.begin(115200);
  delay(2000);
  
  rgbLed.begin();
  rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));
  rgbLed.show();
  
  stepper.setMaxSpeed(400.0);
  stepper.setAcceleration(200.0);
  
  ledcAttach(Stage1, LEDC_FREQ, 12);
  ledcAttach(Stage2, LEDC_FREQ, 12);
  ledcAttach(Stage3, LEDC_FREQ, 12);
  ledcAttach(Claw, LEDC_FREQ, 12);
  ledcAttach(WristRot, LEDC_FREQ, 12);
  
  for(int i = 0; i < 5; i++) {
    *(currentAngles + i) = (float)(*(starts + i));
    *(targetAngles + i) = (float)(*(starts + i));
    ledcWrite(*(servoPins + i), degFunc(*(currentAngles + i)));
  }
  
  targetIrlAngleL3 = *(targetAngles + 2) - (*(targetAngles + 0) + *(targetAngles + 1));
  
  // Serial.print("Loading website");
  // WiFi.softAP(ap_ssid, ap_password);
  
  // Serial.println("\nWebsite active");
  // Serial.print("Hotspot SSID: ");
  // Serial.println(ap_ssid);
  // Serial.print("Web Server Webpage IP: ");
  // Serial.println(WiFi.softAPIP());


  // server.on("/", HTTP_GET, handleRoot);
  // server.on("/update", HTTP_POST, handleUpdate);
  // server.on("/macro", HTTP_POST, handleMacroTrigger);
  // server.begin();
  
  servoTimer = millis();
}


void loop() {
  // server.handleClient();
  macroTimer.update();


  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    inputString.trim();
    
    if (inputString.length() > 0) {
      processIncomingString(inputString);
    }
  }


  if (millis() - lastServoStepTime >= SERVO_STEP_INTERVAL) {
    lastServoStepTime = millis();
    for (int i = 0; i < 5; i++) {
      if (*(currentAngles + i) != *(targetAngles + i)) {
        float diff = *(targetAngles + i) - *(currentAngles + i);
        if (abs(diff) <= SERVO_STEP_SIZE) {
          *(currentAngles + i) = *(targetAngles + i);
        } else if (diff > 0) {
          *(currentAngles + i) += SERVO_STEP_SIZE;
        } else {
          *(currentAngles + i) -= SERVO_STEP_SIZE;
        }
        ledcWrite(*(servoPins + i), degFunc(*(currentAngles + i)));
      }
    }
  }


  if (stepper.distanceToGo() == 0) {
    stepper.disableOutputs();
  } else {
    stepper.run();
  }
}

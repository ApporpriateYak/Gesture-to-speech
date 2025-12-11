#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(2, 3);
DFRobotDFPlayerMini myDFPlayer;

// Flex sensor pins
const int flexPin1 = A0;
const int flexPin2 = A1;
const int flexPin3 = A2;
const int flexPin4 = A3;
const int flexPin5 = A4;

// Enable/disable sensors (set to false for broken sensors)
bool sensor1_enabled = true;
bool sensor2_enabled = true;  // DISABLED - unstable readings
bool sensor3_enabled = true;  // DISABLED - unstable readings
bool sensor4_enabled = true;  // DISABLED - unstable readings
bool sensor5_enabled = false;  // DISABLED - unstable readings

// Baseline values for each sensor
int baseline1 = 250;
int baseline2 = 250;
int baseline3 = 250;
int baseline4 = 250;
int baseline5 = 250;

// Threshold for detecting bend
int threshold = 50; // Lowered threshold for better sensitivity
int threshold1= 150;

// Minimum stable reading to consider sensor valid
const int MIN_STABLE = 50;
const int MAX_STABLE = 950;

// Flags to prevent multiple plays
bool isPlaying1 = false;
bool isPlaying2 = false;
bool isPlaying3 = false;
bool isPlaying4 = false;
bool isPlaying5 = false;

void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  
  Serial.println("Initializing DFPlayer...");
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Unable to begin DFPlayer");
    while (true);
  }
  Serial.println("DFPlayer initialized");
  myDFPlayer.volume(100);
  
  // Calibration period
  Serial.println("\n=== CALIBRATION MODE ===");
  Serial.println("Keep all sensors straight for 3 seconds...");
  delay(3000);
  
  // Calibrate and validate each sensor
  if (sensor1_enabled) {
    baseline1 = calibrateSensor(flexPin1, "Sensor 1");
  }
  if (sensor2_enabled) {
    baseline2 = calibrateSensor(flexPin2, "Sensor 2");
  }
  if (sensor3_enabled) {
    baseline3 = calibrateSensor(flexPin3, "Sensor 3");
  }
  if (sensor4_enabled) {
    baseline4 = calibrateSensor(flexPin4, "Sensor 4");
  }
  if (sensor5_enabled) {
    baseline5 = calibrateSensor(flexPin5, "Sensor 5");
  }
  
  Serial.println("\nReady! Bend sensors to play audio.\n");
}

int calibrateSensor(int pin, String name) {
  int sum = 0;
  int validReadings = 0;
  
  for(int i = 0; i < 20; i++) {
    int reading = analogRead(pin);
    // Only use stable readings (not 0 or 1023)
    if (reading > MIN_STABLE && reading < MAX_STABLE) {
      sum += reading;
      validReadings++;
    }
    delay(50);
  }
  
  if (validReadings < 5) {
    Serial.print(name);
    Serial.println(": UNSTABLE - sensor disabled");
    return 500; // Return dummy value
  }
  
  int baseline = sum / validReadings;
  Serial.print(name);
  Serial.print(": Baseline = ");
  Serial.println(baseline);
  return baseline;
}

void loop() {
  // Read all flex sensor values
  int flexValue1 = analogRead(flexPin1);
  int flexValue2 = analogRead(flexPin2);
  int flexValue3 = analogRead(flexPin3);
  int flexValue4 = analogRead(flexPin4);
  int flexValue5 = analogRead(flexPin5);
  
  // Calculate differences from baseline
  int diff1 = abs(flexValue1 - baseline1);
  int diff2 = abs(flexValue2 - baseline2);
  int diff3 = abs(flexValue3 - baseline3);
  int diff4 = abs(flexValue4 - baseline4);
  int diff5 = abs(flexValue5 - baseline5);
  
  // Print all sensor values with status
  Serial.print("F1: ");
  Serial.print(flexValue1);
  Serial.print(sensor1_enabled ? " [ON] (" : " [OFF] (");
  Serial.print(diff1);
  Serial.print(")");
  
  Serial.print("\tF2: ");
  Serial.print(flexValue2);
  Serial.print(sensor2_enabled ? " [ON] (" : " [OFF] (");
  Serial.print(diff2);
  Serial.print(")");
  
  Serial.print("\tF3: ");
  Serial.print(flexValue3);
  Serial.print(sensor3_enabled ? " [ON] (" : " [OFF] (");
  Serial.print(diff3);
  Serial.print(")");
  
  Serial.print("\tF4: ");
  Serial.print(flexValue4);
  Serial.print(sensor4_enabled ? " [ON] (" : " [OFF] (");
  Serial.print(diff4);
  Serial.print(")");
  
  Serial.print("\tF5: ");
  Serial.print(flexValue5);
  Serial.print(sensor5_enabled ? " [ON] (" : " [OFF] (");
  Serial.print(diff5);
  Serial.println(")");
  
  // Check sensor 1 (only if enabled and reading is stable)
  if (sensor1_enabled && flexValue1 > MIN_STABLE && flexValue1 < MAX_STABLE) {
    if (diff1 >= threshold && !isPlaying1) {
      myDFPlayer.play(2);
      Serial.println(">>> Playing track 2");
      isPlaying1 = true;
      delay(3000);
      isPlaying1 = false;
    }
  }

  if(flexValue2 > 40 && flexValue3 > 200 && flexValue4 > 200){
    myDFPlayer.play(6);
    Serial.println(">>> Playing track 6");
    delay(3000);
    }
     else if (flexValue4 > 200 && flexValue3 > 200 && !isPlaying3 && !isPlaying4) {
      myDFPlayer.play(13);
      Serial.println(">>> Playing track 13");
      isPlaying5 = true;
      delay(3000);
      isPlaying5 = false;
    }
    else if (flexValue4 > 200 && flexValue3 > 300 && !isPlaying3 && !isPlaying4) {
      myDFPlayer.play(8);
      Serial.println(">>> Playing track 8");
      isPlaying5 = true;
      delay(3000);
      isPlaying5 = false;
    }
    else if (flexValue4 > 300 && flexValue3 > 200 && !isPlaying3 && !isPlaying4) {
      myDFPlayer.play(9);
      Serial.println(">>> Playing track 9");
      isPlaying5 = true;
      delay(3000);
      isPlaying5 = false;
    }
    else if (flexValue4 > 150 && flexValue3 > 300 && !isPlaying3 && !isPlaying4) {
      myDFPlayer.play(5);
      Serial.println(">>> Playing track 5");
      isPlaying5 = true;
      delay(3000);
      isPlaying5 = false;
    }
    else if (flexValue4 > 300 && flexValue3 > 200 && !isPlaying3 && !isPlaying4) {
      myDFPlayer.play(10);
      Serial.println(">>> Playing track 10");
      isPlaying5 = true;
      delay(3000);
      isPlaying5 = false;
    }
     else if (flexValue4 > 150 && flexValue3 > 150  && !isPlaying3 && !isPlaying4) {
      myDFPlayer.play(14);
      Serial.println(">>> Playing track 14");
      isPlaying5 = true;
      delay(3000);
      isPlaying5 = false;
    }
    else if (flexValue4 > 100 && flexValue2 > 40 && !isPlaying3 && !isPlaying4) {
      myDFPlayer.play(7);
      Serial.println(">>> Playing track 7");
      isPlaying5 = true;
      delay(3000);
      isPlaying5 = false;
    }
    else if(flexValue3 >=1000 && !isPlaying3){
      myDFPlayer.play(3);
      Serial.println(">>>Playing track 3");
      delay(3000);
      }
    else if (flexValue2 >= 40 && !isPlaying2) {
      myDFPlayer.play(15);
      Serial.println(">>> Playing track 15");
      isPlaying2 = true;
      delay(3000);
      isPlaying2 = false;
    }
    else if (flexValue4 > 150 && !isPlaying4) {
      myDFPlayer.play(12);
      Serial.println(">>> Playing track 12");
      isPlaying4 = true;
      delay(3000);
      isPlaying4 = false;
    }
   else if (flexValue4 > 100 && !isPlaying4) {
      myDFPlayer.play(4);
      Serial.println(">>> Playing track 4");
      isPlaying4 = true;
      delay(3000);
      isPlaying4 = false;
    }
  
  
  //
 
  
  delay(50);
}

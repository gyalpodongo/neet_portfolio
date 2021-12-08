
#include <Statistic.h>
const int EstepPin = 13;
const int EdirPin = 12;
const int EsizPin = 8;
const int RotstepPin = 11;
const int RotdirPin = 10;
const int RotsizPin = 9;
Statistic stat(true);
// Pins with E preface are for elevation (up/down) motors and those with Rot preface are for rotational (left/right) motors.
void setup() {
  // put your setup code here, to run once:
  // Elevation/Up-Down motor
  pinMode(EstepPin, OUTPUT);
  pinMode(EdirPin, OUTPUT);
  pinMode(EsizPin, OUTPUT);
  digitalWrite(EsizPin, HIGH);
  digitalWrite(EdirPin, LOW);
  // Rotation motor
  pinMode(RotstepPin, OUTPUT);
  pinMode(RotdirPin, OUTPUT);
  pinMode(RotsizPin, OUTPUT);
  digitalWrite(RotsizPin, HIGH);
  digitalWrite(RotdirPin, LOW);
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  int aVal = analogRead(A0);
  int bVal = analogRead(A1);
  int cVal = analogRead(A2);
  int dVal =analogRead(A3);
  int counter = 300;
  int Rerrors[counter];
  int Eerrors[counter];
  for (int i = 0; i < 300; i = i + 1) {
    Eerrors[i] = getEerror(aVal,bVal,cVal,dVal);
    Rerrors[i] = getRerror(aVal,bVal,cVal,dVal);
  }
  float averageEerror = stat.average(Eerrors);
  float E_sdev = stat.pop_stdev(Eerrors);
  float averageRerror = stat.average(Rerrors);
  float R_sdev = stat.pop_stdev(Rerrors);
  main(aVal,bVal,cVal,dVal);
}
int getRerror(int aVal,int bVal,int cVal,int dVal){
  int Rerror = (((aVal+cVal)/2) - ((bVal+dVal)/2));
  return Rerror
}
int getEerror(int aVal,int bVal,int cVal,int dVal){
  int Eerror = (((aVal+bVal)/2) - ((cVal+dVal)/2));
  return Eerror
}
void checkPin(int stepPin, int dirPin,int error, int absError, int lowerBoundary, int higherBoundary) {
  if (absError <= lowerBoundary) {
  digitalWrite(stepPin, LOW);
  }else {
  if  (error > higherBoundary) {
    // Set motor to move correct direction
    digitalWrite(dirPin, HIGH);
    // Start stepping in that direction
    digitalWrite(stepPin, LOW);
    delay(50);
    digitalWrite(stepPin, HIGH);
    delay(50);
    // Serial.print("Showing light from upper part");
    //Serial.print("\t");
    //delay(200);
  }else if (error < higherBoundary) {
    // Set motor to move correct direction
    digitalWrite(dirPin, LOW);
    // Start stepping in that direction
    digitalWrite(stepPin, LOW);
    delay(50);
    digitalWrite(stepPin, HIGH);
    delay(50);
    //Serial.print("Showing light from lower part");
   // Serial.print("\t");
    //delay(200);
   }
 }
}
void main(int aVal,int bVal,int cVal,int dVal, float averageRerror, float averageEerror, float Rsdev, float Esdev){
  int currentTime = milis()
  if(currentTime % 300000 > 295000 || currentTime % 300000 < 5000){
    int Rerror = getRerror(aVal,bVal,cVal,dVal);
    int Rposerror = abs(Rerror);
    int Eerror = getEerror(aVal,bVal,cVal,dVal);
    int Eposerror = abs(Eerror)
    checkPin(RotstepPin, RotdirPin, Rerror, Rposerror, averageRerror, averageRerror + 2*Rsdev);
    checkPin(EstepPin,EdirPin, Eerror, Eposerror, averageEerror, averageEerror + 2*Esdev);
  }
}

const int LeftAvoidancePin = A3;
const int RightAvoidancePin = A5;
const int LeftLightPin = A2;
const int RightLightPin = A4;

int dl, dr, LL, LR;
//测试时请把红外寻迹模块的连接线给拔了，只需要拔连接扩展板的一头就行了
void setup() {
  Serial.begin(9600);
  pinMode(LeftAvoidancePin, INPUT);
  pinMode(RightAvoidancePin, INPUT);
  pinMode(LeftLightPin, INPUT);
  pinMode(RightLightPin, INPUT);
  delay(1000);
}
void loop() {
  dl = digitalRead(LeftAvoidancePin);
  dr = digitalRead(RightAvoidancePin);
  LL = analogRead(LeftLightPin);
  LR = analogRead(RightLightPin);
  Serial.print("LeftAvoidance:");
  Serial.print(dl);
  Serial.print("   ");
  Serial.print("RightAvoidance:");
  Serial.println(dr);
  Serial.print("LeftLight:");
  Serial.print(LL);
  Serial.print("   ");
  Serial.print("RightLight:");
  Serial.println(LR);
  delay(1000);
}

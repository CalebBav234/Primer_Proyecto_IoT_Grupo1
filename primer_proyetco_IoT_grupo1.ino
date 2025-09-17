
class LEDGroup {
  int* pins;
  int size;

  public:
    LEDGroup(int p[], int s) : pins(p), size(s) {
      for (int i = 0; i < size; i++) {
        pinMode(p[i], OUTPUT);
        digitalWrite(p[i], LOW);
      }
    }
    void on()  { for (int i = 0; i < size; i++) digitalWrite(pins[i], HIGH); }
    void off() { for (int i = 0; i < size; i++) digitalWrite(pins[i], LOW); }
};

class UltrasonicSensor {
  int trig;
  int echo;

  public:
    UltrasonicSensor(int t, int e) : trig(t), echo(e) {
      pinMode(trig, OUTPUT);
      pinMode(echo, INPUT);
    }

    long readDistance() {
      digitalWrite(trig, LOW);
      delayMicroseconds(2);
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig, LOW);
      long pulse = pulseIn(echo, HIGH, 30000);  
      return (pulse == 0) ? -1 : (pulse * 0.034 / 2);
    }
};

class BolivianFlag {
  long distance;
  int redPins[3]    = {23, 4, 5};
  int yellowPins[3] = {18, 19, 21};
  int greenPins[3]  = {12, 13, 14};
  int thresholds[3] = {10, 20, 30};

  LEDGroup colors[3] = {
    LEDGroup(redPins, 3),
    LEDGroup(yellowPins, 3),
    LEDGroup(greenPins, 3)
  };

  UltrasonicSensor sensor;

  public:
    BolivianFlag(int trigPin, int echoPin) : sensor(trigPin, echoPin) {}

    void loop() {
      distance = sensor.readDistance();

      for (auto &c : colors) c.off();

      if (distance == -1) {
        Serial.println("No object detected.");
      } else {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        for (int i = 0; i < 3; i++) {
          if (distance < thresholds[i]) {
            colors[i].on();
            break;
          }
        }
      }
    }
};

BolivianFlag flag(25, 26);

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Ultrasonic Distance with Bolivian Flag LEDs");
}

void loop() {
  flag.loop();
  delay(200);
}

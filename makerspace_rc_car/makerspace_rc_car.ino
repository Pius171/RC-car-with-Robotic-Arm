#include "BluetoothSerial.h"
#include <ESP32Servo.h>


Servo gripper;  //S1
Servo y;        //S2
Servo x;        // S4
Servo base;     // S3

#define S1 13
#define S2 12
#define S3 15
#define S4 2

//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234";  // Change this to more secure PIN.

String device_name = "ESP32-BT-Slave";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif


BluetoothSerial SerialBT;
char BluetoothData;
//motor pins
#define EN1 25
#define EN2 14
#define EN3 18
#define EN4 22

//M1 and M2 are the motors close to the pcb

//behind left
#define M1A 33
#define M1B 32

//behind right
#define M2A 26
#define M2B 27

// front right
#define M3A 5
#define M3B 4

#define M4A 19
#define M4B 21

void forward() {
  // motor 1
  digitalWrite(33, 1);
  digitalWrite(32, 0);

  //motor 2
  digitalWrite(26, 1);
  digitalWrite(27, 0);

  // motor 3
  digitalWrite(5, 1);
  digitalWrite(4, 0);

  //motor 4
  digitalWrite(19, 1);
  digitalWrite(21, 0);
}

void backward() {
  // motor 1
  digitalWrite(33, 0);
  digitalWrite(32, 1);

  //motor 2
  digitalWrite(26, 0);
  digitalWrite(27, 1);

  // motor 3
  digitalWrite(5, 0);
  digitalWrite(4, 1);

  //motor 4
  digitalWrite(19, 0);
  digitalWrite(21, 1);
}

void left() {
  // motor 1
  digitalWrite(33, 0);
  digitalWrite(32, 1);

  //motor 2
  digitalWrite(26, 1);
  digitalWrite(27, 0);

  // motor 3
  digitalWrite(5, 1);
  digitalWrite(4, 0);

  //motor 4
  digitalWrite(19, 0);
  digitalWrite(21, 1);
}

void right() {
  // motor 1
  digitalWrite(33, 1);
  digitalWrite(32, 0);

  //motor 2
  digitalWrite(26, 0);
  digitalWrite(27, 1);

  // motor 3
  digitalWrite(5, 0);
  digitalWrite(4, 1);

  //motor 4
  digitalWrite(19, 1);
  digitalWrite(21, 0);
}

void stop() {
  digitalWrite(33, 1);
  digitalWrite(32, 1);

  //motor 2
  digitalWrite(26, 1);
  digitalWrite(27, 1);

  // motor 3
  digitalWrite(5, 1);
  digitalWrite(4, 1);

  //motor 4
  digitalWrite(19, 1);
  digitalWrite(21, 1);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gripper.attach(S1);
  y.attach(S2);
  x.attach(S4);
  base.attach(S3);
  SerialBT.begin(device_name);  //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
//Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif

  int pins[] = { 25, 14, 18, 22, 33, 32, 26, 27, 5, 4, 19, 21 };

  for (int i = 0; i <= 11; i++) {
    pinMode(pins[i], OUTPUT);
    if (i <= 3) {
      digitalWrite(pins[i], 1);  // enable all motors
    }
    delay(20);
  }

  // pinMode(25, OUTPUT);  // EN1
  // pinMode(14, OUTPUT);  // EN2
  // pinMode(18, OUTPUT);  // EN3
  // pinMode(22, OUTPUT);  //  EN4

  // pinMode(33, OUTPUT);  // M1A
  // pinMode(32, OUTPUT);  // M1B
  // pinMode(26, OUTPUT);  //M2A
  // pinMode(27, OUTPUT);  // M2B


  // pinMode(5, OUTPUT);   // M3A
  // pinMode(4, OUTPUT);   // M3B
  // pinMode(19, OUTPUT);  //M4A
  // pinMode(21, OUTPUT);  // M4B

  // // enable all motors
  // digitalWrite(25, 1);
  // digitalWrite(14, 1);
  // digitalWrite(18, 1);
  // digitalWrite(22, 1);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
    BluetoothData = SerialBT.read();
    Serial.println(BluetoothData);

    if (BluetoothData == 'F') {
      forward();
      Serial.print("forward");
    }

    if (BluetoothData == 'B') {
      backward();
      Serial.print("backward");
    }

    if (BluetoothData == 'L') {
      left();
      Serial.print("left");
    }

    if (BluetoothData == 'R') {
      right();
      Serial.print("Right");
    }

    if (BluetoothData == 'S') {
      stop();
      Serial.print("Stop");
    }
    if (BluetoothData == 'g') {
      int angle = SerialBT.parseInt();
      gripper.write(angle);
      Serial.println(angle);
    }

    if (BluetoothData == 'x') {
      int angle = SerialBT.parseInt();
      x.write(angle);
      Serial.println(angle);
    }

    if (BluetoothData == 'y') {
      int angle = SerialBT.parseInt();
      //x.write(angle);
      y.write(angle);
      Serial.println(angle);
    }

    if (BluetoothData == 'b') {
      int angle = SerialBT.parseInt();
      base.write(angle);
      Serial.println(angle);
    }
  }

  delay(20);
}

#include <Servo.h>

#define CLAW_NUM 5
#define SERVO_NUM 2

#define CLAW_CLOSE_ANGLE_OFFSET 0
#define CLAW_OPEN_ANGLE_OFFSET 30

const uint8_t CLAW_CLOSE_CMD[CLAW_NUM] = {0x10, 0x20, 0x30, 0x40, 0x50};
const uint8_t CLAW_OPEN_CMD[CLAW_NUM]  = {0x11, 0x21, 0x31, 0x41, 0x51};

const uint8_t CLAW_OPEN_ANGLE[CLAW_NUM][SERVO_NUM]  = {{0+CLAW_OPEN_ANGLE_OFFSET, 180-CLAW_OPEN_ANGLE_OFFSET}, {0+CLAW_OPEN_ANGLE_OFFSET, 180-CLAW_OPEN_ANGLE_OFFSET}, {0+CLAW_OPEN_ANGLE_OFFSET, 180-CLAW_OPEN_ANGLE_OFFSET}, {0+CLAW_OPEN_ANGLE_OFFSET, 180-CLAW_OPEN_ANGLE_OFFSET}, {0+CLAW_OPEN_ANGLE_OFFSET, 180-CLAW_OPEN_ANGLE_OFFSET}};
const uint8_t CLAW_CLOSE_ANGLE[CLAW_NUM][SERVO_NUM] = {{90, 90}, {90, 90}, {90, 90}, {90, 90}, {90, 90}};

Servo servo[CLAW_NUM][SERVO_NUM];
uint8_t pin[CLAW_NUM][SERVO_NUM] = {{2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}};
uint8_t led = 13;

void OpenClaw(int i)
{
  for(int j = 0; j < SERVO_NUM; j++)
    servo[i][j].write(CLAW_OPEN_ANGLE[i][j]);
}

void CloseClaw(int i)
{
  for(int j = 0; j < SERVO_NUM; j++)
    servo[i][j].write(CLAW_CLOSE_ANGLE[i][j]);
}

void LED_ON()
{
  digitalWrite(led, HIGH);
}

void LED_OFF()
{
  digitalWrite(led, LOW);
}

void LED_TOGGLE()
{
  digitalWrite(led, !digitalRead(led));
}

void process(uint8_t cmd)
{
  for(int i = 0; i < CLAW_NUM; i++)
  {
    if(cmd == CLAW_OPEN_CMD[i])
    {
      OpenClaw(i);
      LED_ON();
      Serial.write(cmd);
    }
    if(cmd == CLAW_CLOSE_CMD[i])
    {
      CloseClaw(i);
      LED_OFF();
      Serial.write(cmd);
    }
  }
}

void SetupServo()
{
  for(int i = 0; i < CLAW_NUM; i++)
  {
    for(int j = 0; j < SERVO_NUM; j++)
    {
      servo[i][j].attach(pin[i][j]);
      OpenClaw(i);
    }
  }
}

void SetupLed()
{
  pinMode(led, OUTPUT);
}

void setup()
{
  Serial.begin(115200);
  SetupServo();
  SetupLed();
  Serial.println("setup done!\n");
}

void loop()
{
  if(Serial.available())
  {
    uint8_t cmd = Serial.read();
    process(cmd);
  }
}


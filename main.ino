#include <AccelStepper.h>
#include <MultiStepper.h>
//#include "probeLine.ino"
int en = 8;
// The X Stepper pins
#define STEPPER1_DIR_PIN 5
#define STEPPER1_STEP_PIN 2
// The Y stepper pins
#define STEPPER2_DIR_PIN 6
#define STEPPER2_STEP_PIN 3

bool check = false;
AccelStepper stepper1(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);

MultiStepper steppers;
int priPosition0 = 0;
int priPosition1 = 0;
// each 50 steps
void runn(int x, int y)
{

    long positions[2]; // Array of desired stepper positions
    positions[0] = priPosition0 + x;
    positions[1] = priPosition1 + y;
    steppers.moveTo(positions);
    steppers.runSpeedToPosition();
    priPosition0 = positions[0];
    priPosition1 = positions[1];
}
void up()
{
    runn(50, 50);
}
void back()
{
    runn(-50, -50);
}
const float R = 141.42; // R là khoảng cách tâm trục tới lốp // số hiện tại là đường kính
const float r = 69.18;  // r là bán kính lốp
void rotate(int degree)
{
    int steps = (int)degree * 5 * R / (9 * r);
    runn(steps, -steps);
}
void setup()
{
    pinMode(en, OUTPUT);                // Enable pin - chân khởi động - nối vào GND sẽ giúp ta bật động cơ bước, nối vô VCC động cơ bước được thả ra. Nôm na: GND = servo.attach, VCC = servo.detach
    pinMode(STEPPER1_STEP_PIN, OUTPUT); // Step pin
    pinMode(STEPPER1_DIR_PIN, OUTPUT);  // Dir - pin
    pinMode(STEPPER2_STEP_PIN, OUTPUT); // Step pin
    pinMode(STEPPER2_DIR_PIN, OUTPUT);  // Dir - pi
    digitalWrite(en, LOW);              // Set Enable low
    stepper1.setMaxSpeed(200);
    //    stepper1.setAcceleration(10);
    stepper2.setMaxSpeed(200);
    //    stepper2.setAcceleration(10);
    steppers.addStepper(stepper1);
    steppers.addStepper(stepper2);
    //    rotate(180);
}

void loop()
{
    up();
    Serial.println(priPosition0);
    Serial.println(priPosition1);
    //    int degreeDif = getDif();
    // đang chạy mà cua được ko ???
}
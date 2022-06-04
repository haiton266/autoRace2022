// chèn sau
#include <Servo.h>
Servo myservo;

#define STEP_Y 3
#define STEP_X 2
#define ENB 8
#define DIR_X 5
#define DIR_Y 6

const int trig = 4;
const int echo = 7;
int servoPin = 9;

float arr[13] = {0, 45, 73, 45, 23, 73, 23, 23, 25, 23, 23, 23, 50}; // lưu khoảng cách từng đoạn
int dir[13] = {1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1};                  // 13 đoạn thì cua 12 lần
int lengthArr = 13, steps;
float distance;
const float R = 7.071; // R là khoảng cách tâm trục tới lốp (cm)
const float r = 3.459; // r là bán kính lốp (cm)
const float R = 7.125; // R là khoảng cách tâm trục tới lốp (cm)
const float r = 3.44;  // r là bán kính lốp (cm)
const float pi = 3.14159265359;

void setup()
{
    maxSpeed = 100;
    speedline = 70;
    pinMode(STEP_Y, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(STEP_X, OUTPUT);
    // pinMode(ENB_y, OUTPUT);
    pinMode(DIR_X, OUTPUT);
    pinMode(DIR_Y, OUTPUT);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    interval = 800;
    digitalWrite(DIR_Y, HIGH);
    digitalWrite(DIR_X, LOW);
    digitalWrite(ENB, LOW);
    Serial.begin(115200);
    I = 0;              // biến trong hàm PID
    previous_error = 0; // biến trong hàm PID
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    myservo.attach(servoPin);
    maze();
}

void loop()
{
    ;
}
void Clock_x()
{
    digitalWrite(STEP_X, HIGH);
    delayMicroseconds(1);
    digitalWrite(STEP_X, LOW);
    delayMicroseconds(1);
}
// Ham dieu khien step 2
void Clock_y()
{
    digitalWrite(STEP_Y, HIGH);
    delayMicroseconds(1);
    digitalWrite(STEP_Y, LOW);
    delayMicroseconds(1);
}

float calSteps(float distance) // tính số bước
{
    // r * 2pi            1600 steps
    // distance              ???
    return (distance * 1600) / (r * 2 * pi);
}

void goStraight(float distance) // đi thẳng
{
    digitalWrite(DIR_Y, HIGH);
    digitalWrite(DIR_X, HIGH);
    steps = calSteps(distance);
    for (int i = 1; i <= steps; i++)
    {
        Clock_x();
        Clock_y();
        delayMicroseconds(1100);
    }
}

void turnRight(float deg) // phải
{
    digitalWrite(DIR_Y, HIGH);
    digitalWrite(DIR_X, LOW);
    distance = R * deg;
    steps = calSteps(distance) + 10;
    for (int i = 1; i <= steps; i++)
    {
        Clock_x();
        Clock_y();
        delayMicroseconds(1100);
    }
}
void turnLeft(float deg) // trái
{
    digitalWrite(DIR_Y, LOW);
    digitalWrite(DIR_X, HIGH);
    distance = R * deg;
    steps = calSteps(distance) + 10;
    Serial.println(steps);
    for (int i = 1; i <= steps; i++)
    {
        Clock_x();
        Clock_y();
        delayMicroseconds(1100);
    }
}

// setup
// pinMode(trig, OUTPUT);
// pinMode(echo, INPUT);
// myservo.attach(servoPin);

float getDistance(int deg)
{
    myservo.write(deg);
    while (myservo.read() != deg)
        ;
    unsigned long duration;
    float distance;
    digitalWrite(trig, 0);
    delayMicroseconds(2);
    digitalWrite(trig, 1);
    delayMicroseconds(5);
    digitalWrite(trig, 0);
    duration = pulseIn(echo, HIGH);
    distance = float(duration / 2 / 29.412); // trả về cm
    return distance;
}
void stabilize()
{
    distance = getDistance(90);          // trả về khoảng cách so với tường trước mặt // Sỹ viết chưa thêm vô
    float deg = acos(arr[0] / distance); // tính góc lệch = arcCos(kề / huyền)

    if (previous_error < 0) // trước đó cua trái thì chừ cua phải
        turnRight(deg);
    else
        turnLeft(deg);
}
void maze() // hàm gọi chạy mê cung
{
    // cho chạy vô mê cung tầm 10cm rồi tính
    // stabilize(); // ổn định vị trí
    for (int i = 0; i < lengthArr; i++)
    {
        goStraight(arr[i]); // Di chuyển thẳng ở đây
        delay(200);
        if (dir[i] == 1) // phải
            turnRight(pi / 2);
        else if (dir[i] == 0)
            turnLeft(pi / 2);
        delay(200);
    }
}
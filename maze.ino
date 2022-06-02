// chèn sau
#include <Servo.h>
Servo myservo;

const int trig = 4;
const int echo = 7;
int servoPin = 9;

float arr[13] = {0, 45, 70, 45, 25, 75, 25, 25, 25, 25, 25, 50}; // lưu khoảng cách từng đoạn
int dir[13] = {1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1};              // 13 đoạn thì cua 12 lần
int lengthArr = 13, steps, distance;
const float R = 7.071; // R là khoảng cách tâm trục tới lốp (cm)
const float r = 3.459; // r là bán kính lốp (cm)
const float R = 7.125; // R là khoảng cách tâm trục tới lốp (cm)
const float r = 3.44;  // r là bán kính lốp (cm)
const float pi = 3.14159265359;

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
        delayMicroseconds(500);
    }
}

void turnRight(int deg) // phải
{
    digitalWrite(DIR_Y, HIGH);
    digitalWrite(DIR_X, LOW);
    distance = R * deg;
    steps = calSteps(distance);
    for (int i = 1; i <= steps; i++)
    {
        Clock_x();
        Clock_y();
        delayMicroseconds(500);
    }
}
void turnLeft(int deg) // trái
{
    digitalWrite(DIR_Y, LOW);
    digitalWrite(DIR_X, HIGH);
    distance = R * deg;
    steps = calSteps(distance);
    for (int i = 1; i <= steps; i++)
    {
        Clock_x();
        Clock_y();
        delayMicroseconds(500);
    }
}

// setup
pinMode(trig, OUTPUT);
pinMode(echo, INPUT);
myservo.attach(servoPin);

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
    distance = float(duration * 10 / 2 / 2.9412); // trả về mm
    return distance;
}
void stabilize()
{
    distance = getDistance(90);          // trả về khoảng cách so với tường trước mặt // Sỹ viết chưa thêm vô
    float deg = acos(arr[0] / distance); // tính góc lệch = arcCos(kề / huyền)

    if (previos_error < 0) // trước đó cua trái thì chừ cua phải
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
        if (dir[i] == 1)    // phải
            turnRight(pi / 2);
        else if (dir[i] == 0)
            turnLeft(pi / 2);
    }
}
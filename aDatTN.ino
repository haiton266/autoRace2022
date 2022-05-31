#include <TimerOne.h>

#define STEP_Y 3
#define STEP_X 2
#define ENB 8
#define DIR_X 5
#define DIR_Y 6
#define white 400
#define black 500

float speed_x, speed_y;
unsigned long prvtime_x, prvtime_y, current, interval, stepInterval_x, stepInterval_y;
int sensorPin = A0;
int sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, maxSpeed;
int speed_x_max_now, speed_y_max_now;
int l[6], b[6];

void Clock_x();  // One step driver x
void Clock_y();  // One step driver y
void Driver_x(); // speed driver x
void Driver_y(); // speed driver y
void read_sensor();
void remote();
int v_x = 0;
int v_y = 0;
int a_x, a_y; // gia toc
int tick_time;
int error;
int speedline;
int Kp, Ki, Kd;
int P, I, D, previous_error, PID_value;

// y : left
// x : right
void setup()
{
  // speed_x=300;
  // speed_y=200;
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
  // digitalWrite(ENB_y, LOW);
  // #define DIR_X 5
  //#define DIR_Y 6
  Serial.begin(115200);
  speed_x = 0;
  speed_y = 0;
  I = 0;              // biến trong hàm PID
  previous_error = 0; // biến trong hàm PID
  Timer1.initialize(100);
  Timer1.attachInterrupt(Driver_x);
}

////////////////////////////////////////////////////////Main//////////////////////////////////////////////////////
void loop()
{
  remote();
}
void remote()
{
  error = getError();            // lấy lỗi sai
  PID_value = computePID(error); // từ lỗi tính PID
  speed_x -= PID_value;          // x là bánh phải, thì bánh phải giảm tốc để cua phải (PID_value > 0)
  speed_y += PID_value;
}

//  Ham Dieu khien  step 1
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

void Driver_x()
{

  // speed_x =50;
  current = micros();
  tick_time++;
  if (tick_time > 800)
  {
    tick_time = 0;
    // speed x/////////////////////
    // kiem tra van toc speed
    speed_x_max_now = interval - (interval * (0.01 * speed_x));

    if (speed_x_max_now > stepInterval_x)
    { // neu True toc do giam.

      if (a_x < 25)
      {
        a_x = 0;
        speed_x = 0;
      }
      else
      {
        a_x = a_x - 20;
      }
    }

    else
    { // toc do tang
      //{ // neu True toc do giam.

      if (interval - (interval * (0.01 * speed_x)) >= stepInterval_x)
      {

        // no
      }
      else
      {
        if (700 < stepInterval_x <= 800) // 1-30% step
        {
          a_x = a_x + 10;
        }
        else if (500 < stepInterval_x <= 700) // 30%-50%
        {
          a_x = a_x + 5;
        }
        else if (300 < stepInterval_x < 500) // 50%-70%
        {
          a_x = a_x + 3;
        }
        else if (0 < stepInterval_x < 300) // 70%-80%
        {
          a_x = a_x + 1;
        }
      }
      //      if (stepInterval_x >= (interval * (maxSpeed * 0.01))) // speed max
      //      {
      //        a_x = (interval * (maxSpeed * 0.01));
      //      }
    }
    /////////////////////////////
    speed_y_max_now = interval - (interval * (0.01 * speed_y));

    if (speed_y_max_now > stepInterval_y)
    { // neu True toc do giam.

      if (a_y < 25)
      {
        a_y = 0;
        speed_y = 0;
      }
      else
      {
        a_y = a_y - 20;
      }
    }
    else
    {

      if (interval - (interval * (0.01 * speed_y)) >= stepInterval_y)
      {
        // no
      }
      else
      {
        if (700 < stepInterval_y <= 800) // 1-30% step
        {
          a_y = a_y + 20;
        }
        else if (500 < stepInterval_y <= 700) // 30%-50%
        {
          a_y = a_y + 10;
        }
        else if (300 < stepInterval_y < 500) // 50%-70%
        {
          a_y = a_y + 5;
        }
        else if (0 < stepInterval_y < 300) // 70%-80%
        {
          a_y = a_y + 1;
        }
      }
      //      if (stepInterval_y >= (interval * (maxSpeed * 0.01))) // speed max
      //      {
      //        a_y =  interval * (maxSpeed * 0.01);
      //      }
    }
    //////////////////////////////////////////////////////////////////////////
  }
  // speed x/////////////////////
  stepInterval_x = (interval - a_x); // khoang time giua cac step

  if (current - prvtime_x > (stepInterval_x))
  {
    prvtime_x = current;
    if (speed_x == 0)
    {
      a_x = 0;
    }
    else
    {
      Clock_x();
    }
  }
  stepInterval_y = (interval - a_y); // khoang time giua cac step
  if (current - prvtime_y > (stepInterval_y))
  {
    prvtime_y = current;
    if (speed_y == 0)
    {
      a_y = 0;
    }
    else
    {
      Clock_y();
    }
  }
}
int getError()
{
  l[1] = analogRead(1);
  l[2] = analogRead(2);
  l[3] = analogRead(3);
  l[4] = analogRead(4);
  l[5] = analogRead(5);
  l[6] = analogRead(0);

  if (l[1] < 555)
    b[1] = 1;
  else
    b[1] = 0;
  if (l[2] < 505)
    b[2] = 1;
  else
    b[2] = 0;
  if (l[3] < 500)
    b[3] = 1;
  else
    b[3] = 0;
  if (l[4] < 380)
    b[4] = 1;
  else
    b[4] = 0;
  if (l[5] < 485)
    b[5] = 1;
  else
    b[5] = 0;
  if (l[6] < 500)
    b[6] = 1;
  else
    b[6] = 0;

  // nhận 4 cảm biến liên tiếp (nhiễu)
  if (b[3] == 1 && b[6] == 1) // 001111
    return 2;
  else if (b[2] == 1 && b[5] == 1) // 011110
    return 0;
  else if (b[1] == 1 && b[4] == 1) // 111100
    return -2;

  // nhận 3 cảm biến liên tiếp (nhiễu)
  else if (b[3] == 1 && b[5] == 1) // 001110
    return 1;
  else if (b[2] == 1 && b[4] == 1) // 011100
    return -1;
  else if (b[4] == 1 && b[6] == 1) // 000111
    return 3;
  else if (b[1] == 1 && b[3] == 1) // 111000
    return -3;

  // nhận 2 cảm biến liên tiếp
  else if (b[3] == 1 && b[4] == 1)
    return 0;
  else if (b[4] == 1 && b[5] == 1) // 000110
    return 2;
  else if (b[2] == 1 && b[3] == 1) // 011000
    return -2;
  else if (b[5] == 1 && b[6] == 1) // 000011
    return 4;
  else if (b[1] == 1 && b[2] == 1) // 110000
    return -4;

  // nhận 1 cảm biến
  else if (b[4] == 1) // 000100
    return 1;
  else if (b[3] == 1) // 001000
    return -1;
  else if (b[5] == 1) // 000010
    return 3;
  else if (b[2] == 1) // 010000
    return -3;
  else if (b[6] == 1) // 000001
    return 5;
  else if (b[1] == 1) // 100000
    return -5;

  // thêm trường hợp 000000 thì mình sẽ chạy lui line cũ để dò tiếp
  else
    return 0; // Hiện tại cho chạy thẳng ở vạch đứt đoạn
}
int computePID(int err)
{
  Kp = 20;
  Ki = 0;
  Kd = 0;

  P = err;                  // Proportional: tỉ lệ, lệch line hiện tại mình truyền vào
  I += err;                 // Integral: tích phân
  D = err - previous_error; // Differential : vi phân (trong khoảng thời gian đenta_t = t - t0)

  int value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_error = err;
  return value;
}

int en = 8;
// The X Stepper pins
#define STEPPER0_DIR_PIN 5
#define STEPPER0_STEP_PIN 2
// The Y stepper pins
#define STEPPER1_DIR_PIN 6
#define STEPPER1_STEP_PIN 3

const float R = 141.42; // R là khoảng cách tâm trục tới lốp // số hiện tại là đường kính
const float r = 69.18;  // r là bán kính lốp
const int Kp = 5, Ki = 2, Kd = 2;

int P, I = 0, D, previous_error = 0;

long int cycle[2], lastStep[2] = {0, 0}, accel[2], v0[2], ve[2], lastTime[2] = {0, 0}, sum[2];
long int mySpeed, curTime, vMax, lasttimecr1;
int STEPPER_STEP_PIN[2] = {2, 3};

int temp, temp2, error, PIDValue;
void setup()
{
    Serial.begin(250000);
    pinMode(en, OUTPUT);                // Enable pin - chân khởi động - nối vào GND sẽ giúp ta bật động cơ bước, nối vô VCC động cơ bước được thả ra. Nôm na: GND = servo.attach, VCC = servo.detach
    pinMode(STEPPER0_STEP_PIN, OUTPUT); // Step pin
    pinMode(STEPPER0_DIR_PIN, OUTPUT);  // Dir - pin
    pinMode(STEPPER1_STEP_PIN, OUTPUT); // Step pin
    pinMode(STEPPER1_DIR_PIN, OUTPUT);  // Dir - pi
    digitalWrite(en, LOW);              // Set Enable low
    error = 0;
}
void loop()
{
    // 0 bánh phải
    // 1 bánh trái

    // Dò line
    error = getError(); // lấy giá trị lệch từ hàm trong file "probeLine.ino"
    PIDValue = computePID(error);
    remote0(PIDValue);
    remote1(-PIDValue);
}

void remote0(int accel) // truyền vào gia tốc
{
    curTime = micros();

    if (curTime - lastTime[0] > 25000) // cứ 25mS thì tăng vận tốc 1 lần -> 1S set 40 lần
    {
        lastTime[0] = curTime;
        sum[0] += accel; // vận tốc mới = cũ  + gia tốc
    }

    sum[0] = constrain(sum[0], 0, 1950); // lấy vận tốc từ [0,1950]

    if (curTime - lastStep[0] > (2000 - sum[0])) // chu kỳ càng ngắn thì tốc độ càng cao ( thời gian chạy 1 bước ), tối thiếu = 50
    {
        lastStep[0] = curTime;
        setXung0();
    }
}

void remote1(int accel)
{
    curTime = micros();

    if (curTime - lastTime[1] > 25000)
    {
        lastTime[1] = curTime;
        sum[1] += accel;
    }

    sum[1] = constrain(sum[1], 0, 1950);

    if (curTime - lastStep[1] > (2000 - sum[1]))
    {
        lastStep[1] = curTime;
        setXung1();
    }
}
void setXung0()
{
    digitalWrite(STEPPER0_STEP_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(STEPPER0_STEP_PIN, LOW);
    delayMicroseconds(2);
}
void setXung1()
{
    digitalWrite(STEPPER1_STEP_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(STEPPER1_STEP_PIN, LOW);
    delayMicroseconds(2);
}
int getError()
{
    int l[6], b[6];
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

    // Đọc file Giá trị sensor.docx để hiểu rõ
    if (b[3] == 1 && b[4] == 1)
        return 0;
    // phòng trường hợp nhiễu, nhận 3 cảm biến 1 cùng lúc -> xử lý coi như nhận 1 con cảm biến ở giữa
    else if (b[3] == 1 && b[5] == 1) // 001110
        return 1;
    else if (b[2] == 1 && b[4] == 1) // 011100
        return -1;
    else if (b[4] == 1 && b[6] == 1) // 000111
        return 3;
    else if (b[1] == 1 && b[3] == 1) // 111000
        return -3;
    // nhận 2 cảm biến
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
}
int computePID(int error)
{
    P = error;                  // Proportional: tỉ lệ, lệch line hiện tại mình truyền vào
    I += error;                 // Integral: tích phân
    D = error - previous_error; // Differential : vi phân (trong khoảng thời gian đenta_t = t - t0)

    int PID_value = (Kp * P) + (Ki * I) + (Kd * D);

    previous_error = error;
    return PID_value;
}

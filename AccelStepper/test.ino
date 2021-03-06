int en = 8;
// The X Stepper pins
#define STEPPER0_DIR_PIN 5
#define STEPPER0_STEP_PIN 2
// The Y stepper pins
#define STEPPER1_DIR_PIN 6
#define STEPPER1_STEP_PIN 3

const float R = 141.42; // R là khoảng cách tâm trục tới lốp // số hiện tại là đường kính
const float r = 69.18;  // r là bán kính lốp

long int cycle[2], lastStep[2] = {0, 0}, accel[2], v0[2], ve[2], lastTime[2] = {0, 0}, sum[2];
long int mySpeed, curTime, vMax, lasttimecr1;
int STEPPER_STEP_PIN[2] = {2, 3};

int temp, temp2;
void setup()
{
    Serial.begin(250000);
    pinMode(en, OUTPUT);                // Enable pin - chân khởi động - nối vào GND sẽ giúp ta bật động cơ bước, nối vô VCC động cơ bước được thả ra. Nôm na: GND = servo.attach, VCC = servo.detach
    pinMode(STEPPER0_STEP_PIN, OUTPUT); // Step pin
    pinMode(STEPPER0_DIR_PIN, OUTPUT);  // Dir - pin
    pinMode(STEPPER1_STEP_PIN, OUTPUT); // Step pin
    pinMode(STEPPER1_DIR_PIN, OUTPUT);  // Dir - pi
    digitalWrite(en, LOW);              // Set Enable low
}
void loop()
{
    // 0 bánh phải
    // 1 bánh trái
    switch (lech)
    {
    case 0: // đang ổn định thì cứ tăng 2 bánh lên
        remote0(5);
        remote1(5);
        break;
    case 1: // 000100 hoặc 001110(nhiễu) lệch phải -> giảm bánh phải chậm xuống // PID tính
        remote0(-2);
        remote1(0);
        break;
    case -1: // 001000 hoặc 011100(nhiễu) lệch trái -> giảm bánh trái
        remote0(0);
        remote1(-2);
        break;
    case 2: // 001000 lệch phải -> giảm bánh phải
        remote0(-3);
        remote1(0);
        break;
    case -2: // 001000 hoặc 001110(nhiễu) lệch trái -> giảm bánh trái
        remote0(-3);
        remote1(0);
        break;
    default:
        break;
    }
}

void remote0(int accel)
{
    curTime = micros();
    if (sum[0] > 1950)
        sum[0] = 1950;

    if (curTime - lastTime[0] > 25000)
    {
        lastTime[0] = curTime;
        sum[0] += accel;
    }
    if (curTime - lastStep[0] > (2000 - sum[0]))
    {
        lastStep[0] = curTime;
        setXung0();
    }
}
void remote1(int accel) // nên truyền vào gia tốc
{
    curTime = micros();
    if (sum[1] > 1950)
        sum[1] = 1950;

    if (curTime - lastTime[1] > 25000) // cứ 25mS thì set 1 lần -> 1S set 40 lần
    {
        lastTime[1] = curTime;
        sum[1] += accel;
    }
    if (curTime - lastStep[1] > (2000 - sum[1])) // chu kỳ càng ngắn thì tốc độ càng cao ( thời gian chạy 1 bước ), tối thiếu =50
    {                                            // Nếu coi như coi sum là vận tốc, thì để điều chỉnh vận tốc, ta chỉ cần ???
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
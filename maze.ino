float arr[13] = {};
// 1 phải, 0 trái, 2 ra khỏi mê cung
int dir[13] = {1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1}; // 13 đoạn thì cua 12 lần
int lengthArr = 13, steps, distance;
const float R = 7.125; // R là khoảng cách tâm trục tới lốp (cm)
const float r = 3.44;  // r là bán kính lốp (cm)
const float pi = 3.14159265359;

float calSteps(float distance)
{
    // r * 2pi            1600 steps
    // distance              ???
    return (distance * 1600) / (r * 2 * pi);
}
void goStraight(float distance)
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

void turnRight()
{
    digitalWrite(DIR_Y, HIGH);
    digitalWrite(DIR_X, LOW);
    steps = 828; // tính sẵn được
    for (int i = 1; i <= steps; i++)
    {
        Clock_x();
        Clock_y();
        delayMicroseconds(500);
    }
}
void turnLeft()
{
    digitalWrite(DIR_Y, LOW);
    digitalWrite(DIR_X, HIGH);
    steps = 828; // tính sẵn được
    for (int i = 1; i <= steps; i++)
    {
        Clock_x();
        Clock_y();
        delayMicroseconds(500);
    }
}

void maze()
{
    ;
    for (int i = 0; i < lengthArr; i++)
    {
        goStraight(arr[i]); // Di chuyển thẳng ở đây
        if (dir[i] == 1)    // phải
            turnRight();
        else if (dir[i] == 0)
            turnLeft();
    }
}
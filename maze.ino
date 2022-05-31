float arr[13] = {};
// 1 phải, 0 trái, 2 ra khỏi mê cung
int dir[13] = {1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1}; // 13 đoạn thì cua 12 lần
int lengthArr = 13, steps, distance;
const float R = 7.071; // R là khoảng cách tâm trục tới lốp (cm)
const float r = 3.459; // r là bán kính lốp (cm)
const float pi = 3.14159265359;

float calSteps(float distance)
{
    // r * 2pi            3200 steps
    // distance             ???
    return (distance * 3200) / (r * 2 * pi);
}
void goStraight(float distance)
{
    steps = calSteps(distance);
    //
}

void rotate(int interval)
{
    for (int)
}

void turnRight()
{
    steps = 1636; // tính sẵn được
}
void turnLeft()
{
    steps = 1636; // tính sẵn được
}

void maze()
{
    for (int i = 0; i < lengthArr; i++)
    {
        goStraight(arr[i]); // Di chuyển thẳng ở đây
        if (dir[i] == 1)    // phải
            turnRight();
        else if (dir[i] == 0)
            turnLeft();
    }
}
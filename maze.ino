float arr[13] = {};
// 1 phải, 0 trái, 2 ra khỏi mê cung
int dir[13] = {1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1}; // 13 đoạn thì cua 12 lần
int lengthArr = 13, steps, distance;
const float R = 7.071; // R là khoảng cách tâm trục tới lốp (cm) // số hiện tại là đường kính
const float r = 3.459; // r là bán kính lốp (mm)
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
void turnRight()
{
    distance = R * pi / 2;
    steps = calSteps(distance); // tính sẵn được
}
void turnLeft()
{
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
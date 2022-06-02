int e[64], previous_error;
void init()
{
    e[4] = 1;
    e[2] = 2;
    e[1] = 3;
    e[6] = 4;
    e[3] = 5;
    e[33] = 6;
    e[7] = 7;
    e[14] = 8;
    e[0] = 9;
    e[30] = 10;
    e[12] = 0;
    e[8] = -1;
    e[16] = -2;
    e[32] = -3;
    e[24] = -4;
    e[48] = -5;
    e[56] = -7;
    e[28] = -8;
    e[63] = -9;
}
int findError(int s1, int s2, int s3, int s4, int s5, int s6)
{
    int val = s6 + 2 * s5 + 4 * s4 + 8 * s3 + 16 * s2 + 32 * s1; // chuyển nhị phân
    int res = e[val];
    if (val == 30 || val == 33) // trường hợp 011110 và 100001
        if (previous_error > 0)
            res = -res;
    previous_error = res;
    return res;
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
    error = findError(b[1], b[2], b[3], b[4], b[5], b[6]);
}

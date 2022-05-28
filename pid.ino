const int Kp = 5, Ki = 2, Kd = 2;

int P, I, D, previous_error = 0;
int computePID(int error)
{
    P = error;                  // Proportional: tỉ lệ, lệch line hiện tại mình truyền vào
    I += error;                 // Integral: tích phân
    D = error - previous_error; // Differential : vi phân (trong khoảng thời gian đenta_t = t - t0)

    int PID_value = (Kp * P) + (Ki * I) + (Kd * D);

    previous_error = error;
    return PID_value;
}
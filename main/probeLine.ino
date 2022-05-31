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

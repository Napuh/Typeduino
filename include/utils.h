//   **************************************************************
//   || The following functions are extracted from the           ||
//   || open source repository www.github.com/seytonic/malduino  ||
//   || and fairly used according to the MIT license             ||
//   **************************************************************

#include <Arduino.h>

const int buffersize = 256;
char *buf = (char*) malloc(sizeof(char) * buffersize);

int rMin = -100;
int rMax = 100;

int getSpace(int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if (buf[i] == ' ')
            return i;
    }
    return -1;
}

bool equals(char *strA, int start, int end, char *strB, int strLen)
{
    if (end - start != strLen)
        return false;
    for (int i = 0; i < strLen; i++)
    {
        if (strA[start + i] != strB[i])
            return false;
    }
    return true;
}

int toPositive(int num)
{
    if (num < 0)
        return num * (-1);
    else
        return num;
}

bool equalsBuffer(int start, int end, char *str)
{
    return equals(buf, start, end, str, String(str).length());
}

int getInt(char *str, int pos)
{
    if (equals(str, pos + 1, pos + 7, "RANDOM", 6))
    {
        return random(rMin, rMax);
    }
    else
    {
        return String(str).substring(pos + 1, pos + 6).toInt();
    }
}


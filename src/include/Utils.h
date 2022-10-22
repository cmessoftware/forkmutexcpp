#ifndef UTILS_H
#define UTILS_H

class Utils
{
private:
    /* data */
public:
    Utils(/* args */);
    ~Utils();
    void  Swap(char *x, char *y);
    char* Reverse(char *buffer, int i, int j);
    char* Itoa(int value, char* buffer, int base);
    void  ReverseFtoa(char* str, int len);
    int   IntToStr(int x, char str[], int d);
    void  Ftoa(float n, char* res, int afterpoint);
};

#endif

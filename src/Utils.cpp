#include "include/Utils.h"
#include <math.h>
#include <iostream>


Utils::Utils()
{}

// Función para intercambiar dos números
void Utils::Swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Función para invertir `buffer[i…j]`
char* Utils::Reverse(char *buffer, int i, int j)
{
    while (i < j) {
        Swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}

// Función iterativa para implementar la función `itoa()` en C
char* Utils::Itoa(int value, char* buffer, int base)
{
    // entrada inválida
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // considera el valor absoluto del número
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // si el numero es 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // Si la base es 10 y el valor es negativo, la string resultante
    // va precedido de un signo menos (-)
    // Con cualquier otra base, el valor siempre se considera sin firmar
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // string de terminación nula
 
    // invertir la string y devolverla
    return Reverse(buffer, 0, i - 1);
}


// Reverses a string 'str' of length 'len'
void Utils::ReverseFtoa(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int Utils::IntToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    ReverseFtoa(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating-point/double number to a string.
void Utils::Ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = IntToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        IntToStr((int)fpart, res + i + 1, afterpoint);
    }
}

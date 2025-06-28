#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <Arduino.h>

class StringUtils
{
private:
    static StringUtils *instance; // Pointer to hold the current instance of the class

public:
    StringUtils();

    // Static member functions
    static String pad5(int v);
};

#endif

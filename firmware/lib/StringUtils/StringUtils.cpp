#include "StringUtils.h"

// Define the static instance pointer
StringUtils *StringUtils::instance = nullptr;

StringUtils::StringUtils()
{
    instance = this; // Assign the instance pointer
}

String StringUtils::pad5(int v)
{
    String s = String(v);
    while (s.length() < 5)
    {
        s = " " + s;
    }
    return s;
}

#include "common/common.h"

extern "C" {

char* itoa(uint32_t value, char * str, int base) {
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    //if ( value < 0 && base == 10 )
    //{
    //    *ptr++ = '-';
    //}
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

bool isKiBAligned(uint32_t num, uint32_t kibValue) {
    return num % kibValue == 0; // num % kibValue is only 0 if num is aligned to kibValue
}

}
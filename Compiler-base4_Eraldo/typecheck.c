#include <macros.h>
/*
 * 0 -> void
 * 1 -> integer
 * 2 -> real
 * 3 -> double
 * 4 -> char
 * 5 -> text
 * 6 -> boolean
 */

int
typematch(int type1, int type2)
{
    if (type1 == 0 || type2 == 0)
        return max(type1, type2);
    if (type1 == 6) {
        if (type2 == 6)
            return 6;
        return -1;
    }
    if (type2 == 6)
        return -2;
    /*
     * must be 1, 2, 3, 4, 5 
     */
    switch (type1) {
    case 1:
    case 2:
    case 3:
        if (type2 > 3)
            return -3;
        return max(type1, type2);
    default:
        if (type2 < 4 || type2 > 5)
            return -4;
        return max(type1, type2);
    }
}

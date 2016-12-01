/*
 * 0 -> void
 * 1 -> integer
 * 2 -> real
 * 3 -> double
 * 4 -> char
 * 5 -> text
 * 6 -> boolean
 */
#define         TYPEINT         1
#define         TYPEFLT         2
#define         TYPEDBL         3
#define         TYPEASC         4
#define         TYPESTR         5
#define         TYPELOG         6

int             typematch(int, int);

#define SMBNFND         -9
#define ILEGCALL        -8

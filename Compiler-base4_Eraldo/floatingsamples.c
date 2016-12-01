#include <stdio.h>
#include <string.h>

float           x = 3.14159;

char            cx[4];

double          y = 3.14159;
char            cy[8];

main()
{
    int             i;
    memcpy(cx, &x, 4);
    memcpy(cy, &y, 8);
    printf("%.6f %.15lf\n", x, y);
    for (i = 0; i < 4; i++) {
        printf("%02x", cx[3 - i]);
    }
    printf("\n%u\n", *((unsigned *) &x));
    putchar(10);
    for (i = 0; i < 8; i++) {
        printf("%02x", cy[7 - i]);
    }
    printf("\n%u\n", ((unsigned *) &y)[0]);
    printf("\n%u\n", ((unsigned *) &y)[1]);
    putchar(10);
    return 0;
}

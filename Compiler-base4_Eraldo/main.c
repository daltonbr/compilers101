/**@<main.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <main.h>

extern FILE    *object;

FILE           *target;

int
main(int argc, char const *argv[])
{
    if (argc > 1) {
        target = fopen(argv[1], "r");
        if (target == NULL) {
            fprintf(stderr, "%s: cannot open target %s... exiting\n",
                    argv[0], argv[1]);
            exit(-1);
        }
    } else {
        fprintf(stderr, "%s: missing file name... exiting\n", argv[0]);
        exit(-2);
    }
    object = stdout;
    start();
    printf("\n");
    return 0;
}

/**@<mypas.c>::**/

/*
1: Tue Aug 16 20:50:47 BRT 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include <mypas.h>

FILE *source, *object;

extern int lookahead; // @ parser.c

int main (int argc, char *argv[], char *envp[])
{
    object = stdout;
/*
    if(!(object = fopen("../object.s","w"))){
        perror("Unable to output object.s");
        return errno;
    }
*/
/*    if (argc == 1) {
        source = stdin;
    } else { */
        //source = fopen (argv[1], "r");
        source = fopen ("../source.pas", "r");  // TODO: remove this hardcoded input file and return the original form
        if (source == NULL) {
            fprintf (stderr, "%s: cannot open %s... exiting\n",
                    argv[0], argv[1]);

            exit (-1);
        }
//    }

    lookahead = gettoken (source);

    mypas ();

    exit (0);
}

#include <stdio.h>
#include <stdlib.h>
#include "search.h"
#include <string>

using namespace std;


int main(int argc, char **argv)
{
        char kostka[]="DRLUUBFBRBLURRLRUBLRDDFDLFUFUFFDBRDUBRUFLLFDDBFLUBLRBD";
    
        char patternized[64];
        char* facelets = (char*)kostka;
        char *sol = solution(
            facelets,
            24,
            1000,
            0,
            "cache"
        );
        if (sol == NULL) {
            puts("Unsolvable cube!");
            return 2;
        }
        puts(sol);
        free(sol);
        return 0;
}


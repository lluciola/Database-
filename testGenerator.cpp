#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include <chrono>

int main (int argc, char* argv[]) {

    assert (argc == 3);

    char prefix = '\0';

    if (strcmp (argv[1], "-I") == 0) prefix = 'A';
    else if (strcmp (argv[1], "-S") == 0) prefix = 'S';
    else return -1;

    unsigned int testSize = atoi (argv[2]);

    FILE* outFile = fopen (testFileName, "wb");
    assert (outFile != NULL);

    srand (std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1));

    char* outStr = (char*) calloc (strLen + 2, sizeof (char));
    assert (outStr != NULL);
    outStr[0] = prefix;
    outStr[1] = ' ';

    for (int i = 0; i < testSize; i++) {

        for (int j = 2; j < strLen + 2; j++) {

            outStr[j] = (unsigned char ) 33 + rand () % (127 - 33);
            while (outStr[j] == '\0' or outStr[j] == ' ' or outStr[j] == '\t') outStr[j] = (unsigned char) 33 + rand () % (127 - 33);
        }

        fprintf (outFile, "%s ", outStr);
        if (prefix == 'A') fprintf (outFile, "%u", hashFunc (outStr));
        fprintf (outFile, "\n");
    }

    fprintf (outFile, "Q\n");

    fclose (outFile);
}
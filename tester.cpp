#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <assert.h>
#include "config.h"

int main () {

    char command[200] = "";

    FILE* dataFileSearch = fopen (graphDataNameSearch, "wb");
    assert (dataFileSearch != NULL);

    FILE* dataFileInsert = fopen (graphDataNameInsert, "wb");
    assert (dataFileInsert != NULL);

    for (int testSize = 100; testSize < maxTestSize; testSize+=100) {

        printf ("testSize : %u\n", testSize, testSize);

        if (GenerateTests) {

            sprintf (command, "./%s -I %d", testGen, testSize);
            system (command);
        }

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now ();

        sprintf (command, "./%s < %s > %s", subjectName, testFileName, outputFileName);
        system (command);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now ();

        unsigned int elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count ();

        fprintf (dataFileInsert, "%d %u\n", testSize, elapsedTime);

        //=====================================================================================

        if (GenerateTests) {

            sprintf (command, "./%s -S %d", testGen, testSize);
            system (command);
        }

        begin = std::chrono::steady_clock::now ();

        sprintf (command, "./%s < %s > %s", subjectName, testFileName, outputFileName);
        system (command);

        end = std::chrono::steady_clock::now ();

        elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count ();

        fprintf (dataFileSearch, "%d %u\n", testSize, elapsedTime);
    }

    fclose (dataFileSearch);
    fclose (dataFileInsert);
}
const char subjectName[] = "hash.exe";
const char testFileName[] = "test.txt";
const char testGen[] = "testGen.exe";
const char graphDataNameSearch[] = "data_search.txt";
const char graphDataNameInsert[] = "data_insert.txt";
const char outputFileName[] = "out.txt";
const int maxTestSize = 100000;
const int strLen = 10;
const bool GenerateTests = true;

const unsigned int hashMult = 37;

unsigned int hashFunc (char* inputStr) {

    unsigned int retVal = 0;
    for (int i = 0; inputStr[i] != '\0' and inputStr[i] != '\n'; i++) {

        retVal += inputStr[i];
        retVal *= hashMult;
    }
    return retVal;
}
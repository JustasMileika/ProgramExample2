/// Author of the program - Justas Mileika 2021
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/// UI messages
#define msgAskingForInputArraySize "Please enter a single integer that will be the size of the array that You will build:\n"
#define msgConfirmingInput "%d entered successfully\n"
#define msgSingleInvalidInput "You have entered an invalid value:\n"
#define msgTooManyValuesSomeIncorrect "You have entered too many values and some of them weren't correct :\n"
#define msgTooManyValuesOneIncorrect "You have entered too many values and one of them wasn't correct :\n"
#define msgReaskingForInput "Please reenter the value:\n"
#define msgTooManyValidNumbers "You have entered a to many numbers, please choose only of them!\n"
#define msgInputLineIsTooLong "The line you have entered is too long, it should be less than %d symbols\n"
#define msgWordContainsUnavailableSymbols "%s contains unavailable symbols\n"
#define msgNumberExceedsBoundariesForArraySize "%s exceeds the mentioned boundaries, the number must be in the interval [%d; %d]\n"
#define msgNumberExceedsBoundariesForElementsOfArray "%s exceeds the mentioned boundaries, the number must be in the interval [%d; %d]\n"
#define msgArrayIsEmpty "The array is empty\n"
#define msgAskingForInputArrayElements "Please enter %d integers to fill the array\n"
#define msgAskingForTheRestOfInput "%d integer(-s) are left to enter\n"
#define msgAnswer "The array after the modifications is:\n"
#define msgPurposeOfTheProgram1 "The program takes in a size of an array N from an interval [%d; %d]\n"
#define msgPurposeOfTheProgram2 "Then N amount of numbers from an interval [%d; %d] are scanned to fill the array\n"
#define msgPurposeOfTheProgram3 "Lastly, the array is modified in such way that the elements equal to minimum and maximum values are removed\n"
#define msgProgramTerminates "The program ends....\n"
#define msgCalculating "Calculating the results.....\n"
#define msgNotEnoughMemory "There is not enough memory on your system to make an array of this size, please reenter a smaller value\n"

/// Boundaries used in the program
#define maxWordAndLineSize 999999
#define minSizeOfArray 0
#define maxSizeOfArray 999999999
#define minValueOfAnElement -999999999
#define maxValueOfAnElement 999999999

/// function that takes a string and returns the number of leadings 0s at the start of the word
int removingLeading0s(char input[]) {
    char characterToScan0s;
    int offset = 0;
    do{
        sscanf(input + offset, "%c", &characterToScan0s);
        ++offset;
    } while(characterToScan0s == '0');
    return offset - 1;
}

/// function that takes in a string and the offset that you wish to start at and returns the amount of white spaces at the start of the word
int removingWhiteSpaces(char inputAsString[], int offsetInLine)
{
    int offset = offsetInLine;
    char temp = 'x';
    do{
        sscanf(inputAsString + offset, "%c", &temp);
        ++offset;
    } while(isspace(temp));
    return offset - 1;
}

/// function that takes in a string and the maximum word size and returns 1 if the word passed is too long and 0 otherwise
int checkingIsWordTooLong(char input[], int maxWordSize) {
    int offset = removingWhiteSpaces(input, 0);
    offset = removingLeading0s(input + offset);
    char *symbols = (char*) calloc(maxWordSize + 1, sizeof(char));
    char checkingForMinus;
    int isNegative = 0;
    sscanf(input + offset, "%c", &checkingForMinus);
    /// seperate case for negative numbers to skip the first character
    if(checkingForMinus == '-')
    {
        isNegative = 1;
    }
    sscanf(input + offset + isNegative, "%9[^\n \t\v\f\r]", symbols);

    char lastChar;
    sscanf(input + offset + strlen(symbols) + isNegative, "%c", &lastChar);
    free(symbols);
    if(!isspace(lastChar))
    {
        return 1;
    }
    return 0;
}

/// function that takes in a string and maximum word size and returns 1 if the word contains incorrect symbols and 0 otherwise
int checkingDoesInputContainUnavailableSymbols(char inputAsString[], int maxWordSize){
    char *wordAnalized = (char*) calloc(maxWordSize + 1, sizeof(char));
    sscanf(inputAsString, "%s", wordAnalized);
    int offset = 0;
    /// seperate case for negative numbers to skip the first symbol
    if(*wordAnalized == '-')
    {
        offset = 1;
    }

    while(offset < strlen(wordAnalized))
    {
        if(*(wordAnalized + offset) < '0' || *(wordAnalized + offset) > '9')
        {
            free(wordAnalized);
            return 1;
        }
        ++offset;
    }
    free(wordAnalized);
    return 0;
}

/// function that takes in a string and returns 1 if it is empty and 0 otherwise
int checkingIsInputEmpty(char inputAsString[])
{
    if (!strlen(inputAsString))
    {
        return 1;
    }
    return 0;
}

/// function that takes in a number, a lower boundary and a higher boundary and returns 1 if the number exceeds it and 0 otherwise
int checkingDoesNumberExceedBoundaries (int number, int lowerBoundary, int higherBoundary)
{
    if(number > higherBoundary || number < lowerBoundary)
    {
        return 1;
    }
    return 0;
}

/// function that takes in the maximum amount of characters to read and a UI message and returns a full scanned line
char* scanningAndValidatingInputFullLine(int charCountToRead, char inputLineIsTooLong[])
{
    char *fullLine = (char*) calloc(charCountToRead + 1, sizeof(char));
    fgets(fullLine, charCountToRead + 1, stdin);
    char *lastChar = (char*) calloc(1, sizeof(char));

    ///base case to end the recursion
    if(*(fullLine + strlen(fullLine) - 1) == '\n')
    {
        return fullLine;
    }
    /// if the line is too long, we print a message to the user, scan the rest of the invalid line and call this function recursively until a valid line is scanned
    printf(inputLineIsTooLong, charCountToRead);
    do{
        fgets(lastChar, 2, stdin);
    } while(*lastChar != '\n');
    free(fullLine);
    free(lastChar);
    return scanningAndValidatingInputFullLine(charCountToRead, inputLineIsTooLong);
}

/// function that takes in a string, the offset you wish to start scanning at and the maximum word size and returns one scanned word with one character after it
char* scanningOneWord(char inputAsString[], int offset, int maxWordSize)
{
    char *word = (char *) calloc(maxWordSize + 1, sizeof(char));
    sscanf(inputAsString + offset, "%s", word);
    char nextChar;
    sscanf(inputAsString + offset + strlen(word), "%c", &nextChar);
    *(word + strlen(word)) = nextChar;
    return word;
}

/// function that takes in a string, the offset you wish to start scanning at and returns one scanned integer
int scanningOneNumber(char input[], int offset) {
    int number = 0;
    sscanf(input + offset, "%d", &number);
    return number;
}

/// function that takes in a string, the max word size, lower and higher boundaries and returns the amount of valid integers in this string
int countingValidNumbersInARow(char input[], int maxWordSize, int lowerBoundary, int higherBoundary) {
    int numberCounter = 0;
    char *array = input;
    int lineLength = strlen(array);
    int offset = removingWhiteSpaces(array, 0);
    while(offset < lineLength)
    {
        char* word = (char *) calloc(maxWordSize + 1, sizeof(char));
        word = scanningOneWord(array, offset, maxWordSize);

        /// modifying the offset to scan the following word
        offset += strlen(word);
        offset = removingWhiteSpaces(array, offset);

        /// checking whether the word scanned is a valid integer and increasing the counter if it is
        if(!checkingIsInputEmpty(word) && !checkingDoesInputContainUnavailableSymbols(word, maxWordSize) && !checkingIsWordTooLong(word, maxWordSize) &&
           !checkingDoesNumberExceedBoundaries(scanningOneNumber(word, 0), lowerBoundary, higherBoundary))
        {
            ++numberCounter;
        }
        free(word);
    }
    return numberCounter;
}

/// function that takes in a string and the maximum word size and returns the amount of words in that string
int countingWordsInARow(char input[], int maxWordSize) {
    int wordCounter = 0;
    char *array = input;
    int lineLength = strlen(array);
    int offset = removingWhiteSpaces(array, 0);
    while(offset < lineLength)
    {
        char* word = (char *)calloc(maxWordSize + 1, sizeof(char));
        word = scanningOneWord(array, offset, maxWordSize);

        /// modifying the offset to scan the following word
        offset += strlen(word);
        offset = removingWhiteSpaces(array, offset);

        ++wordCounter;
        free(word);
    }
    return wordCounter;
}

/// function that takes in a word, maximum word size, minimum and maximum available numbers and UI messages and returns 1 if the word is a valid number and 0 otherwise
int checkingIsWordAValidNumber(char *line, char containsUnavailableSymbols[], char inputNumberIsTooBig[], int maxWordSize, int minNumberAvailable, int maxNumberAvailable) {
    char *word = (char*) calloc(maxWordSize + 1, sizeof(char));
    sscanf(line, "%s", word);

    if(checkingDoesInputContainUnavailableSymbols(line, maxWordSize))
    {
        printf(containsUnavailableSymbols, word);
        free(word);
        return 0;
    }

    if(checkingIsInputEmpty(line))
    {
        free(word);
        return 0;
    }

    if(checkingIsWordTooLong(line, maxWordSize) || checkingDoesNumberExceedBoundaries(scanningOneNumber(line, 0), minNumberAvailable, maxNumberAvailable))
    {
        printf(inputNumberIsTooBig, word + removingLeading0s(word), minNumberAvailable, maxNumberAvailable);
        free(word);
        return 0;
    }
    free(word);
    return 1;
}

/// function that takes in maximum word size, maximum and minimum number available, UI messages and returns scanned and validated array size
int scanningAndValidatingArraySize (char askingUserForInput[], char confirmingInput[], char inputCotainsSingleInvalidInput[], char inputCotainsTooManyValuesSomeIncorrect[],
                                    char inputCotainsTooManyValuesOneIncorrect[], char reaskingForInput[], char inputContainsTooManyNumbers[], int maxLineWordSize,
                                    char inputLineIsTooLong[], int minValueAvailable, int maxValueAvailable, char wordContainsUnavailableSymbols[], char numberExceedsBoundaries[])
        {
            printf(askingUserForInput);
            while(1)
            {
                char *line = (char *) calloc(maxLineWordSize + 1, sizeof(char));
                line = scanningAndValidatingInputFullLine(maxLineWordSize + 1, inputLineIsTooLong);
                int amountOfValidNumbersInARow = countingValidNumbersInARow(line, maxLineWordSize, minValueAvailable, maxValueAvailable);
                int amountOfWordsInARow = countingWordsInARow(line, maxLineWordSize);

                /// if only one word was entered and it is a valid number
                if(amountOfValidNumbersInARow == 1 && amountOfWordsInARow == 1)
                {
                    printf(confirmingInput, scanningOneNumber(line, 0));
                    return scanningOneNumber(line, 0);
                }

                /// if the input line contains incorrect words
                if(amountOfWordsInARow > amountOfValidNumbersInARow)
                {
                    /// printing a message according to the amount of invalid words
                    if(amountOfWordsInARow == 1)
                    {
                        printf(inputCotainsSingleInvalidInput);
                    }
                    else if(amountOfWordsInARow - amountOfValidNumbersInARow > 1)
                    {
                        printf(inputCotainsTooManyValuesSomeIncorrect);
                    }
                    else if(amountOfWordsInARow - amountOfValidNumbersInARow == 1)
                    {
                        printf(inputCotainsTooManyValuesOneIncorrect);
                    }

                    /// printing what is wrong with the invalid words
                    int wordsEnteredCounter = 0;
                    int offsetInLine = removingWhiteSpaces(line, 0);
                    while(wordsEnteredCounter < amountOfWordsInARow)
                    {
                        char *word = (char*) calloc(maxLineWordSize + 1, sizeof(char));
                        word = scanningOneWord(line, offsetInLine, maxLineWordSize);
                        checkingIsWordAValidNumber(word, wordContainsUnavailableSymbols, numberExceedsBoundaries, maxLineWordSize, minValueAvailable, maxValueAvailable);
                        wordsEnteredCounter++;

                        /// modifying the offset to scan the next word
                        offsetInLine += strlen(scanningOneWord(line, offsetInLine, maxLineWordSize)) - 1;
                        offsetInLine = removingWhiteSpaces(line, offsetInLine);

                        free(word);
                    }
                    printf(reaskingForInput);
                }

                /// if the input line is empty, skip this line
                else if(amountOfWordsInARow == 0)
                {
                    free(line);
                    continue;
                }

                /// if all the entered words are valid numbers, but there are too many of them
                else if(amountOfValidNumbersInARow > 1 && amountOfValidNumbersInARow == amountOfWordsInARow)
                {
                    printf(inputContainsTooManyNumbers);
                    printf(reaskingForInput);
                }
                free(line);
            }
        }

/// function that takes in an empty array, the amount of numbers needed to fill the array, the maximum word size,
/// minimum and maximum values and UI messages and fills the array with numbers entered by the user
void buildInputArray (int *array, int amountOfNumbersNeeded, char askingForNAmountOfNumbers[], char askingForTheAmountOfNumbersThatAreLeft[], int maxWordSize, char wordContainsUnavailableSymbols[],
         char numberExceedsBoundaries[], int minNumberAvailable, int maxNumberAvailable, char inputConfirmation[], char inputLineIsTooLong[]) {
    int numbersEntered = 0;
    int arrayBuildingCounter = 0;
    printf(askingForNAmountOfNumbers, amountOfNumbersNeeded);

    /// scanning words until the needed amount of numbers are scanned and stored in the array
    while(numbersEntered < amountOfNumbersNeeded)
    {
        printf(askingForTheAmountOfNumbersThatAreLeft, amountOfNumbersNeeded - numbersEntered);
        char *fullLine = (char *) calloc (maxWordSize + 1, sizeof(char));
        char *word = (char *) calloc (maxWordSize + 1, sizeof(char));
        fullLine = scanningAndValidatingInputFullLine(maxWordSize + 1, inputLineIsTooLong);
        int scannedWordsInLine = 0;
        int offset = removingWhiteSpaces(fullLine, 0);
        int amountOfWordsInARow = countingWordsInARow(fullLine, maxWordSize);

        /// scanning all the words in the array
        while(scannedWordsInLine < amountOfWordsInARow)
        {
            word = scanningOneWord(fullLine, offset, maxWordSize);

            /// if the entered word is a valid number, we increase the number counter and the array building counter and add the number to the array
            if(checkingIsWordAValidNumber(word, wordContainsUnavailableSymbols, numberExceedsBoundaries, maxWordSize, minNumberAvailable, maxNumberAvailable))
            {
                printf(inputConfirmation, scanningOneNumber(fullLine, offset));
                *(array + arrayBuildingCounter) = scanningOneNumber(fullLine, offset);
                ++arrayBuildingCounter;
                ++numbersEntered;
            }

            /// modifying the offset to scan the following word
            offset += strlen(scanningOneWord(fullLine, offset, maxWordSize)) - 1;
            offset = removingWhiteSpaces(fullLine, offset);

            ++scannedWordsInLine;
            free(word);

            /// if we reach the needed amount of numbers, we stop checking the rest of the line
            if(numbersEntered == amountOfNumbersNeeded)
            {
                break;
            }
        }
        free (fullLine);
    }
}

/// function that takes in an array and it's size and returns the minimum value in the array
int findMin(int *array, int arraySize) {
    int minValue = INT_MAX;
    for(int i = 0; i < arraySize; ++i)
    {
        if(*(array + i) < minValue)
        {
            minValue = *(array + i);
        }
    }
    return minValue;
}

/// function that takes in an array and it's size and returns the maximum value in the array
int findMax(int *array, int arraySize) {
    int maxValue = INT_MIN;
    for(int i = 0; i < arraySize; ++i)
    {
        if(*(array + i) > maxValue)
        {
            maxValue = *(array + i);
        }
    }
    return maxValue;
}

/// function that takes in an array and 2 values and returns how many elements are equal to it
int findElementCountToRemove (int *array, int arraySize, int value1, int value2) {
    int amountOfElementsToRemove = 0;
    for(int i = 0; i < arraySize; ++i)
    {
        if(*(array + i) == value2 || *(array+i) == value1)
        {
            ++amountOfElementsToRemove;
        }
    }
    return amountOfElementsToRemove;
}

/// function that takes in an array, it's size, 2 values and builds an array of indexes of elements of the first array that are equal to those 2 values
void buildArrayOfIndexesToRemove (int *array, int *indexes, int arraySize, int value1, int value2) {
    int counterToBuildIndexArray = 0;
    for(int i = 0; i < arraySize; ++i)
    {
        if(*(array+i) == value2 || *(array+i) == value1)
        {
            *(indexes + counterToBuildIndexArray) = i;
            ++counterToBuildIndexArray;
        }
    }
}

/// function that takes in an array, it's size and an index and removes the element with that index
void removalFromArray(int *array, int indexToRemove, int *arraySize) {
    for(int i = indexToRemove; i < *arraySize-1; ++i)
    {
        *(array + i) = *(array + i + 1);
    }

    /// changing array size to one less
    *arraySize = *arraySize - 1;
}

/// function that takes in an array, it's size and modifies it in such way that the elements equal to minimum and maximum values are removed
void modifyingArray (int *array, int *arraySize)
{
    int indexesSize = findElementCountToRemove(array, *arraySize, findMin(array, *arraySize), findMax(array, *arraySize));
    int *indexes = (int*) calloc(indexesSize, sizeof(int));
    buildArrayOfIndexesToRemove(array, indexes, *arraySize, findMin(array, *arraySize), findMax(array, *arraySize));
    for(int i = indexesSize - 1; i >= 0 ; --i)
    {
        removalFromArray(array, indexes[i], &*arraySize);
    }
    free(indexes);
}

/// function that takes in an array, it's size and UI messages and prints out the whole array
void printAnswer(int *array, int arraySize, char arrayIsEmpty[], char answerDeclaration[]) {
    if(arraySize == 0)
    {
        printf(arrayIsEmpty);
    }
    else
    {
        printf(answerDeclaration);
        for(int i = 0; i < arraySize; ++i)
        {
            printf("%d\n", *(array + i));
        }
    }
}

/// function that takes in UI messages and prints them
void printingProgramIntroduction(char whatProgramDoes1[], char whatProgramDoes2[], char whatProgramDoes3[], int maximumNumberAvailableForArray, int minimumNumberAvailableForArray,
                                 int maximumNumberAvailableForElement, int minimumNumberAvailableForElement) {
    printf(whatProgramDoes1,
           minimumNumberAvailableForArray,
           maximumNumberAvailableForArray);
    printf(whatProgramDoes2, minimumNumberAvailableForElement,
           maximumNumberAvailableForElement);
    printf(whatProgramDoes3);
}

/// main function
int main()
{
    printingProgramIntroduction(msgPurposeOfTheProgram1, msgPurposeOfTheProgram2, msgPurposeOfTheProgram3, maxSizeOfArray, minSizeOfArray, maxValueOfAnElement, minValueOfAnElement);
    int inputSize;
    int *array = NULL;

    /// creating an array with the size that the user inputted and printing a warning if there is not enough memory
    do{
    inputSize = scanningAndValidatingArraySize(msgAskingForInputArraySize, msgConfirmingInput, msgSingleInvalidInput, msgTooManyValuesSomeIncorrect, msgTooManyValuesOneIncorrect, msgReaskingForInput, msgTooManyValidNumbers,
                    maxWordAndLineSize, msgInputLineIsTooLong, minSizeOfArray, maxSizeOfArray, msgWordContainsUnavailableSymbols, msgNumberExceedsBoundariesForArraySize);
        array = (int*) calloc(inputSize, sizeof(int));
        if(array == NULL)
        {
            printf(msgNotEnoughMemory);
        }
    } while(array == NULL);

    if(inputSize == 0)
    {
        printf(msgArrayIsEmpty);
    } else {
        buildInputArray(array, inputSize, msgAskingForInputArrayElements, msgAskingForTheRestOfInput, maxWordAndLineSize, msgWordContainsUnavailableSymbols,
                        msgNumberExceedsBoundariesForElementsOfArray, minValueOfAnElement, maxValueOfAnElement, msgConfirmingInput, msgInputLineIsTooLong);
        printf(msgCalculating);
        modifyingArray(array, &inputSize);
        printAnswer(array, inputSize, msgArrayIsEmpty, msgAnswer);
    }

    printf(msgProgramTerminates);
    return 0;
}

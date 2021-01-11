// Arjun Singh
// Readability
// 17/11/2019

// Prompt user for a sentence
// Apply Cauleman-Liau Index
// return (U.S.) grade level needed to understand the text
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Prototypes
bool isPunctuation(char letter);
int CLIndex(int letters, int words, int sentences);

int main(int argc, char *argv[])
{
    // Verify usage
    if (argc != 1)
    {
        fprintf(stdout, "Take no command-line arguments\n");
        return 1;
    }

    string userInput = get_string("Text: ");

    int letters = 0;
    int words = 0;
    int sentences = 0;

    // Count the first word as it can't be checked in loop
    if (isalpha(userInput[0]))
    {
        words++;
    }

    for (int i = 0, n = strlen(userInput); i < n; i++)
    {
        // Detect letters
        if (isalpha(userInput[i]))
        {
            letters++;
        }
        // Detect words
        else if (isspace(userInput[i]))
        {
            words++;
        }
        // Detect sentences
        else if (isPunctuation(userInput[i]))
        {
            sentences++;
        }
    }
    // Store required grade
    int grade = CLIndex(letters, words, sentences);

    // Output required grade
    if (grade >= 16)
    {
        fprintf(stdout, "Grade 16+\n");
    }
    else if (grade < 1)
    {
        fprintf(stdout, "Before Grade 1\n");
    }
    else
    {
        fprintf(stdout, "Grade %i\n", grade);
    }

    return 0;
}

// Check if character is a punctuation
bool isPunctuation(char letter)
{
    if (letter == '.' || letter == '?' || letter == '!')
    {
        return true;
    }

    return false;
}

// Return Coleman-Liau Index
int CLIndex(int letters, int words, int sentences)
{
    float l = (float) letters / (float) words * 100;
    float s = (float) sentences / (float) words * 100;

    return round(0.0588 * l - 0.296 * s - 15.8);
}
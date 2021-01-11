#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes ++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Bubble sort
    for (int i = 0; i < candidate_count - 1; i++)
    {
        bool swap = false;
        for (int j = 0, k = candidate_count - i - 1; j < k; j++)
        {
            if (candidates[j].votes > candidates[j + 1].votes)
            {
                swap = true;
                candidate temp = candidates[j];
                candidates[j] = candidates[j + 1];
                candidates[j + 1] = temp;
            }
        }
        if (!swap)
        {
            break;
        }
    }

    // Initialize winners
    const int maxWinners = 3;
    int *winners = malloc(sizeof(int));
    if (winners == NULL)
    {
        fprintf(stderr, "Can not allocate memory for winners variable\n");
        return;
    }
    *winners = 1;
    int firstWinner = candidate_count - 1;

    // Count winners
    for (int l = 1; l < maxWinners; l++)
    {
        if (candidates[firstWinner].votes == candidates[firstWinner - l].votes)
        {
            *winners += 1;
        }
    }

    // Print winners
    for (int m = 1; m <= *winners; m++)
    {
        fprintf(stdout, "%s\n", candidates[candidate_count - m].name);
    }

    free(winners);

    return;
}
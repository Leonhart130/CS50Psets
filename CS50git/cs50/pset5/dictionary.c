// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;
// Count words in dictionary
unsigned int *words;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Count words in dictionary
    words = malloc(sizeof(unsigned int));
    if (words == NULL)
    {
        return false;
    }
    *words = 0;

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Set traveler node
        node *tmp = root;

        // For each characters
        for (int i = 0; word[i] != '\0'; i++)
        {
            // Retrieve index
            int index = (word[i] == '\'') ? 26 : word[i] % 32 - 1;

            // If children is not allocated yet
            if (tmp->children[index] == NULL)
            {
                // Allocate a new trie
                node *child = malloc(sizeof(node));
                if (child == NULL)
                {
                    return false;
                }
                child->is_word = false;
                for (int j = 0; j < N; j++)
                {
                    child->children[j] = NULL;
                }

                // Link current node to child
                tmp->children[index] = child;
            }
            // Set trav to new location
            tmp = tmp->children[index];
        }
        // Word is reached, turn bool true
        tmp->is_word = true;
        *words += 1;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Copy global counter to free it
    unsigned int copy = *words;
    free(words);
    return copy;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Set traveler node
    node *tmp = root;

    // For each characters
    for (int i = 0; word[i] != '\0'; i++)
    {
        // Retrieve index
        int index = (word[i] == '\'') ? 26 : word[i] % 32 - 1;

        // if child doesn't exist
        if (tmp->children[index] == NULL)
        {
            return false;
        }
        // Set trav to new location
        tmp = tmp->children[index];
    }
    // Return word's boolean
    return tmp->is_word;
}

// Recursively go to the first child, free whole path
void freeNode(node *t)
{
    for (int i = 0; i < N; i++)
    {
        if (t->children[i] != NULL)
        {
            freeNode(t->children[i]);
        }
    }
    // No more child
    free(t);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Init
    node *trie = root;

    // Call recursive freed
    if (root != NULL)
    {
        freeNode(trie);
        return true;
    }
    return false;
}
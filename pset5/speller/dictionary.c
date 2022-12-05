// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 148444;

// Hash table
node *table[N];

// count for dictionary words and whether it's loaded
unsigned int word_count;
bool check_load;


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO  get index
    unsigned int index = hash(word);

    // check whether word is in dictionary
    for (node *n = table[index]; n != NULL; n = n->next)
    {
        if (strcasecmp(n->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO  this is djb2 hash function
    unsigned int hash = 5381;
    int c;

    for (int i = 0; word[i] != '\0'; ++i)
    {
        if (isupper(word[i]))
        {
            c = tolower(word[i]);
        }
        else
        {
            c = word[i];
        }

        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // scan for every word in dictionary
    char word[LENGTH + 1];
    while (fscanf(dict, "%s", word) != EOF)
    {
        // create memory for new node and copy word into it
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);

        // insert word into hashtable and update word_count
        unsigned int index = hash(n->word);
        n->next = table[index];
        table[index] = n;
        ++word_count;
    }

    fclose(dict);
    check_load = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (check_load)
    {
        return word_count;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    int count = 0;
    for (unsigned int i = 0; i < N; ++i)
    {
        for (node *tmp = table[i]; table[i] != NULL; table[i] = tmp)
        {
            tmp = tmp->next;
            free(table[i]);
            count++;
        }
    }

    // make sure we free every word we load in
    if (count == word_count)
    {
        return true;
    }

    return false;
}

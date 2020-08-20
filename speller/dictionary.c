// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// global variable for tracking dictionary size
unsigned int word_count = 0;

// global boolean for tracking load/unload dictionary operations
bool loaded = false;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}
    
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];
    
    
    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        word_count++;
        
        node *new_node = malloc(sizeof(node));
        
        if (new_node==NULL){
            unload();
            return false;
        }else{
            strcpy(new_node->word, word);
            
            node *head = hashtable[hash(word)] ;
            
            if(head==NULL){
                
                hashtable[hash(word)] = new_node;
                
            }else{
                //new pointer points to previous first node
                new_node->next = hashtable[hash(word)];
                hashtable[hash(word)] = new_node;
            }
            
        }
    }

    // Close dictionary
    fclose(file);
    loaded = true;
    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    
    // TODO
    if(loaded){
        return word_count;
    }else{
        return 0;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int len = strlen(word);
    char word_copy [len+1];
    
    for(int i=0; i<len; i++){
        word_copy[i]= tolower(word[i]);
    }
    
    word_copy[len]= '\0';
    
    node *cursor = hashtable[hash(word_copy)];
    
    while(cursor != NULL){
        if (strcmp(word_copy, cursor->word)==0){
            return true;
        }else{
            cursor = cursor->next;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    int i=0;
    while(i<N){
        node *cursor = hashtable[i];
        while(cursor != NULL){
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp); 
        }
        i++;
    }
    loaded = false;
    return true;
}

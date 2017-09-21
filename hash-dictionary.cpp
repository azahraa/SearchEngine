
// Implementation of a dictionary using a hash table

#include <stdlib.h>
#include <string.h>
#include "hash-dictionary.h"

// Constructor
HashDictionary::HashDictionary()
{
  for(int i = 0; i < MaxBuckets; i++)
  {
    buckets[i] = NULL;
  }
}

int
HashDictionary::hash(KeyType key) {
  // Add your code here
  int sum = 0;
  int len = strlen(key);
  for (int i = 0; i < len; i++) {
		sum += i*key[i];
  }
  return sum%MaxBuckets;
}

// Add a record to the dictionary. Returns false if key already exists
bool
HashDictionary::addRecord( KeyType key, DataType record)
{
  int h = hash(key);
  HashNode *n = buckets[h];

  while(n != NULL)
  {
    if(strcmp(n -> key, key) == 0)
    {
      n -> data = record;
      return false;
    }
    n = n -> next;
  }

  n = new HashNode();
  n -> key = strdup(key);
  n -> data = record;
  n -> next = buckets[h];
  buckets[h] = n;
  nElements++;
  return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
HashDictionary::findRecord( KeyType key)
{
  int h = hash(key);
  HashNode *n = buckets[h];

  while(n != NULL)
  {
    if(strcmp(n -> key, key) == 0)
    {
      return (DataType)n -> data;
    }
    n = n -> next;
  }
  return NULL;
}

// Removes one element from the table
bool
HashDictionary::removeElement(KeyType key)
{
  int h = hash(key);
  HashNode *n = buckets[h];
  HashNode *prev = NULL;
  // Find entry to remove
  while(n != NULL)
  {
    if(strcmp(n -> key, key) == 0)
    {
      // entry found
      break;
    }
    prev = n;
    n = n -> next;
  }
  if(n == NULL)
  {
    // Did not find key
    return false;
  }
  // Two cases for prev
  if(prev == NULL)
  {
    // prev points to first element in the list
    buckets[h] = n -> next;
  }
  else
  {
    // n is an internal node in the list
    prev -> next = n -> next;
    // we need to free the key because we allocated it with strdup
  }
  return true;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
HashDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));

	*n = nElements;
	int i = 0;
	for (int h = 0; h < MaxBuckets; h++) {
		HashNode * n = buckets[h];
		while (n!=NULL) {
			a[i] = n->key;
			i++;
			n = n->next;
		}
	}
	*n = nElements;
	return a;
}

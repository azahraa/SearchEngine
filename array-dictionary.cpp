
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
  // Add your code here
	currentNumber = 0;
	maxNumber = 5000;
	array = new ArrayDictionaryNode[maxNumber];
}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
	for(int i = 0; i < currentNumber; i++)
	{
		if(strcmp(array[i].key, key) == 0)
		{
			array[i].data = (DataType*)record;
			return false;
		}
	}

	if(currentNumber == maxNumber)		// have to resize array
	{
		int oldSize = maxNumber;
		maxNumber *= 2;
		ArrayDictionaryNode *newArray = new ArrayDictionaryNode[maxNumber];

		for(int j = 0; j < oldSize; j++)
		{
			newArray[j].data = new DataType;
			newArray[j].key = (char*)malloc(sizeof(char) * 50);
			newArray[j].data = (DataType*)array[j].data;
			strcpy(newArray[j].key, array[j].key);
		}

		delete [] array;
		array = newArray;
	}

	// insert item here
	array[currentNumber].data = new DataType;
	array[currentNumber].key = (char*)malloc(sizeof(char) * 50);
	array[currentNumber].data = (DataType*)record;
	strcpy(array[currentNumber++].key, key);

	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
  for(int i = 0; i < currentNumber; i++)
	{
		if(strcmp(array[i].key, key) == 0)
		{
			return (DataType)array[i].data;
		}
	}
	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
	int isFound = 0;
  for(int i = 0;i < currentNumber; i++)
	{
		if(strcmp(array[i].key, key) == 0)
		{
			isFound = 1;
		}
	}
	if(isFound)
	{
		int k = 0;
		ArrayDictionaryNode *newArray = new ArrayDictionaryNode[maxNumber];
		for(int j = 0; j < currentNumber; j++)
		{
			if(strcmp(array[j].key, key))
			{
				newArray[k].data = new DataType;
				newArray[k].key = (char*)malloc(sizeof(char) * 50);
				newArray[k].data = (DataType*)array[j].data;
				strcpy(newArray[k++].key, array[j].key);
			}
		}
		delete [] array;
		array = newArray;
		currentNumber--;
		return true;
	}
	else
	{
		return false;
	}
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
	*n = currentNumber;
  KeyType *keys = new KeyType[currentNumber];
	for(int i = 0; i < currentNumber; i++)
	{
		keys[i] = (char*)malloc(sizeof(char) * 50);
		strcpy((char*)keys[i], array[i].key);
	}
	return keys;
}

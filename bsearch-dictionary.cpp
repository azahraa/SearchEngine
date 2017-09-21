
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		sort();
		sorted = true;
	}

	// Use binary search
	int low = 0;
	int high = currentNumber - 1;
	while(high >= low)
	{
		int mid = (high + low) / 2;
		int diff = strcmp(array[mid].key, key);
		if(diff == 0)
		{
			// found
			return array[mid].data;
		}
		else if(diff > 0)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}

	return NULL;
}


// Sort array using heap sort
void
BinarySearchDictionary::sort()
{
	//printf("Before\n");
	//for (int i = 0; i < currentNumber; i++) {
	//	printf("%s\n", array[i].key);
	//}

	ArrayDictionaryNode temp;
	for(int i = 0; i < currentNumber; i++)
	{
		for(int j = 0; j < currentNumber - 1; j++)
		{
			if(strcmp(array[j].key, array[j+1].key) > 0)
			{
				temp.key = array[j].key;
				array[j].key = array[j+1].key;
				array[j+1].key = temp.key;
				temp.data = array[j].data;
				array[j].data = array[j+1].data;
				array[j+1].data = temp.data;
			}
		}
	}

	//printf("After\n");
	//for (int i = 0; i < currentNumber; i++) {
	//	printf("%s\n", array[i].key);
	//}
}

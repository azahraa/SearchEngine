
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
AVLDictionary::addRecord( KeyType key, DataType record)
{
	if ( debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}

	// Add your implementation here

	AVLNode *current = root;
  AVLNode *previous = NULL;

  while(current != NULL)
  {
    previous = current;
    if(strcmp(key, current -> key) == 0)
    {
      // key is found. substitute data
      current -> data = record;
      return false;
    }
		else
    {
      if(strcmp(key, current -> key) < 0)
      {
        current = current -> left;
      }
      else // key > current -> _key
      {
        current = current -> right;
      }
    }
  } // end while

	// key not found. create new node
	AVLNode *n = new AVLNode();
	n -> key = strdup(key);
	n -> data = (DataType)record;
	n -> left = NULL;
	n -> right = NULL;
	n -> parent = NULL;
	n -> height = 1;

	// test if tree is empty
  if(previous == NULL)
  {
    // insert n as root
    root = n;
		nElements++;
    return true;
  }

	// tree is not empty. previous points to parent where new node will be inserted
  if(strcmp(key, previous -> key) < 0)
  {
    previous -> left = n;
  }
  else
  {
    previous -> right = n;
  }
  n -> parent = previous;

	// adjust heights of nodes in path from inserted node to root
  AVLNode *m = n -> parent;
	while(m != NULL) // iterate until root
  {
    int maxHeight = 0;
    if(m -> left != NULL) // check if left exists
    {
      maxHeight = m -> left -> height;
    }
    if(m -> right != NULL && m -> right -> height > maxHeight)
    {
      maxHeight = m -> right -> height;
    }
    m -> height = maxHeight + 1;
    m = m -> parent; // go to parent to recompute height
  }// end while


	//Find node to insert into
	//Node does not exist. Create it.
	//Height might not be valid anymore.
	//We need to restructure .

	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}

	// Call restructure
	restructure(n); // restructure tree if necessary starting at node n


	if (debug) {
		checkRecursive(root);

		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
	}
	nElements++;
	return true;
}

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n

	//Go upwards until root is found

        // See class notes

	// Add your implementation here
	AVLNode *z = n -> parent;
  while(z != NULL)
  {
    // check if this z is parent of the unbalanced tree
    int hleft = 0;
    if(z -> left != NULL)
    {
      hleft = z -> left -> height;
    }
    int hright = 0;
    if(z -> right != NULL)
    {
      hright = z -> right -> height;
    }
    int hdiff = hright - hleft;
		if(hdiff < 0)
    {
      hdiff = (-1) * hdiff;
    }
    if(hdiff <= 1)
    {
      // z is balanced. go up to find a z that is unbalanced
      z = z -> parent;
      continue;  // goes to top of while loop
    }
		// z is unbalanced
    // find y. child of z with max height
    AVLNode *y = NULL;
    int maxh = 0;
    if(z -> left != NULL)
    {
      y = z -> left;
      maxh = z -> left -> height;
    }
    if(z -> right != NULL && maxh < z -> right -> height)
    {
      y = z -> right;
    }
    // we found y by now
    assert(y != NULL);

		// find x
		AVLNode *x = NULL;
    maxh = 0;
    if(y -> left != NULL)
    {
      x = y -> left;
      maxh = y -> left -> height;
    }
    if(y -> right != NULL && maxh < y -> right -> height)
    {
      x = y -> right;
    }
    // we found x by now
    assert(x != NULL);

		// identify a, b, and c
    AVLNode *a, *b, *c, *t0, *t1, *t2, *t3;
    if(z -> right == y)
    {
      // case 1 or 2
      if(y -> right == x)
      {
        // case 1
        a = z;
        b = y;
        c = x;
        t0 = z -> left;
        t1 = y -> left;
        t2 = x -> left;
        t3 = x -> right;
      }
      else
      {
        // case 2
        a = z;
        c = y;
        b = x;
        t0 = z -> left;
        t1 = x -> left;
        t2 = x -> right;
        t3 = y -> right;
      }
    }
		else
    {
      // case 3 or 4
      if(y -> left == x)
      {
        // case 3
        a = x;
        b = y;
        c = z;
        t0 = x -> left;
        t1 = x -> right;
        t2 = y -> right;
        t3 = z -> right;
      }
      else
      {
        // case 4
        a = y;
        c = z;
        b = x;
        t0 = y -> left;
        t1 = x -> left;
        t2 = x -> right;
        t3 = z -> right;
      }
    }

		// do rotation
    AVLNode *p = z -> parent;
    if(p != NULL)
    {
      // not root
      if(p -> left == z)
      {
        // attach b to left of p
        p -> left = b;
      }
      else
      {
        p -> right = b; // attach b to right of p
      }
    }
    else
    {
      // p == NULL, z is root
      root = b;
    }

		// connect a, b, c, t0, t1, t2, t3
    b -> parent = p;
    b -> left = a;
    b -> right = c;
    a -> parent = b;
    a -> left = t0;
    a -> right = t1;
    c -> parent = b;
    c -> left = t2;
    c -> right = t3;

		// connect parents of t0, t1, t2, t3
		if(t0 != NULL)
		{
			t0 -> parent = a;
		}
		if(t1 != NULL)
		{
			t1 -> parent = a;
		}
		if(t2 != NULL)
		{
			t2 -> parent = c;
		}
		if(t3 != NULL)
		{
			t3 -> parent = c;
		}

		// fix height of a
		int maxHeight = 0;
		if(a -> left != NULL)
		{
			maxHeight = a -> left -> height;
		}
		if(a -> right != NULL && a -> right -> height > maxHeight)
		{
			maxHeight = a -> right -> height;
		}
		a -> height = maxHeight + 1;

		// fix height of c
		maxHeight = 0;
		if(c -> left != NULL)
		{
			maxHeight = c -> left -> height;
		}
		if(c -> right != NULL && c -> right -> height > maxHeight)
		{
			maxHeight = c -> right -> height;
		}
		c -> height = maxHeight + 1;

		// fix height of b
		maxHeight = 0;
		if(b -> left != NULL)
		{
			maxHeight = b -> left -> height;
		}
		if(b -> right != NULL && b -> right -> height > maxHeight)
		{
			maxHeight = b -> right -> height;
		}
		b -> height = maxHeight + 1;

		// go up to restructure
		z = z -> parent;
	} // end while
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
	AVLNode *current = root;
	while(current != NULL)
	{
		if(strcmp(current -> key, key) == 0 && current -> data != NULL)
		{
			return (DataType)current -> data;
		}
		else if(strcmp(current -> key, key) < 0)
		{
			current = current -> right;
		}
		else
		{
			current = current -> left;
		}
	}
	return NULL;
}

// Removes one element from the dictionary
bool
AVLDictionary::removeElement(KeyType key)
{
	// if (debug) {
	// 	printf("------------------------------------\n");
	// 	printf("removeElement(\"%s\")\n",  key);
	// 	printf("---------- Before -----------------\n");
	// 	printNode("", root, 0);
	// }

	AVLNode *current = root;
	AVLNode *previous = NULL;
	AVLNode *temp = NULL;
	while(current != NULL)
	{
		previous = current;
		if(strcmp(key, current -> key) == 0)
		{/*
			if(current -> left != NULL && current -> right != NULL)
			{
				// node is internal.
				if(current -> right != NULL)
				{
					// swap with successor
					AVLNode *temp = current -> right;
					while(temp != NULL)
					{
						previous = temp;
						temp = temp -> left;
					}
					if(previous -> parent != NULL && previous -> parent -> right != NULL)
					{
						temp = previous -> parent -> right;
					}
					else if(previous -> parent != NULL)
					{
						temp = previous -> parent;
					}
					// replace current with previous
					previous -> parent = current -> parent;
					previous -> left = current -> left;
					previous -> right = current -> right;
					current -> left -> parent = previous;
					current -> right -> parent = previous;
					if(current != root)
					{
						if(current -> parent == root)
						{
							current -> parent -> right = previous;
						}
						else
						{
							current -> parent -> left = previous;
						}
					}
					current -> key = strdup((char *)previous -> key);
					current -> data = previous -> data;
					delete previous;
					if(temp != NULL)
					{
						restructure(temp);
					}
				}
				else if(current -> left != NULL)
				{
					// swap with predecessor
					AVLNode *temp = current -> left;
					while(temp != NULL)
					{
						previous = temp;
						temp = temp -> right;
					}
					if(previous -> parent != NULL && previous -> parent -> left != NULL)
					{
						temp = previous -> parent -> left;
					}
					else if(previous -> parent != NULL)
					{
						temp = previous -> parent;
					}
					// replace current with previous
					previous -> parent = current -> parent;
					previous -> left = current -> left;
					previous -> right = current -> right;
					current -> left -> parent = previous;
					current -> right -> parent = previous;
					if(current != root)
					{
						if(current -> parent == root)
						{
							current -> parent -> left = previous;
						}
						else
						{
							current -> parent -> right = previous;
						}
					}
					current -> key = strdup((char *)previous -> key);
					current -> data = previous -> data;
					delete previous;
					if(temp != NULL)
					{
						restructure(temp);
					}
				}
			}
			else
			{
				// node is at bottom
				delete current;
			}*/
			// key is found.
			current -> data = NULL;
			return true;
		}
		else
		{
			if(strcmp(key, current -> key) < 0)
			{
				current = current -> left;
			}
			else // key > current -> _key
			{
				current = current -> right;
			}
		}
	} // end while
	// key not found.

	// if (debug) {
	// 	printf("---------- After -----------------\n");
	// 	printNode("", root, 0);
	//
	// 	checkRecursive(root);
	// }

	return false;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
AVLDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*n = 0;
	addKeysRecursive(root, a, n);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node==NULL) {
		return;
	}

	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}

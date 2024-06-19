/**
 * Vector implementation.
 *
 * - Implement each of the functions to create a working growable array (vector).
 * - Do not change any of the structs
 * - When submitting, You should not have any 'printf' statements in your vector 
 *   functions.
 *
 * IMPORTANT: The initial capacity and the vector's growth factor should be 
 * expressed in terms of the configuration constants in vect.h
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "vect.h"

/** Main data structure for the vector. */
struct vect {
  char **data;             /* Array containing the actual data. */
  unsigned int size;       /* Number of items currently in the vector. */
  unsigned int capacity;   /* Maximum number of items the vector can hold before growing. */
};

/** Construct a new empty vector. */
vect_t *vect_new() {

  /* [TODO] Complete the function */

  //allocate space for the vector
  vect_t *v = (vect_t *)malloc(sizeof(vect_t));

  //check whether vector is not Null
  if(v == NULL) {
    return NULL;
  }
  //initalize data
  v->data = (char **)malloc(2 * sizeof(char *));
  if (v->data == NULL) {
    free(v);
    return NULL;
  }

  v->size = 0;

  v->capacity = 2;

  return v;
}

/** Delete the vector, freeing all memory it occupies. */
void vect_delete(vect_t *v) {
  if(v != NULL) {
    //if v isn't already null: 
    if(v->data != NULL) {
     //iterate through underlying array of strings
      for(unsigned int i = 0; i < v->size; i++) {
        //free each string
        free(v->data[i]);
      }
      //free the now empty array of strings
      free(v->data);

      //free the vector
      free(v);
    }
  } 
  else {return;}

}

/** Get the element at the given index. */
const char *vect_get(vect_t *v, unsigned int idx) {
  assert(v != NULL);
  assert(idx < v->size);

  return v->data[idx];
}

/** Get a copy of the element at the given index. The caller is responsible
 *  for freeing the memory occupied by the copy. */
char *vect_get_copy(vect_t *v, unsigned int idx) {
  assert(v != NULL);
  assert(idx < v->size);

  // element at given index
  const char *item = v->data[idx];

  //check whether the element is empty
  if(item == NULL) {
    return NULL;
  }
  //get size of an item
  size_t length = strlen(item);

  //space for the copy
  char *copy = (char *)malloc(length + 1);

  if(copy == NULL) {
    return NULL;
  }

  //data from item to copy of item
  strcpy(copy, item);
  return copy;
}
/** Set the element at the given index. */
void vect_set(vect_t *v, unsigned int idx, const char *elt) {
  assert(v != NULL);
  assert(idx < v->size);

  //set previously occupying value to NULL
  if (v->data[idx] != NULL) {
    free(v->data[idx]);
  }

  //set space to new element 
  if(elt != NULL) {
    v->data[idx] = (char *)malloc(strlen(elt) + 1);
    if(v->data[idx] == NULL) {
      return;
    }
    strcpy(v->data[idx], elt);
  }
  else {
    v->data[idx] = NULL;
  }
}

/** Add an element to the back of the vector. */
void vect_add(vect_t *v, const char *elt) {
  assert(v != NULL);

  //the underlying array needs to be resized(there is no space)
  if(v->size == v->capacity) {
    char **resizedData = (char **)realloc(v->data, v->capacity * sizeof(char *) * 2);
    if(resizedData == NULL) {
      return;
    }

    v->data = resizedData;
    v->capacity = v->capacity * 2;
  }

  //space for new element to be added
  char *item = NULL;
  if(elt != NULL) {
    item = (char *)malloc(strlen(elt) + 1); 
    if(item == NULL) {
      return;
    }
    strcpy(item, elt); 
  }

  //adding the element
  v->data[v->size] = item;
  v->size++;
}

/** Remove the last element from the vector. */
void vect_remove_last(vect_t *v) {
  assert(v != NULL);

  //check if vector is empty
  if (v->size == 0) {
    return;
  }
  // free memory at back of vector
  if(v->data[v->size - 1] != NULL) {
      free(v->data[v->size-1]);
  }
  // Decrease vector size by 1
  v->size = v->size - 1;
}

/** The number of items currently in the vector. */
unsigned int vect_size(vect_t *v) {
  assert(v != NULL);

  /* [TODO] Complete the function */
  
  return v->size;
}

/** The maximum number of items the vector can hold before it has to grow. */
unsigned int vect_current_capacity(vect_t *v) {
  assert(v != NULL);

  /* [TODO] Complete the function */

  return v->capacity;
}

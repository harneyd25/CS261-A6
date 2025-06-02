/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: David Harney
 * Email: harneyd@gmail.com
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"
#include <assert.h>

/*
 * node structure to hold data and priority
 * to be stored in the dynarray
 */
struct node {
  void* data;
  int priority;
};


/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 * in addition, you want to define an element struct with both data and priority, 
 * corresponding to the elements of the priority queue. 
 */
struct pq {
  struct dynarray* da;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* pq = malloc(sizeof(struct pq));
  assert(pq);

  pq->da = dynarray_create();

  return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  
  assert(pq);
  dynarray_free(pq->da);
  free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  assert(pq);
  return (dynarray_length(pq->da) == 0);
}


/* 
 * (use dynarray_set and dynarray_get)
 * helper function that swaps two nodes within the given dynarray
 *
 * Params:
 *  da - the dynammic array containing elements to be swapped. May not be NULL
 *  a - the index of the first element to be swapped
 *  b - the index of the second element to be swapped
 */
void swap_nodes(struct dynarray* da, int a, int b) {
  assert(da);
		
  void* temp = dynarray_get(da, a);
  dynarray_set(da, a, dynarray_get(da, b));
  dynarray_set(da, b, temp);
}

/*
 * helper function that recurively percolates the node at the given index up 
 * through its parent nodes while the node at the given index has a higher 
 * priority than the parent nodes
 *
 * Params:
 *   da - the dynammic array acting as a heap to be percolated. May not be NULL
 *   index - the index of the node to be percolated up through the heap
 */ 
void percolate_up(struct dynarray* da, int index) {
  assert(da);
  
  // base case: hit the root of the heap
  if (index == 0) {
    return;
  }

  // recursive case: not at root
  int parent = (index - 1) / 2; // truncates to correct parent node index

  // getting nodes
  struct node* current = (struct node*)dynarray_get(da, index);
  struct node* parent_node = (struct node*)dynarray_get(da, parent);

  // swapping when child node has higher priority than parent node
  if (current->priority > parent_node->priority) {
    swap_nodes(da, index, parent);
    percolate_up(da, parent);
  }
}

/*
 * helper function that percolates the node at the given index down
 * through its child nodes while the node at the given index has a lower 
 * priority than at least one of the child nodes
 *
 * Params:
 *   da - the dynammic array acting as a heap to be percolated
 *   index - the index of the node to be percolated up through the heap
 */
void percolate_down(struct dynarray* da, int index) {
  assert(da);

  // getting child node indices
  int left_index = 2 * index + 1;
  int right_index = 2 * index + 2;

  // getting highest priority child node
  int highest = index;
  
  struct node* current = (struct node*)dynarray_get(da, index);
  struct node* left_node = NULL;
  if (left_index < dynarray_length(da)) {
    left_node = (struct node*)dynarray_get(da, left_index);
  }
  struct node* right_node = NULL;
  if (right_index < dynarray_length(da)) {
    right_node = (struct node*)dynarray_get(da, right_index);
  }

  // in this case left child has higher priority
  if (left_node && left_node->priority > current->priority) {
    highest = left_index;
	
    // so both child nodes will be compared against eachother 
    current = left_node;
  }

  // in this case right child has higher priority
  if (right_node && right_node->priority > current->priority) {
    highest = right_index;
  }

  // swapping index node down through heap when neccessary
  if (highest != index) {
    swap_nodes(da, index, highest);

    // recursively calling function to continue moving node down through heap
    percolate_down(da, highest);
  }
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, higher priority
 * values are given precedent, and higher place in the queue.  In other words, the
 * element in the priority queue with the highest priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   data - the data value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, higher priority values
 *     should correspond to the first elements.  In other words,
 *     the element in the priority queue with the highest priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* data, int priority) {
  assert(pq);
  
  // making new node
  struct node* new_node = (struct node*)malloc(sizeof(struct node));
  new_node->priority = priority;
  new_node->data = data;

  // inserting new node into last position on heap
  dynarray_insert(pq->da, -1, new_node);

  // percolating up through heap until new node is in correct priority order
  percolate_up(pq->da, dynarray_length(pq->da) - 1);
}


/*
 * This function should return the data of the first element in a priority
 * queue, i.e. the data associated with the element with highest priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the data of the first item in pq, i.e. the item with
 *   max priority value.
 */
void* pq_max(struct pq* pq) {
  assert(pq);

  // getting pointer to root node in heap
  struct node* max_node = (struct node*)dynarray_get(pq->da, 0);

  return max_node->data;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with highest priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with highest priority value.
 */
int pq_max_priority(struct pq* pq) {
  assert(pq);

  // getting pointer to root node in heap
  struct node* max_node = (struct node*)dynarray_get(pq->da, 0);

  return max_node->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with highest priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   highest priority value.
 */
void* pq_max_dequeue(struct pq* pq) {
  assert(pq);

  if (pq_isempty(pq)) {
    return NULL;
  }

  // getting max node
  struct node* max_node = (struct node*)dynarray_get(pq->da, 0);

  // fixing heap since root will be removed
  
    // swaping root node to leaf
  swap_nodes(pq->da, 0, dynarray_length(pq->da) - 1);
    // removing max priority node
  
  dynarray_remove(pq->da, -1);
    // moving node at root to proper order by priority
  if (dynarray_length(pq->da) > 0) {
    percolate_down(pq->da, 0);
  }
  
  void* value = max_node->data;
  free(max_node);

  return value;
}

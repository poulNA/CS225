/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in MP3.1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(NULL);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in MP3.1
    if (length_ == 0) {return;}

    ListNode* curr = head_;

    while (curr != NULL) {
        ListNode* temp = curr;
        curr = curr->next;
        delete temp;
    }  
  }

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  // Not empty
  if (head_ != NULL) {
    head_ -> prev = newNode;
  }
  // When list is empty
  if (tail_ == NULL) {
    tail_ = newNode;
  }
  
  head_ = newNode;
  length_++;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode* newNode = new ListNode(ndata);
  newNode->prev = tail_;
  newNode->next = NULL;

  if (tail_ != NULL) {tail_->next = newNode;}
  // Empty case
  if (head_ == NULL) {
    head_ = newNode;
  }
  tail_ = newNode;
  length_++;

}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.1
  if (splitPoint == 0) {return start;}
  for (int i = 0; i < splitPoint; i++) {
    if (start != NULL) {
      start = start->next;
    }
  }
  
  if (start != NULL) {
    start->prev->next = NULL;
    start->prev = NULL;
    return start;
  }
  return NULL;
}

/**
  * Modifies List using the rules for a TripleRotate.
  *
  * This function will to a wrapped rotation to the left on every three 
  * elements in the list starting for the first three elements. If the 
  * end of the list has a set of 1 or 2 elements, no rotation all be done 
  * on the last 1 or 2 elements.
  * 
  * You may NOT allocate ANY new ListNodes!
  */
template <typename T>
void List<T>::tripleRotate() {
  // @todo Graded in MP3.1

  int offset = length_;
  if (this->length_ == 0) {return;}
  if (this->length_ % 3 == 1) {offset -= 1;}
  if (this->length_ % 3 == 2) {offset -= 2;}


  ListNode* first = this->head_;
  ListNode* temp = NULL;

  for (int i = 0; i < offset; i = i+3) {
    ListNode* second = first->next;
    ListNode* third = first->next->next;
    ListNode* fourth = first->next->next->next;

    first->prev = third;
    third->next = first;
    first->next = fourth;
    

    if (i == 0) {
      head_ = second;
      second->prev = NULL;
    } else {
      second->prev = temp;
      temp->next = second;
    }

    temp = first;
    if (offset == length_) {tail_ = first;}
    first = first->next;
  }


}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.2
  if (startPoint == endPoint) {return;}
  // if (startPoint == NULL || endPoint == NULL) {return;}

  ListNode * spHolder = startPoint;
  ListNode * endHolder = endPoint;

  ListNode * before = spHolder->prev;
  ListNode * after = endHolder->next;

  ListNode * curr = startPoint;
  ListNode * temp;
  while (curr != endHolder) {
    temp = curr->next;

    curr->next = curr->prev;
    curr->prev = temp;

    curr = temp;
  }
  temp = curr->next;
  curr->next = curr->prev;
  curr->prev = temp;

  // end connections
  if (before != NULL) {
    before->next = endHolder;
  }
  endHolder->prev = before;

  spHolder->next = after;
  if (after != NULL) {
    after->prev = spHolder;
  }

  // swap start and end
  if (startPoint == head_) {head_ = endHolder;}
  if (endPoint == tail_) {tail_ = spHolder;}
  if (startPoint != head_ && endPoint != tail_) {
    temp = startPoint;
    startPoint = endPoint;
    endPoint = temp;
  }
}
/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.2
  ListNode * start = head_;
  ListNode * end = start;
  int count = n;

  if (n == 0 || n == 1) {return;}

  while (start != NULL) {
    if (count == 1 || end == tail_) {
      reverse(start, end);
      if (end == tail_) {return;}
      count = n;
      start = end->next;
      end = end->next;
    }
    end = end->next;
    count--;
  }
  
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2
  ListNode * ret;
  ListNode * result;
  ListNode * newSecond;
  if (first == NULL && second != NULL) {return second;}
  if (second == NULL && first != NULL) {return first;}
  if (first == NULL && second == NULL) {return NULL;}

  // Check which list starts smaller
  if (first->data < second->data) {
    result = first;
    newSecond = second;
  } else {
    result = second;
    newSecond = first;
  }
  ret = result;

  ListNode * newFirst = split(result, 1);

  while (newFirst != NULL || newSecond != NULL) {

    if (newFirst == NULL) {
      result->next = newSecond;
      newSecond->prev = result;
      result = result->next;
      // Save the connected node and split after
      newSecond = split(result, 1);
    } else if (newSecond == NULL) {
      result->next = newFirst;
      newFirst->prev = result;
      result = result->next;
      // Save the connected node and split after
      newFirst = split(result, 1);
    } else {
      // Neither are NULL
      if (newFirst->data < newSecond->data) {
        result->next = newFirst;
        newFirst->prev = result;
        result = result->next;
        // Save the connected node and split after
        newFirst = split(result, 1);
      } else {
        result->next = newSecond;
        newSecond->prev = result;
        result = result->next;
        // Save the connected node and split after
        newSecond = split(result, 1);
      }
    }
  }
  return ret;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in MP3.2
  // Base Case: A list of size 1 is sorted. Return.
  if (chainLength == 1 || chainLength == 0) {return start;}

  // Split the current list into two smaller, more manageable parts
  int half = chainLength/2;
  ListNode * firstHalf = start;
  ListNode *  secondHalf = split(firstHalf, half);
  // Sort the two halves (this should be a recursive call)
  ListNode * first = mergesort(firstHalf, half);
  ListNode * second = mergesort(secondHalf, chainLength-half);
  // Merge the two sorted halves back together into a single list
  return merge(first, second);
}

/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] < second[curDim]) {return true;}
    if (first[curDim] > second[curDim]) {return false;}
    if (first < second) {
      return true;
    } else {
      return false;
    }
    
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    double potDist = 0.0;
    double currBDist = 0.0;

    for (int i = 0; i < Dim; i++) {
      potDist += (potential[i]-target[i]) * (potential[i]-target[i]);
      currBDist += (currentBest[i]-target[i]) * (currentBest[i]-target[i]);
    }
  
    if (potDist < currBDist) {return true;}
    if (potDist > currBDist) {return false;}
    if (potential < currentBest) {
      return true;
    } else {
      return false;
    }
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    // If vector is empty
    size = newPoints.size();
    if (size == 0) {
      root = NULL;
      return;
    }
    vector<Point<Dim>> points = newPoints;
    buildTree(points, 0, 0, points.size()-1, root);

}

template <int Dim>
void KDTree<Dim>::buildTree(vector<Point<Dim>> & points, int dim, int lefti, int righti, KDTreeNode *& curRoot) {
  if (points.size() == 0) {return;}

  if (lefti <= righti) {
    int middle = (lefti + righti)/2;
    points[middle] = quickselect(points, lefti, righti, middle, dim);
    curRoot = new KDTreeNode(points[middle]);

    buildTree(points, (dim+1)%Dim, lefti, middle-1, curRoot->left);
    buildTree(points, (dim+1)%Dim, middle+1, righti, curRoot->right);
  }
}

template <int Dim>
Point<Dim> KDTree<Dim>::quickselect(vector<Point<Dim>> & vect, int left, int right, int k, int dim) {
  while (true) {
    if (left == right) {return vect[right];}
    int pivot = (left+right)/2;
    pivot = partition(vect, left, right, pivot, dim);

    if (k == pivot) {
      return vect[k];
    } else if (k < pivot) {
      right = pivot - 1;
    } else {
      left = pivot + 1;
    }
  }
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>> & vect, int left, int right, int pivot, int dim) {
  Point<Dim> pivotVal = (vect[pivot]);
  swap((vect[pivot]), (vect[right]));
  int leftIndex = left;

  for (int i = left; i < right; i++) {
    if (smallerDimVal(vect[i], pivotVal, dim)) {
      swap(vect[leftIndex], vect[i]);
      leftIndex++;
    }
  }
  swap(vect[right], vect[leftIndex]);
  return leftIndex;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  size = other.size;
  root = copy(other->root);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if (this != &rhs) {
    delete *this;
    this = new KDTree(rhs);
  }
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  destroy(this->root);
  size = 0;
}

template<int Dim>
void KDTree<Dim>::copy(KDTreeNode * curr) {
  if (curr == NULL) {return;}

  this->left = copy(curr->left);
  KDTreeNode * node = new KDTreeNode(curr);
  this->right = copy(curr->right);
}

template<int Dim>
void KDTree<Dim>::destroy(KDTreeNode * root) {
  if (root == NULL) {return;}

  destroy(root->left);
  KDTreeNode * temp = root;
  delete temp;
  destroy(root->right);
}



template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    Point<Dim> nearest = root->point;
    return findNNHelper(query, 0, root, root->point);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNNHelper(const Point<Dim> query, int dim, KDTreeNode * curr, Point<Dim> nearest) const
{
    /**
     * @todo Implement this function!
     */
  
    if (curr->left == NULL && curr->right == NULL) {return curr->point;}
    // bool leftCheck = false;
    if (smallerDimVal(query, curr->point, dim) == true) {
      if (curr->left != NULL) {
        nearest = findNNHelper(query, (dim+1)%Dim, curr->left, nearest);
        // leftCheck = true;
      }
    } else {
      // Right recursion
      if (curr->right != NULL) {
        nearest = findNNHelper(query, (dim+1)%Dim, curr->right, nearest);
      }
    }



    if (shouldReplace(query, nearest, curr->point)) {nearest = curr->point;}

    double radius = 0.0;
    double splitDist = (curr->point[dim] - query[dim])*(curr->point[dim] - query[dim]);
    for (int i = 0; i < Dim; i++) {
      radius += (nearest[i]-query[i])*(nearest[i]-query[i]);
    }

  
    if (radius >= splitDist) {
      if (smallerDimVal(query, curr->point, dim) == true) {
        if (curr->right != NULL) {
          Point<Dim> temp = findNNHelper(query, (dim+1)%Dim, curr->right, nearest);
          if (shouldReplace(query, nearest, temp)) {nearest = temp;}
        }
      } else {
          if (curr->left != NULL) {
            Point<Dim> temp = findNNHelper(query, (dim+1)%Dim, curr->left, nearest);
            if (shouldReplace(query, nearest, temp)) {nearest = temp;}
          }
        }
      }

    return nearest;
}

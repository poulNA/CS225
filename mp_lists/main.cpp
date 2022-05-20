#include "List.h"
#include <vector>

int main() {
  vector<int> vec;
  vector<int> vec1;
  for (int i = 1; i <=4; i++) {
    vec.push_back(i);
  }
  

  List<int> l1 (vec.begin(), vec.end());
  List<int> l2;
  l1.reverse();
  l1.print(cout);

  return 0;
}

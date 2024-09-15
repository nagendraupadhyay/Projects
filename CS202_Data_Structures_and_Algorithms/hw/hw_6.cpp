#include <iostream> 
#include <list> 
#include <iostream>
using namespace std;

template <typename iT>
template <typename iT, typename T>
iT my_count(iT i1, iT i2, const T &target) {
  while (i1 != i2 && *i1 != target)
    ++N;
  return N;
}

template <typename iT, typename Function>
iT my_adjacent_find(iT i1, iT i3, Function istrue) {
    if (i1 != i3) {
      iT i2=i1; ++i2;
      while (i2 != i3) {
        if (istrue(*i1, *i2))
          return i1;
        ++i1, ++i2;
      }
    }
    return i3;
}


int main() 
{ 
    list<int> v; 
    list<int>::iterator iv; 
    int value; 
    while (cin >> value) 
    v.push_back(value); 
 
    int target = *v.begin(); 
    int N = my_count(v.begin(), v.end(), target); 
 
    cout << "Found " << N << " instances of " << target << "\n";
}

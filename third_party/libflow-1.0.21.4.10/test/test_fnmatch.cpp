#include <fnmatch.h>
#include <iostream>

using namespace std;

int main() {
  int a = fnmatch("*.foo", "test.foo", 0);
  cout << a << endl;

  a = fnmatch("*.bar", "test.bar", 0);
  cout << a << endl;

  a = fnmatch("*.bar", "test1.bar", 0);
  cout << a << endl;

  a = fnmatch("*.bar.*", "test1.bar1", 0);
  cout << a << endl;

  return 0;
}

# deco_cpp
Python style decorator in C++

In Python, you can easily use the decorator to enhance a function, like
```python
import functools

@functools.cache
def fibonacci(a):
    if a == 1:
        return 0
    elif a == 2:
        return 1
    else:
        return fibonacci(a - 2) + fibonacci(a - 1)
    
for i in range(1, 50):
    print(fibonacci(i), end=' ')
```
`deco_cpp` allows you to use the similar syntax in cpp.
c++17 required.

# Example

This example shows how to write a recursive fibonacci sequence function, with a memorization decorator. Note the `auto` in function parameter is a c++20 feature. If you use c++17, please change to the `template<typename T> uint64_t Call(T& self, uint32_t a)`.
```c++
#include "decorator.hpp"

struct Fibonacci : deco::Decorate<Fibonacci, deco::Memorization> {
  uint64_t Call(auto& self, uint32_t a) {
    if (a == 1) {
      return 0;
    } else if (a == 2) {
      return 1;
    } else {
      return self(a - 1) + self(a - 2);
    }
  }
};

int main() {
  Fibonacci fib;
  for (int i = 1; i < 50; i++) {
    std::cout << fib(i) << ' ';
  }
}
```

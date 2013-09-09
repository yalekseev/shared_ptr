shared_ptr
================

An implementation of a thread safe shared pointer

Usage
=====

````cpp

#include "shared_ptr.h"

int main() {
    util::SharedPtr<int> sp(new int(10));

    /* ... */

    return 0;
}

```

#include "shared_ptr.h"

#include <cassert>

void test_copy_assignment() {
    util::SharedPtr<int> sp1(new int(111));
    util::SharedPtr<int> sp2;

    sp2 = sp1;

    assert(*sp1 == 111);
    assert(*sp1 == *sp2);
}

void test_copy_construction() {
    util::SharedPtr<int> sp1(new int(111));
    util::SharedPtr<int> sp2(sp1);

    assert(*sp1 == 111);
    assert(*sp1 == *sp2);
}


void test_swap() {
    util::SharedPtr<int> sp1(new int(1));
    util::SharedPtr<int> sp2(new int(2));

    // Thanks to ADL the util::swap will be called
    swap(sp1, sp2);

    assert(*sp1 == 2);
    assert(*sp2 == 1);
}

int main() {
    test_copy_assignment();
    test_copy_construction();
    test_swap();

    return 0;
}

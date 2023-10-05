#include "benchmark0.h"

int Base::concrete() const {
    return 7;
}

int Base::virt() const {
    return 7;
}

int main() {
    Base b;

    benchmark(&b, 1000000);
}

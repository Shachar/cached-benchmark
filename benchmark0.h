#pragma once

#include <stddef.h>

class Base {
public:
    int concrete() const;
    virtual int virt() const;
};

void benchmark(Base *b, size_t num_iterations);

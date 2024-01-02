#include "benchmark0.h"

#include "time.h"

void benchmark(Base *b, size_t num_iterations) {
    // Populate cache
    auto result = b->concrete();

    auto start = Clock::now();
    for( size_t i=0; i<num_iterations; ++i ) {
        result ^= b->concrete();
    }
    auto end = Clock::now();

    std::cout<<"Interim value 1: "<<result<<"\n";
    auto concrete_duration = end-start;

    result = b->virt();
    start = Clock::now();
    for( size_t i=0; i<num_iterations; ++i ) {
        result ^= b->virt();
    }
    end = Clock::now();

    std::cout<<"Interim value 2: "<<result<<"\n";
    auto virt_duration = end-start;

    std::cout<<"Concrete duration: "<<concrete_duration<<"\n";
    std::cout<<"Virtual duration: "<<virt_duration<<"\n";
    std::cout<<"Slowdown "<<(virt_duration.count()*100/concrete_duration.count() - 100)<<"%\n";
}

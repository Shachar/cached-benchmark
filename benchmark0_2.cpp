#include "benchmark0.h"

#include "time.h"

void benchmark(Base *b, size_t num_iterations) {
    // Populate cache
    b->concrete();
    b->virt();

    auto start = Clock::now();
    for( size_t i=0; i<num_iterations; ++i ) {
        b->concrete();
    }
    auto end = Clock::now();

    auto concrete_duration = end-start;

    start = Clock::now();
    for( size_t i=0; i<num_iterations; ++i ) {
        b->virt();
    }
    end = Clock::now();

    auto virt_duration = end-start;


    std::cout<<"Concrete duration: "<<concrete_duration<<"\n";
    std::cout<<"Virtual duration: "<<virt_duration<<"\n";
    std::cout<<"Slowdown "<<(virt_duration.count()*100/concrete_duration.count() - 100)<<"%\n";
}

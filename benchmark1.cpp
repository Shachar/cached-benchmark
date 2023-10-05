#include "time.h"

#include <array>
#include <iostream>
#include <memory>
#include <random>

class Base {
public:
    virtual uint64_t method(uint64_t num) = 0;
    virtual ~Base() {}
};

constexpr uint64_t values[16] = { 0x3d, 0x2f, 0x09, 0xf5, 0xeb, 0xdf, 0xe9, 0x6a, 0x7d, 0xa4, 0x94, 0x66, 0x35, 0x65, 0x9c, 0x27  };

#define VCLS(n) class Virtual##n : public Base { \
public: \
        uint64_t method(uint64_t num) final { return num * values[n]; } \
}

VCLS(0);
VCLS(1);
VCLS(2);
VCLS(3);
VCLS(4);
VCLS(5);
VCLS(6);
VCLS(7);
VCLS(8);
VCLS(9);
VCLS(10);
VCLS(11);
VCLS(12);
VCLS(13);
VCLS(14);
VCLS(15);

#define CONCRETE(n) uint64_t concrete##n(uint64_t num) {  return num * values[n]; }

CONCRETE(0)
CONCRETE(1)
CONCRETE(2)
CONCRETE(3)
CONCRETE(4)
CONCRETE(5)
CONCRETE(6)
CONCRETE(7)
CONCRETE(8)
CONCRETE(9)
CONCRETE(10)
CONCRETE(11)
CONCRETE(12)
CONCRETE(13)
CONCRETE(14)
CONCRETE(15)

std::unique_ptr<Base> alloc( uint32_t type ) {
    switch(type) {
#define CASE(n) case n: return std::make_unique<Virtual##n>()
        CASE(0);
        CASE(1);
        CASE(2);
        CASE(3);
        CASE(4);
        CASE(5);
        CASE(6);
        CASE(7);
        CASE(8);
        CASE(9);
        CASE(10);
        CASE(11);
        CASE(12);
        CASE(13);
        CASE(14);
        CASE(15);
    }

    abort();
}

int main(int argc, char *argv[]) {
    if( argc<3 ) {
        std::cerr<<"Usage: benchmark1 NumFunctions NumIterations\n";

        return 1;
    }
    const size_t NumFunctions = strtoul(argv[1], nullptr, 0);
    const size_t NumIterations = strtoul(argv[2], nullptr, 0);

    std::vector<std::unique_ptr<Base>> virtuals;
    std::vector<uint32_t> concretes;

    std::random_device os_seed;
    const uint32_t seed = os_seed();

    std::default_random_engine generator( seed );
    std::uniform_int_distribution< uint32_t > distribute( 0, 15 );

    for( unsigned i=0; i<NumFunctions; ++i ) {
        uint32_t random = distribute(generator);
        concretes.push_back( random );
        virtuals.push_back( alloc( random ) );
    }

    std::cerr<<"Starting virtual run\n";
    uint64_t res1 = 0;
    Timepoint start = Clock::now();
    for( unsigned i=0; i<NumIterations; ++i ) {
        for( unsigned j=0; j<NumFunctions; ++j ) {
            res1 += virtuals[j]->method(i+j);
        }
    }
    Timepoint end = Clock::now();

    Duration virt_run = end-start;

    std::cerr<<"Starting concrete run\n";
    uint64_t res2 = 0;
    start = Clock::now();
    for( unsigned i=0; i<NumIterations; ++i ) {
        for( unsigned j=0; j<NumFunctions; ++j ) {
            switch(concretes[j]) {
#define CALL(n) case n: res2 += concrete##n(i+j); break
                CALL(0);
                CALL(1);
                CALL(2);
                CALL(3);
                CALL(4);
                CALL(5);
                CALL(6);
                CALL(7);
                CALL(8);
                CALL(9);
                CALL(10);
                CALL(11);
                CALL(12);
                CALL(13);
                CALL(14);
                CALL(15);
            }
        }
    }
    end = Clock::now();
    
    Duration conc_run = end-start;

    /*
    std::cout<<"Virtual returned "<<res1<<", ran for "<<virt_run<<"\n";
    std::cout<<"Concrete returned "<<res2<<", ran for "<<conc_run<<"\n";
    std::cout<<"Virtual is "<<((static_cast<double>(virt_run.count()) / conc_run.count()) * 100 - 100)<<"% slower\n";
    */

    std::cout<<
            NumIterations<<","<<NumFunctions<<","<<
            res1<<","<<std::chrono::duration_cast<std::chrono::nanoseconds>(virt_run).count()<<","<<
            res2<<","<<std::chrono::duration_cast<std::chrono::nanoseconds>(conc_run).count()<<","<<
            ( double(virt_run.count())/conc_run.count() * 100 - 100 )<<"\n";
}

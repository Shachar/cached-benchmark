#include "time.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <algorithm>
#include <iostream>

class BaseRunner {
public:
    virtual bool run(const char *string, size_t size) = 0;
};

class Runner1 : public BaseRunner {
    unsigned value = 0;

public:
    bool run(const char *string, size_t size) final {
        for( unsigned i=0; i<size; ++i ) {
            value += string[i];
        }

        return value%2;
    }
};

class Runner2 : public BaseRunner {
    unsigned value = 0;

public:
    bool run(const char *string, size_t size) final {
        for( unsigned i=0; i<size; ++i ) {
            value += string[i];
        }
        value += 1;

        return value&2;
    }
};

void check_error(int res, const char *op) {
    if( res==-1 ) {
        std::cerr<<op<<" failed: "<<strerror(errno)<<"\n";
        abort();
    }
}

template<typename T, size_t Step>
bool calc_run(size_t size, const char *ptr) {
    bool res = false;

    T runner_c;

    while(size>0) {
        size_t chunk = std::min(Step, size);
        res = runner_c.run(ptr, chunk);
        ptr += chunk;
        size -= chunk;
    }

    return res;
}

int main(int argc, char *argv[]) {
    constexpr size_t Step = 256;
    int fd = open(argv[1], O_RDONLY);
    check_error(fd, "open");
    struct stat status;
    check_error( fstat(fd, &status), "fstat" );
    size_t size = status.st_size;
    const char *map = (const char *)mmap(nullptr, size, PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED, fd, 0);
    const char *ptr;
    Timepoint start, end;
    Duration virt_dur = Duration::max(), conc_dur = Duration::max();
    bool res_v, res_c;

    for( int i=0; i<100; ++i ) {

        std::cerr<<"Virtual run\n";

        BaseRunner *runner_v;
        if( status.st_size&16 )
            runner_v = new Runner1;
        else
            runner_v = new Runner2;

        res_v = false;
        ptr = map;
        size = status.st_size;

        start = Clock::now();
        while(size>0) {
            size_t chunk = std::min<size_t>(Step, size);
            res_v = runner_v->run(ptr, chunk);
            ptr += chunk;
            size -= chunk;
        }
        end = Clock::now();
        virt_dur = std::min( end-start, virt_dur );

        std::cerr<<"Concrete run\n";

        size = status.st_size;
        ptr = map;
        res_c = false;

        start = Clock::now();
        if( status.st_size&16 )
            res_c = calc_run<Runner1, Step>(size, ptr);
        else
            res_c = calc_run<Runner2, Step>(size, ptr);
        end = Clock::now();

        conc_dur = std::min( end-start, conc_dur );
    }
    
    std::cout<<
            "Virtual run took "<<virt_dur<<" resulting in "<<res_v<<
            ", concrete run took "<<conc_dur<<" resulting in "<<res_c<<
            ". Virtual ran "<<( double(virt_dur.count()) / conc_dur.count() * 100 - 100 )<<"% slower\n";
}

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

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    check_error(fd, "open");
    struct stat status;
    check_error( fstat(fd, &status), "fstat" );
    size_t size = status.st_size;
    const char *map = (const char *)mmap(nullptr, size, PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED, fd, 0);
    const char *ptr;
    Timepoint start, end;
    Duration virt_dur, conc_dur;
    bool res_v, res_c;

    for( int i=0; i<3; ++i ) {

        std::cerr<<"Virtual run\n";

        BaseRunner *runner_v = new Runner1;
        res_v = false;
        ptr = map;
        size = status.st_size;

        start = Clock::now();
        while(size>0) {
            size_t chunk = std::min<size_t>(255, size);
            res_v = runner_v->run(ptr, chunk);
            ptr += chunk;
            size -= chunk;
        }
        end = Clock::now();
        virt_dur = end-start;

        std::cerr<<"Concrete run\n";

        size = status.st_size;
        ptr = map;
        Runner1 runner_c;
        res_c = false;

        start = Clock::now();
        while(size>0) {
            size_t chunk = std::min<size_t>(255, size);
            res_c = runner_c.run(ptr, chunk);
            ptr += chunk;
            size -= chunk;
        }
        end = Clock::now();

        conc_dur = end-start;
    }
    
    std::cout<<
            "Virtual run took "<<virt_dur<<" resulting in "<<res_v<<
            ", concrete run took "<<conc_dur<<" resulting in "<<res_c<<
            ". Virtual ran "<<( double(std::chrono::duration_cast<std::chrono::nanoseconds>(virt_dur).count()) / std::chrono::duration_cast<std::chrono::nanoseconds>(conc_dur).count() * 100 - 100 )<<"% slower\n";
}

#include "function.h"

uint64_t __attribute__((aligned(@@ALIGNMENT@@))) benchmark_function_@@NUMBER@@(uint64_t run_num, Class *cls) {
    return run_num+@@NUMBER@@;
}

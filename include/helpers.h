#ifndef __EVOLVE_HELPERS
#define __EVOLVE_HELPERS

#include "common.h"
#include <iostream>

std::ostream &operator<< (std::ostream &stream, const Chromosome &obj) {
    for (auto gene: obj.genes) {
        stream << gene.get_name();    
    }
    return stream;
};

#endif
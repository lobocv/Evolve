#ifndef __EVOLVE_HELPERS
#define __EVOLVE_HELPERS

#include "common.h"
#include "creature.h"
#include "genetics.h"
#include <iostream>

std::ostream &operator<< (std::ostream &stream, const Chromosome &obj) {
    for (auto gene: obj.genes_) {
        stream << gene.get_name();    
    }
    return stream;
};

std::ostream &operator<< (std::ostream &stream, const Species &obj) {
    stream << "Species " << obj.get_name();    
    return stream;
};


std::ostream &operator<< (std::ostream &stream, const Creature &obj) {
    stream << "Creature (" << obj.get_species() << ") #" << obj.get_id();
    return stream;
};


#endif
#ifndef __EVOLVE_HELPERS
#define __EVOLVE_HELPERS

#include "common.h"
#include "creature.h"
#include "genetics.h"
#include "trait.h"
#include <iostream>

std::ostream &operator<< (std::ostream &stream, const Gene &obj)
{
    stream << obj.get_name();    
    return stream;
}

std::ostream &operator<< (std::ostream &stream, const Chromosome &obj)
{
    for (auto gene: obj.genes_) {
        stream << gene.second;
    }
    return stream;
}

std::ostream &operator<< (std::ostream &stream, const Species &obj)
{
    stream << "Species:" << obj.get_name();
    return stream;
}


std::ostream &operator<< (std::ostream &stream, const Creature &obj)
{
    stream << "Creature (" << "#" << obj.get_id() << ",";
    (obj.get_sex() == Male) ? (stream << "M") : (stream << "F");
    stream << "," << obj.get_species() << ")";
    return stream;
}


std::ostream &operator<< (std::ostream &stream, const Trait &obj)
{
    stream << obj.get_name() << " (";
    for (auto gene: obj.get_genes()) {
        stream << gene;
    }
    stream << ")";
    return stream;
}



#endif

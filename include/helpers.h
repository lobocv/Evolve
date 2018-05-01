#ifndef __EVOLVE_HELPERS
#define __EVOLVE_HELPERS

#include <iostream>

#include "common.h"
#include "creature.h"
#include "genetics.h"
#include "trait.h"


// must be inline if you are going to define functions inside a header file

inline std::ostream &operator<< (std::ostream &stream, const Gene &obj)
{
    stream << obj.get_name();    
    return stream;
}

inline std::ostream &operator<< (std::ostream &stream, const Chromosome &obj)
{
    for (auto gene: obj.get_genes())
    {
        stream << gene.second;
    }
    return stream;
}

inline std::ostream &operator<< (std::ostream &stream, const Species &obj)
{
    stream << "Species " << obj.get_name();    
    return stream;
}


inline std::ostream &operator<< (std::ostream &stream, const Creature &obj)
{
    stream << "Creature (" << "#" << obj.get_id() << ",";
    (obj.get_sex() == Male) ? (stream << "M") : (stream << "F");
    stream << "," << obj.get_species() << ")";
    return stream;
}


inline std::ostream &operator<< (std::ostream &stream, const Trait &obj)
{
    stream << obj.get_name() << " (";
    for (auto gene: obj.get_genes())
    {
        stream << gene;
    }
    stream << ")";
    return stream;
}


#endif

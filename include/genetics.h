#ifndef __Genetics
#define __Genetics

#include <vector>
#include <map>
#include <ostream>
#include "common.h"

class Gene {
    char name;
public:
    Gene() = default;
    Gene(char);
    char get_name() const;
};

class Chromosome {
    int id;    
public:
    const int ASD=6;
    Chromosome() = default;
    Chromosome(std::string);
    std::vector<Gene> genes;
    static ChromosomePair MakePair(std::string);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
};


#endif
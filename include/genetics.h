#ifndef __Genetics
#define __Genetics

#include <vector>
#include <map>
#include <ostream>
#include "common.h"

class Gene {
    char name_;
public:
    Gene() = default;
    Gene(char);
    const char& get_name() const;
};

class Chromosome {
    int id;    
    std::vector<Gene> genes;
public:
    const int ASD=6;
    Chromosome() = default;
    Chromosome(std::string);
    static ChromosomePair MakePair(std::string);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
};

#endif
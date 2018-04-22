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
    int id_;    
    std::vector<Gene> genes_;
public:
    Chromosome() = default;
    Chromosome(std::string);
    static ChromosomePair MakePair(std::string);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
};

#endif
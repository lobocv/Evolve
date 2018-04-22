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
    std::vector<Gene> genes_;
public:
    Chromosome() = default;
    Chromosome(std::string);
    static ChromosomePair MakeRandomPair(std::string);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
    const int get_length() const;
};

#endif
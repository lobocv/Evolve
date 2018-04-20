#ifndef __Gene
#define __Gene

#include <vector>
#include <unordered_set>
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
    Chromosome(geneset);
    std::vector<Gene> genes;
    static std::pair<Chromosome, Chromosome> MakePair(geneset);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
};



#endif
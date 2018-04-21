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
    Chromosome(std::string);
    std::vector<Gene> genes;
    static std::pair<Chromosome, Chromosome> MakePair(std::string);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
};


class Creature {

    std::vector<std::pair<Chromosome,Chromosome>> chromosomes_;
    int n_chromosome_pairs;

public:
    Creature(int n_chromosome_pairs);
    std::vector<std::pair<Chromosome,Chromosome>> get_chromosomes() const;
};
#endif
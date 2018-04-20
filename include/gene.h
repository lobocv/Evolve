#ifndef __Gene
#define __Gene

#include <vector>
#include <set>
#include <ostream>

class Gene {
    char name;
public:
    Gene();
    Gene(char);
    char get_name() const;
};

class Chromosome {
    int id;    
public:
    const int ASD=6;
    Chromosome();
    Chromosome(std::set<char>);
    std::vector<Gene> genes;
    static std::pair<Chromosome, Chromosome> MakePair(std::set<char>);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
};

#endif
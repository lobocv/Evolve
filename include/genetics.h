#ifndef __Genetics
#define __Genetics

#include <vector>
#include <map>
#include <ostream>
#include "common.h"

class Gene {
    char name_;
    GeneType type_;
public:
    Gene() = default;
    Gene(char, GeneType);
    const char get_name() const;
    const GeneType& get_type() const;
    bool operator< (const Gene &other) const;
};

class Chromosome {
    GeneSequence genes_;
public:
    Chromosome() = default;
    Chromosome(GeneSequence);
    static ChromosomePair MakeRandomPair(std::string);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
    const int get_length() const;
    const GeneSequence& get_genes() const;
};


class Trait {
    std::string name_;
    std::string gene_codes_;
    TraitType type_;
    float ComputePolygeneStrength(Genome &genome);
public:
    Trait() = default;
    Trait(std::string name, TraitType type, std::string genes);
    const std::string get_name() const;
    const std::string& get_genes() const;
    float CalculateValue(Genome &genome);
};

#endif

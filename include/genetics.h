#ifndef __Genetics
#define __Genetics

#include <iostream>
#include "common.h"

class Gene
{
    char name_;
    GeneType type_;
public:
    Gene() = default;
    Gene(char, GeneType);
    const char get_name() const;
    const GeneType& get_type() const;
};

class Chromosome
{
    GeneSequence genes_;
public:
    Chromosome() = default;
    Chromosome(GeneSequence);
    static ChromosomePair MakeRandomPair(std::string);
    const int get_length() const;
    const GeneSequence& get_genes() const;
};



std::pair<int, int> GetAlleleRatio(const std::string &gene_codes, const Genome &genome);

std::ostream &operator<< (std::ostream &stream, const Gene &obj);

std::ostream &operator<< (std::ostream &stream, const Chromosome &obj);

#endif

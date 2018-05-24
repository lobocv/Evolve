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
    const char GetName() const;
    const GeneType& GetType() const;
};

class Chromosome
{
    GeneSequence genes_;
public:
    Chromosome() = default;
    Chromosome(GeneSequence);
    static ChromosomePair MakeRandomPair(std::string);
    const int GetLength() const;
    const GeneSequence& GetGenes() const;
};



std::pair<int, int> GetAlleleRatio(const std::string &gene_codes, const Genome &genome);

std::ostream &operator<< (std::ostream &stream, const Gene &obj);

std::ostream &operator<< (std::ostream &stream, const Chromosome &obj);

#endif

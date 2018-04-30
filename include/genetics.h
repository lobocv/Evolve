#ifndef __Genetics
#define __Genetics

#include <vector>
#include <map>
#include <memory>
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
protected:
    std::string name_;
    std::string gene_codes_;
public:
    Trait() = default;
    Trait(std::string name, std::string genes);
    const std::string get_name() const;
    const std::string& get_genes() const;
    virtual float CalculateValue(const Genome &genome)=0;
};

class ContinuousTrait : public Trait {
    float max_;
    float min_;
  public:
    ContinuousTrait() = default;
    ContinuousTrait(std::string name, std::string genes, float max, float min);
    float CalculateValue(const Genome &genome);
    std::pair<float, float> CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures);
};


class DiscreteTrait : public Trait {
  public:
    DiscreteTrait() = default;
    DiscreteTrait(std::string name, std::string genes);
    float CalculateValue(const Genome &genome);
};


std::pair<int, int> GetAlleleRatio(const std::string &gene_codes, const Genome &genome);

#endif

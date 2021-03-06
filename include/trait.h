#ifndef TRAIT_H
#define TRAIT_H

#include <iostream>
#include <memory>
#include <cmath>
#include "common.h"
#include "genetics.h"
#include "creature.h"



class Trait
{
protected:
    std::vector<Phenovector> phenotype_vectors_;
    std::map<char, Phenovector> gene_phenovectors_;
    virtual void InitializeGenevectors()=0;
    std::vector<std::shared_ptr<TraitWeighting>> weights_;
    std::string name_;
    std::string gene_codes_;
public:
    const std::vector<std::string> phenotypes_;
    Trait() = default;
    virtual ~Trait() {}
    virtual void InitializePhenospace()=0;
    Trait(std::string name, std::string genes, std::vector<std::string> phenotypes);
    const std::string GetName() const;
    const std::string& GetGenes() const;
    Phenovector CumulativePhenovector(const Genome &genome);
    int PhenovectorMaxDimension(Phenovector trait_vec);
    virtual std::string ValueToPhenotype(Phenovector trait_vec);
    std::string ValueToPhenotype(std::shared_ptr<Creature> c);
    std::pair<std::vector<float>, std::vector<float>> CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures);
    std::map<std::string, int> CalculatePhenotypeStatistics(const std::vector<std::shared_ptr<Creature>> creatures);
    virtual float ApplyWeighting(PhenotypeWeights weights, Phenovector trait_vec);

    friend class Ecosystem;
};

class DiscreteTrait : public Trait
{
    void InitializePhenospace();
    void InitializeGenevectors();
public:
    DiscreteTrait() = default;
    DiscreteTrait(std::string name, std::string genes, std::vector<std::string> phenotypes);
};


class ContinuousTrait : public Trait
{
    float max_;
    float min_;
    void InitializePhenospace();
    void InitializeGenevectors();
  public:
    ContinuousTrait() = default;
    ContinuousTrait(std::string name, std::string genes, std::vector<std::string> phenotypes, float max, float min);
    std::string ValueToPhenotype(Phenovector trait_vec);
    float ApplyWeighting(PhenotypeWeights weights, Phenovector trait_vec);
};


class BinaryTrait : public DiscreteTrait
{
    const int DominantPhenotypeIndex_ = 0;
    const int RecessivePhenotypeIndex_ = 1;
    void InitializeGenevectors();
public:
    std::string ValueToPhenotype(Phenovector trait_vec);
    BinaryTrait(std::string name, std::string genes, std::vector<std::string> phenotypes);
};

std::ostream &operator<< (std::ostream &stream, const Trait &obj);

struct TraitWeighting
{
   std::vector<float> weights_;
   TraitWeighting(std::vector<float> weights);
   float operator()(Trait &trait, const Genome &genome);
};


#endif // TRAIT_H

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
    void InitializePhenospace();
    Trait(std::string name, std::string genes, std::vector<std::string> phenotypes);
    const std::string get_name() const;
    const std::string& get_genes() const;
    Phenovector CumulativePhenovector(const Genome &genome);
    int ValueToPhenotypeDimension(Phenovector trait_vec);
    std::string ValueToPhenotype(Phenovector trait_vec);
    std::string ValueToPhenotype(std::shared_ptr<Creature> c);
    std::pair<std::vector<float>, std::vector<float>> CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures);
    std::map<std::string, int> CalculatePhenotypeStatistics(const std::vector<std::shared_ptr<Creature>> creatures);
    std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights);
};

class DiscreteTrait : public Trait
{
    void InitializeGenevectors();
public:
    DiscreteTrait() = default;
    DiscreteTrait(std::string name, std::string genes, std::vector<std::string> phenotypes);
    std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights);

};


class ContinuousTrait : public Trait
{
    float max_;
    float min_;
    void InitializeGenevectors();
  public:
    ContinuousTrait() = default;
    ContinuousTrait(std::string name, std::string genes, std::vector<std::string> phenotypes, float max, float min);
    float CalculateValue(const Genome &genome);
    float CalculateNormalizedValue(const Genome &genome);
    std::string ValueToPhenotype(Phenovector trait_vec);
    std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights);

};


class BinaryTrait : public DiscreteTrait
{
    void InitializeGenevectors();
public:
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

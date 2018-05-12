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
    std::vector<std::shared_ptr<TraitWeighting>> weights_;
    std::string name_;
    std::string gene_codes_;
public:
    const std::vector<std::string> phenotypes_;
    Trait() = default;
    virtual ~Trait() {}
    Trait(std::string name, std::string genes, std::vector<std::string> phenotypes);
    const std::string get_name() const;
    const std::string& get_genes() const;
    virtual float CalculateValue(const Genome &genome)=0;
    /**
     * @brief Determine the phenotype that the given value corresponds to.
     * @param value
     * @return
     */
    virtual std::string ValueToPhenotype(float value)=0;
    std::pair<float, float> CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures);
    virtual std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights)=0;
};

class ContinuousTrait : public Trait
{
    float max_;
    float min_;
  public:
    ContinuousTrait() = default;
    ContinuousTrait(std::string name, std::string genes, std::vector<std::string> phenotypes, float max, float min);
    float CalculateValue(const Genome &genome);
    float CalculateNormalizedValue(const Genome &genome);
    std::string ValueToPhenotype(float value);
    std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights);

    friend class ContinuousTraitWeighting;
};


class DiscreteTrait : public Trait
{
  public:
    DiscreteTrait() = default;
    DiscreteTrait(std::string name, std::string genes, std::vector<std::string> phenotypes);
    float CalculateValue(const Genome &genome);
    std::string ValueToPhenotype(float value);
    std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights);

    friend class DiscreteTraitWeighting;
};

class BinaryTrait : public DiscreteTrait
{
public:
    BinaryTrait(std::string name, std::string genes, std::vector<std::string> phenotypes);

};

std::ostream &operator<< (std::ostream &stream, const Trait &obj);

struct TraitWeighting
{
   std::vector<float> weights_;
   TraitWeighting(std::vector<float> weights);
   virtual ~TraitWeighting() {}
   virtual float CalculateValue(Trait &trait, const Genome &genome)=0;
};

struct ContinuousTraitWeighting : public TraitWeighting
{
    ContinuousTraitWeighting(std::vector<float> weights);
    float CalculateValue(Trait &trait, const Genome &genome);
};

struct DiscreteTraitWeighting : public TraitWeighting
{
    DiscreteTraitWeighting(std::vector<float> weights);
    float CalculateValue(Trait &trait, const Genome &genome);
};


struct Phenotype
{
  const std::string name_;
  Phenotype(std::string name);
  ~Phenotype();
};

struct ContinuousTraitPhenotype : Phenotype
{
    const float value_min_, value_max_;
    ContinuousTraitPhenotype(std::string name, float min, float max);
};


struct DiscreteTraitPhenotype : Phenotype
{
    const int phenotype_index_;
    DiscreteTraitPhenotype(std::string name, int phenotype_index);
};

#endif // TRAIT_H

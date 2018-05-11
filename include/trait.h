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
    const int n_phenotypes_;
    Trait() = default;
    virtual ~Trait() {}
    Trait(std::string name, std::string genes, int n_phenotypes);
    const std::string get_name() const;
    const std::string& get_genes() const;
    virtual float CalculateValue(const Genome &genome)=0;

    /**
     * @brief Determine the phenotype index that the given value corresponds to.
     * @param value
     * @return
     */
    std::pair<float, float> CalculateStatistics(const std::vector<std::shared_ptr<Creature>> creatures);
    virtual std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights)=0;
};

class ContinuousTrait : public Trait
{
    float max_;
    float min_;
  public:
    ContinuousTrait() = default;
    ContinuousTrait(std::string name, std::string genes, int n_phenotypes, float max, float min);
    float CalculateValue(const Genome &genome);
    float CalculateNormalizedValue(const Genome &genome);
    std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights);

    friend class ContinuousTraitWeighting;
};


class DiscreteTrait : public Trait
{
  public:
    DiscreteTrait() = default;
    DiscreteTrait(std::string name, std::string genes, int n_phenotypes);
    float CalculateValue(const Genome &genome);
    std::weak_ptr<TraitWeighting> MakeWeighting(std::vector<float> weights);

    friend class DiscreteTraitWeighting;
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

#endif // TRAIT_H

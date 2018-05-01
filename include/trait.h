#ifndef TRAIT_H
#define TRAIT_H

#include <iostream>
#include <memory>
#include <cmath>
#include "common.h"
#include "genetics.h"
#include "creature.h"



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
#endif // TRAIT_H
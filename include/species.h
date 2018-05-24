#ifndef __Species
#define __Species

#include <vector>
#include <map>
#include <ostream>
#include "common.h"
#include "genetics.h"
#include <memory>


class Species
{
    int n_chromosome_pairs_;
    int genotype_length_;
    std::string name_;
    std::vector< std::shared_ptr<Creature> > creatures_;
    int alive_=0;
    int deceased_=0;
    int max_offspring_;
    int life_expectancy_days_;
public:
    Species() = default;
    Species(std::string name, int genotype_length, int n_chromosome_pairs, int max_offspring, int life_expectancy_days);
    void InitializeCreatures(int n_males, int n_females);
    std::shared_ptr<Creature> AddCreature(Sex, Genome);
    const std::string GetName() const;
    const int GetNChromosomePairs() const;
    std::vector<std::shared_ptr<Creature>> &GetCreatures();
    const int GetAlivePopulation() const;
    const int GetDeceasedPopulation() const;

    friend bool operator==(const Species &lhs, const Species &rhs);
    friend bool operator!=(const Species &lhs, const Species &rhs);
    friend class Creature;
    friend class Ecosystem;
};



std::ostream &operator<< (std::ostream &stream, const Species &obj);


#endif

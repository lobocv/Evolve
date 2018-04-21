#ifndef __Creature
#define __Creature

#include <vector>
#include <map>
#include <ostream>
#include "common.h"

class Creature {
    std::string species_;
    std::vector<ChromosomePair> chromosomes_;
    int n_chromosome_pairs_;
    int id_;
public:
    Creature() = default;
    Creature(std::string species, int n_chromosome_pairs, int id);    
    std::vector<ChromosomePair> get_chromosomes() const;
    const int get_id() const;
    const std::string get_species() const;
};


class Species {

    int n_chromosome_pairs_;
    int genotype_length_;
    std::string name_;
    std::vector<Creature> creatures_;
    int alive_=0;
    int deceased_=0;
    
public:
    Species() = default;
    Species(std::string name, int genotype_length, int n_chromosome_pairs);
    const std::string get_name() const;
    void AddCreatures(int number);
    int get_n_chromosome_pairs();
    const std::vector<Creature> &get_creatures() const;
    
};

// Singleton
class CreatureRegistry {
public:
    static CreatureRegistry& GetRegistry();
    std::map<std::string, Species> species_;
    void GenerateCreatures(std::string species_name, int number);
private:
    CreatureRegistry(){};
    ~CreatureRegistry(){};
    CreatureRegistry(CreatureRegistry &c){};
    void operator=(CreatureRegistry &c){};
};

#endif
#ifndef __Creature
#define __Creature

#include <vector>
#include <map>
#include <ostream>
#include "common.h"

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
    void AddCreatures(int number);
    const std::string get_name() const;
    const int get_n_chromosome_pairs() const;
    const std::vector<Creature> &get_creatures() const;
    const int get_alive_population() const;
    const int get_deceased_population() const;
    
};


class Creature {
    Species& species_;
    std::vector<ChromosomePair> chromosomes_;
    int id_;
public:
    Creature() = default;
    Creature(Species& species);
    void Reproduce(const Creature& c1, const Creature& c2);    
    friend void Species::AddCreatures(int);
    const std::vector<ChromosomePair>& get_chromosomes() const;
    const int get_id() const;
    const Species& get_species() const;
    void print() const;
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
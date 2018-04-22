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
    void InitializeCreatures(int number);
    void AddCreature(Genome);
    const std::string get_name() const;
    const int get_n_chromosome_pairs() const;
    const std::vector<Creature> &get_creatures() const;
    const int get_alive_population() const;
    const int get_deceased_population() const;
    
};


class Creature {
    Species& species_;
    Genome chromosomes_;
    int id_;
public:
    Creature() = default;
    Creature(Species& species);
    void Reproduce(const Creature& c1, const Creature& c2);    
    friend void Species::AddCreature(Genome);
    friend void Species::InitializeCreatures(int);
    const Genome& get_chromosomes() const;
    const int get_id() const;
    const Species& get_species() const;
    void print() const;
};


// Singleton
class SpeciesRegistry {
public:
    static SpeciesRegistry& GetRegistry();
    std::map<std::string, Species> species_;
    void RegisterSpecies(std::string species_name, int chromosome_length, int n_chromosome_pairs, int initial_population);
private:
    SpeciesRegistry(){};
    ~SpeciesRegistry(){};
    SpeciesRegistry(SpeciesRegistry &c){};
    void operator=(SpeciesRegistry &c){};
};

#endif
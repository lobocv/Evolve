#ifndef __Gene
#define __Gene

#include <vector>
#include <map>
#include <ostream>
#include "common.h"

class Gene {
    char name;
public:
    Gene() = default;
    Gene(char);
    char get_name() const;
};

class Chromosome {
    int id;    
public:
    const int ASD=6;
    Chromosome() = default;
    Chromosome(std::string);
    std::vector<Gene> genes;
    static std::pair<Chromosome, Chromosome> MakePair(std::string);
    friend std::ostream &operator<< (std::ostream &ostream, const Chromosome &c);
};


class Creature {
    std::string species_;
    std::vector<std::pair<Chromosome,Chromosome>> chromosomes_;
    int n_chromosome_pairs_;
    int id_;
public:
    Creature() = default;
    Creature(std::string species, int n_chromosome_pairs, int id);
    std::vector<std::pair<Chromosome,Chromosome>> get_chromosomes() const;
    int get_id();
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
    std::string get_name();
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
#include "creature.h"
#include "genetics.h"
#include "common.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

Creature::Creature(std::string species, int n_chromosome_pairs, int id) : species_(species), n_chromosome_pairs_(n_chromosome_pairs), id_(id) {
    std::string genecodes;
    for (int ii=0; ii < N_GENES; ii++) {
        genecodes.push_back('A' + ii);
    }    
    for (int ii=0; ii < n_chromosome_pairs_; ii++) {
        auto chromosome_pair = Chromosome::MakePair(genecodes);
        chromosomes_.push_back(chromosome_pair);
    }
};

int Creature::get_id() const { return id_;};

std::vector<std::pair<Chromosome,Chromosome>> Creature::get_chromosomes() const {
    return chromosomes_; 
};

Species::Species(std::string name, int genotype_length, int n_chromosome_pairs) 
    : name_(name), genotype_length_(genotype_length), n_chromosome_pairs_(n_chromosome_pairs) {};

std::string Species::get_name() {return name_;};

int Species::get_n_chromosome_pairs() {return n_chromosome_pairs_;};

const std::vector<Creature>& Species::get_creatures() const {
    
    return creatures_;};

void Species::AddCreatures(int number) {
    int id = creatures_.size();
    for (int ii=0; ii < number; ii++) {
        creatures_.push_back(Creature(name_, n_chromosome_pairs_, id));
        id++;
        std::cout << "Size"<< get_creatures().size() << std::endl;
    }
};

CreatureRegistry& CreatureRegistry::GetRegistry() {
    static CreatureRegistry registry;
    return registry;
};

void CreatureRegistry::GenerateCreatures(std::string species_name, int number) {
    CreatureRegistry &registery = CreatureRegistry::GetRegistry();
    Species &species = registery.species_[species_name];
    species.AddCreatures(number);
};

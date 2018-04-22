#include "creature.h"
#include "genetics.h"
#include "common.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

/*
    Creature
*/
Creature::Creature(Species& species) : species_(species) {};

void Creature::print() const {
    int chromo_num=0;
    for (auto &chromosome_pair: get_genome()){
        std::cout << "Chromosome Pair #" << (chromo_num+1) << ":" << std::endl;
        std::cout << chromosome_pair.first << std::endl;
        std::cout << chromosome_pair.second << std::endl;
        chromo_num++;
    }
};

void Creature::Reproduce(const Creature& other){
    const int N_offspring = rand() % (MAX_OFFSPRING+1);
    Chromosome c1_chromo1, c1_chromo2, c2_chromo1, c2_chromo2, child_chromo1, child_chromo2;
    // TODO: Check creatures are of the same species (allowed to mate)
    // TODO: Check sex of the creatures are opposite
    // TODO: Check creatures are of reproductive age.
    Species &species = this->get_species();

    Genome c1_chromo_pairs = this->get_genome();
    Genome c2_chromo_pairs = other.get_genome();
    Genome child_genome;

    // Iterate through the chromosome pairs. If both creatures are the same 
    // species, they necessarily have the same number of chromosome pairs
    for (int chromo_pair_num=0; chromo_pair_num < c1_chromo_pairs.size(); chromo_pair_num++) {
        // Get the parents chromosomes
        c1_chromo1 = c1_chromo_pairs[chromo_pair_num].first;
        c1_chromo2 = c1_chromo_pairs[chromo_pair_num].second;
        c2_chromo1 = c2_chromo_pairs[chromo_pair_num].first;
        c2_chromo2 = c2_chromo_pairs[chromo_pair_num].second;
        // Role the dice and take one chromosome from each parent to create a new
        
        child_chromo1 = (std::rand() % 2) ? c1_chromo1 : c2_chromo1;
        child_chromo2 = (std::rand() % 2) ? c1_chromo2 : c2_chromo2;
        child_genome.push_back(std::make_pair(child_chromo1, child_chromo2));
    }

    species.AddCreature(child_genome);

};

const int Creature::get_id() const { return id_;};
Species &Creature::get_species() const { return species_;};
const Genome &Creature::get_genome() const { return genome_;}

/*
    Species
*/
Species::Species(std::string name, int genotype_length, int n_chromosome_pairs) 
    : name_(name), genotype_length_(genotype_length), n_chromosome_pairs_(n_chromosome_pairs) {};

const std::string Species::get_name() const {return name_;};
const int Species::get_alive_population() const { return alive_;};
const int Species::get_deceased_population() const { return deceased_;};
const int Species::get_n_chromosome_pairs() const{return n_chromosome_pairs_;};

const std::vector<Creature>& Species::get_creatures() const {
    return creatures_;};

void Species::AddCreature(Genome genome) {
    Creature creature(*this);
    for (int chromo_num=0; chromo_num < this->get_n_chromosome_pairs(); chromo_num++) {
        creature.genome_ = genome;
        creature.id_ = get_alive_population() + get_deceased_population();
    }
    // Add the creature to the list of recorded creatures in the species
    creatures_.push_back(creature);
    alive_++;
}

void Species::InitializeCreatures(int number) {
    std::string gene_sequence;
    for (int chromo_num=0; chromo_num < genotype_length_; chromo_num++) {
        gene_sequence.push_back('A' + chromo_num);
    }        

    // For each creature
    for (int creature_num=0; creature_num < number; creature_num++) {
        Creature creature(*this);
        Genome genome;
        // Create all the chromosomes for the new creature
        for (int chromo_num=0; chromo_num < this->get_n_chromosome_pairs(); chromo_num++) {
            auto chromosome_pair = Chromosome::MakeRandomPair(gene_sequence);
            genome.push_back(chromosome_pair);
        }
        // Add the creature to the species
        AddCreature(genome);
    }
};


/*
    Creature Registry (Singleton)
*/

SpeciesRegistry& SpeciesRegistry::GetRegistry() {
    static SpeciesRegistry registry;
    return registry;
};

Species& SpeciesRegistry::RegisterSpecies(std::string species_name, int chromosome_length, int n_chromosome_pairs, int initial_population) {
    SpeciesRegistry &registery = SpeciesRegistry::GetRegistry();
    registery.species_[species_name] = Species(species_name, chromosome_length, n_chromosome_pairs);
    registery.species_[species_name].InitializeCreatures(initial_population);
    return registery.species_[species_name];
};

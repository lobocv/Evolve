#include "common.h"
#include "creature.h"
#include "genetics.h"
#include "species.h"
#include "trait.h"
#include "ecosystem.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

/*
    Creature
*/
Creature::Creature(Species& species, Sex sex) : species_(species), sex_(sex){}

void Creature::print() const
{
    int chromo_num=0;
    for (auto &chromosome_pair: get_genome()){
        std::cout << "Chromosome Pair #" << (chromo_num+1) << ":" << std::endl;
        std::cout << chromosome_pair.first << std::endl;
        std::cout << chromosome_pair.second << std::endl;
        chromo_num++;
    }
}

Creature::~Creature()
{
    species_.deceased_++;
    species_.alive_--;
    std::cout << *this << " has died." << std::endl;
}

std::vector<std::shared_ptr<Creature>> Creature::Reproduce(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2)
{
    Ecosystem &eco = Ecosystem::GetEcosystem();
    Species &species = creature1->get_species();
    const int N_offspring = rand() % (species.max_offspring_+1);

    // Determine which creature is male (father) and which is female (mother)    
    // Throw an error if trying to mate two of the same sex.
    std::shared_ptr<Creature>& father = (creature1->get_sex() == Male) ? creature1 : creature2;
    std::shared_ptr<Creature>& mother = (creature2->get_sex() == Male) ? creature1 : creature2;
    if (mother->get_sex() == father->get_sex()) {throw(CannotProcreateError());}

    // TODO: Check creatures are of reproductive age.

    Genome male_chromo_pairs = creature1->get_genome();
    Genome female_chromo_pairs = creature2->get_genome();

    std::vector<std::shared_ptr<Creature>> offspring;
    for (int child_num=0; child_num < N_offspring; child_num++)
    {
        Genome child_genome;
        Chromosome child_chromo1, child_chromo2;
        // Iterate through the chromosome pairs. If both creatures are the same
        // species, they necessarily have the same number of chromosome pairs
        for (int chromo_pair_num=0; chromo_pair_num < male_chromo_pairs.size(); chromo_pair_num++) {
            // Get the parents chromosomes
            Chromosome& male_chromo1 = male_chromo_pairs[chromo_pair_num].first;
            Chromosome& male_chromo2 = male_chromo_pairs[chromo_pair_num].second;
            Chromosome& female_chromo1 = female_chromo_pairs[chromo_pair_num].first;
            Chromosome& female_chromo2 = female_chromo_pairs[chromo_pair_num].second;
            // Role the dice and take one chromosome from each parent to create a new

            child_chromo1 = (FlipCoin()) ? male_chromo1 : male_chromo2;
            child_chromo2 = (FlipCoin()) ? female_chromo1 : female_chromo2;
            child_genome.push_back(std::make_pair(child_chromo1, child_chromo2));
        }

        // Add the creature to the species
        Sex sex_of_child = Sex(FlipCoin());
        std::shared_ptr<Creature> child = species.AddCreature(sex_of_child, child_genome);
        child->birth_date = eco.get_day();
        child->mother_ = mother;
        child->father_ = father;
        offspring.push_back(child);
    }
    return offspring;
}

const int Creature::get_id() const { return id_;}
Species &Creature::get_species() const { return species_;};
const Genome &Creature::get_genome() const { return genome_;}
const Sex Creature::get_sex() const { return sex_;}
const int Creature::get_birth_date() const {return birth_date;}
std::shared_ptr<Creature> Creature::get_father() const {return father_.lock();}
std::shared_ptr<Creature> Creature::get_mother() const {return mother_.lock();}
void Creature::print_traits()
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    std::cout << "Trait Values" << std::endl;
    for (auto trait: ecosystem.traits_) {
        float value = trait.second->CalculateValue(genome_);
    }
}


std::ostream &operator<<(std::ostream &stream, const Creature &obj)
{
    stream << "Creature (" << "#" << obj.get_id() << ",";
    (obj.get_sex() == Male) ? (stream << "M") : (stream << "F");
    stream << "," << obj.get_species() << ")";
    return stream;
}

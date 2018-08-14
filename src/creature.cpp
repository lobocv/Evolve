#include "common.h"
#include "creature.h"
#include "genetics.h"
#include "species.h"
#include "trait.h"
#include "ecosystem.h"
#include <ctype.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <spdlog/spdlog.h>

/*
    Creature
*/
Creature::Creature(Species& species, Sex sex) : species_(species), sex_(sex){}

void Creature::Print() const
{
    int chromo_num=0;
    for (auto &chromosome_pair: GetGenome()){
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
    auto logger = spdlog::get("logger");
    std::stringstream s;
    s << *this;
    logger->debug("{0} has died.", s.str());
}

/**
 * @brief Attempt to have two creatures reproduce. The creatures will not reproduce if they are the same sex or not the same species.
 *
 * If the creatures are successful at reproducing, they will make a random number of offspring (limted by the species).
 * Each offspring will take one random chromosome from each parent, for each chromsome pair. The created creatures are added
 * to the creatures list in the species.
 * @param creature1
 * @param creature2
 * @return
 */
std::vector<std::shared_ptr<Creature>> Creature::Reproduce(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2)
{
    Ecosystem &eco = Ecosystem::GetEcosystem();
    Species &species = creature1->GetSpecies();
    const int N_offspring = rand() % (species.max_offspring_+1);

    // Determine which creature is male (father) and which is female (mother)    
    // Throw an error if trying to mate two of the same sex.
    std::shared_ptr<Creature>& father = (creature1->GetSex() == Male) ? creature1 : creature2;
    std::shared_ptr<Creature>& mother = (creature2->GetSex() == Male) ? creature1 : creature2;
    if (mother->GetSex() == father->GetSex()) {throw(CannotProcreateError("Creatures are of the same sex."));}
    if (mother->GetSpecies() != father->GetSpecies()) {throw(CannotProcreateError("Creatures are not of the same species."));}
    // TODO: Check creatures are of reproductive age.

    Genome male_chromo_pairs = creature1->GetGenome();
    Genome female_chromo_pairs = creature2->GetGenome();

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
        child->birth_date = eco.GetDay();
        child->mother_ = mother;
        child->father_ = father;
        offspring.push_back(child);
    }
    return offspring;
}


const int Creature::GetID() const { return id_;}
Species &Creature::GetSpecies() const { return species_;}
const Genome &Creature::GetGenome() const { return genome_;}
const Sex Creature::GetSex() const { return sex_;}
const int Creature::GetBirthDate() const {return birth_date;}
std::shared_ptr<Creature> Creature::GetFather() const {return father_.lock();}
std::shared_ptr<Creature> Creature::GetMother() const {return mother_.lock();}
void Creature::PrintTraits()
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    std::cout << *this << " is ";
    for (auto trait: ecosystem.traits_) {
        auto value = trait.second->CumulativePhenovector(genome_);
        auto phenotype = trait.second->ValueToPhenotype(value);
        std::cout << phenotype << "(" << value << "), ";
    }
    std::cout << std::endl;
}


std::ostream &operator<<(std::ostream &stream, const Creature &obj)
{
    stream << "Creature (" << "#" << obj.GetID() << ",";
    (obj.GetSex() == Male) ? (stream << "M") : (stream << "F");
    stream << "," << obj.GetSpecies() << ")";
    return stream;
}

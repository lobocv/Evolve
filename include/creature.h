#ifndef __Creature
#define __Creature

#include <vector>
#include <map>
#include <ostream>
#include "common.h"
#include "genetics.h"
#include <memory>


class Creature {
    Species& species_;
    Genome genome_;
    Sex sex_;
    int id_;
    int birth_date=0;
    std::weak_ptr<Creature> mother_;
    std::weak_ptr<Creature> father_;
    Creature() = default;
public:
    ~Creature();
    Creature(Species& species, Sex sex);
    static std::vector<std::shared_ptr<Creature>> Reproduce(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2);
    const Genome& get_genome() const;
    const int get_id() const;
    const Sex get_sex() const;
    const int get_birth_date() const;
    std::shared_ptr<Creature> get_father() const;
    std::shared_ptr<Creature> get_mother() const;
    Species& get_species() const;
    void print() const;
    void print_traits();

    friend class Species;
};

std::ostream &operator<< (std::ostream &stream, const Creature &obj);

#endif

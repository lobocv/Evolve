#ifndef __EVOLVE_COMMON
#define __EVOLVE_COMMON

#include <utility>
#include <vector>
#include <cstdlib>
#include <map>

const int MAX_OFFSPRING = 3;

// Forward Declarations
class Chromosome;
class Creature;
class Species;
class Gene;

// Typedefs

typedef std::pair<Chromosome, Chromosome> ChromosomePair;
typedef std::vector<ChromosomePair> Genome;
typedef std::map<char, Gene> GeneSequence;

enum Sex{Male, Female};
enum GeneType{Dominant, Recessive};

// Exceptions

class CannotProcreateError {};

inline bool FlipCoin() {return std::rand() % 2;}

#endif

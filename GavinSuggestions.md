##class Ecosystem 
- responsible for holding all information about SPECIES that live in the ecosystem
- **method: registerSpecies** registers a species. Creates a new shared_ptr<Species> and places it in a std::map

## class Species
- responsible for **constructing** all creatures of that specific species.
- **method: std::shared_ptr<Creature> createCreture(...)**
- stores the shared pointer of the creature created in it's own container
- sets the creature's parent species on construction

## class Creature
- has private constructor, and Species class is set as **friend** so that only Species can call new creature()
- contains member:  std::shared_ptr<Species> m_parent_species;
- contains member:  std::weak_ptr<Creature>  m_mother, m_father;
- contains static member function called:
  - static std::shared_ptr<Creature> reproduce( std::shared_ptr<Creature> & parent1, std::shared_ptr<Creature> & parent2 )
  - calls  m_parent_species->createCreature( )
  - automatically sets the parent's appropriatly


```C++

class Ecosystem
{
    std::map<std::string, std::shared_ptr<Species> > m_Species;

    public: 

        std::shared_ptr<Species> registerSpecies( ... ); // registers a species and stores it in the map
    
}

class Species
{
    std::weak_ptr<Ecosystem>                 m_ParentSystem;
    std::vector< std::shared_ptr<Creature> > m_Creatures;

    public:
        std::shared_ptr<Creature> createCreature( ... ); creates a creature 

        
}

class Creature
{
    std::weak_ptr<Creature> m_mother, m_father;
    std::weak_ptr<Species>  m_species; 

    private:
    Creature() {} // do not allow anyone else to create creatures expect the Species class

    public:
        // creates a new creature by mating two parents
        static std::shared_ptr<Creature> reproduce( std::shared_ptr<Creature> & p1, std::shared_ptr<Creature> & p2);

    friend class Species;

}

``` 
 

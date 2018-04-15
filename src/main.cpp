#include <SFML/Graphics.hpp>

#include <iostream>
#include <stdlib.h> 
#include <functional>
#include <memory>

#include "EvolveConfig.h"


class Test {

    int id;
    std::string name;

public:
    Test(): id(0), name("calvin") {}

    friend std::ostream& operator<<(std::ostream& out, const Test &test) {
        out << test.id << ":" << test.name;
    }
};

int main()
{

    std::cout << "Major Version: " << Evolve_VERSION_MAJOR << std::endl;
    std::cout << "Minor Version: " << Evolve_VERSION_MINOR << std::endl;

    Test test1 = Test();
    std::cout << test1 << std::endl;

    return 0;
}

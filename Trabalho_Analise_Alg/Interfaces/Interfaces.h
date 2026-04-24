#ifndef INTERFACES
#define INTERFACES

#include <vector>
#include <string>

class ISortAlgorithm {
    public:
        virtual std::vector<int> sort(std::vector<int>)=0;
        virtual std::string generateRelatory()=0;
}; 

#endif
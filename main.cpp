#include "reducer.hpp"

int main()
{
    ReducerDelegate reducer("reducer.csv");
    reducer.makeMeasures({10, 100, 1000, 100000, 1000000, 10000000, 50000000}, 30);
    return 0;
}
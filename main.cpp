#include "reducer.hpp"
#include "transposer.hpp"

int main()
{
    ReducerDelegate reducer("reducer.csv");
    reducer.makeMeasures({10, 100, 1000, 100000, 1000000, 10000000, 50000000}, 30);

    TransposerDelegate transposer("transposer.csv");
    transposer.makeMeasures({10, 100, 1000, 10000}, 10);

    return 0;
}
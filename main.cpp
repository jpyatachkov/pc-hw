#include "reducer.hpp"
#include "transposer.hpp"

int main()
{
    ReducerDelegate reducer("reducer_latest.csv");
    reducer.makeMeasures({10, 100, 1000, 100000, 1000000, 10000000, 50000000}, 10);

    TransposerDelegate transposer("transposer_latest.csv");
    transposer.makeMeasures({10, 100, 1000, 100000, 1000000, 10000000, 50000000}, 10);

    return 0;
}
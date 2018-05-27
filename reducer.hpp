#ifndef GOLOVKOV_HW_REDUCER_HPP
#define GOLOVKOV_HW_REDUCER_HPP

#include <functional>
#include <numeric>
#include <random>
#include <vector>

#include "measuredelegate.hpp"
#include "timeutils.hpp"

using std::cout;


template <class T>
static T sumSequential(const std::vector<T > &array)
{
    return std::accumulate(array.begin(), array.end(), 0);
}

template <class T>
static T sumParallel(const std::vector<T > &array)
{
    auto size = array.size();
    auto sum = T(0);

#pragma omp parallel for reduction (+:sum)
    for (decltype(size) i = 0; i < size; ++i)
        sum += array[i];

    return sum;
}

class ReducerDelegate : public MeasureDelegate
{
public:

    explicit ReducerDelegate(const std::string &outFilePath);

    void makeMeasures(std::vector<std::size_t > arraySizeVariants,
                      std::size_t measuresCount);
};

ReducerDelegate::ReducerDelegate(const std::string &outFilePath)
        : MeasureDelegate(outFilePath)
{
    ofs << "Размер массива;"
        << "Время последовательного алгоритма (нс);"
        << "Время параллельного алгоритма (нс);"
        << "Ускорение (раз);" << std::endl;
}

void ReducerDelegate::makeMeasures(std::vector<std::size_t> arraySizeVariants,
                                   std::size_t measuresCount)
{
    using DataType = int;

    std::default_random_engine engine;
    std::uniform_int_distribution<DataType > distribution(-1000, 1000);
    auto dice = std::bind(distribution, engine);

    for (const auto &size : arraySizeVariants) {
        cout << "Размер массива " << size << std::endl;

        std::vector<DataType > array;
        array.reserve(size);
        for (auto i = 0; i < size; ++i)
            array.push_back(dice());

        long avgSequential = 0;
        long avgParallel = 0;

        for (auto i = 0; i < measuresCount; ++i) {
            auto sequentialTimePoint = getTime();
            sumSequential(array);
            avgSequential += timeDiff(sequentialTimePoint);

            auto parallelTimePoint = getTime();
            sumParallel(array);
            avgParallel += timeDiff(parallelTimePoint);
        }

        avgSequential /= measuresCount;
        avgParallel /= measuresCount;

        cout << "Последовательный алгоритм: " << avgSequential
             << std::endl
             << "Параллельный алгоритм: " << avgParallel
             << std::endl
             << "Ускорение: " << avgSequential / static_cast<double >(avgParallel)
             << std::endl << std::endl;

        ofs << size << ";"
            << avgSequential << ";"
            << avgParallel << ";"
            << avgSequential / static_cast<double >(avgParallel) << std::endl;
    }
}

#endif //GOLOVKOV_HW_REDUCER_HPP

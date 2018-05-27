#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#define N_THREADS 4

using std::cout;


template <class T>
T sumSequential(const std::vector<T > &array) {
    return std::accumulate(array.begin(), array.end(), 0);
}

template <class T>
T sumParallel(const std::vector<T > &array) {
    auto size = array.size();
    auto sum = T(0);

#pragma omp parallel for reduction (+:sum)
    for (decltype(size) i = 0; i < size; ++i)
        sum += array[i];

    return sum;
}

//template <class T>
//void transposeSequential(std::vector<std::vector<T > > &matrix) {
//    for (std::vector::size_type i = 0; i < matrix.size(); ++i)
//        for (std::vector::size_type j = 0; j < matrix[i].size(); ++j)
//            std::swap(matrix[i][j], matrix[j][i]);
//}
//
//template <class T>
//void transposeParallel(std::vector<std::vector<T > > &matrix) {
//
//}

static inline auto getTime() {
    return std::chrono::steady_clock::now();
}

template <class T>
static inline auto timeDiff(T lastTimePoint) {
    return std::chrono::duration_cast<std::chrono::nanoseconds >(getTime() - lastTimePoint).count();
}

int main() {
    using DataType = int;

    std::default_random_engine engine;
    std::uniform_int_distribution<DataType > distribution(-1000, 1000);
    auto dice = std::bind(distribution, engine);

    static auto constexpr ITEMS_COUNT = 1000000;

    std::vector<DataType > array;
    array.reserve(ITEMS_COUNT);
    for (auto i = 0; i < ITEMS_COUNT; ++i)
        array.push_back(dice());

    static auto constexpr MEASURES_COUNT = 10;

    long avgSequential = 0;
    long avgParallel = 0;

    for (auto i = 0; i < MEASURES_COUNT; ++i) {
        auto sequentialTimePoint = getTime();
        sumSequential(array);
        avgSequential += timeDiff(sequentialTimePoint);

        auto parallelTimePoint = getTime();
        sumParallel(array);
        avgParallel += timeDiff(parallelTimePoint);
    }

    cout << "Последовательный алгоритм: " << avgSequential / MEASURES_COUNT
         << std::endl
         << "Параллельный алгоритм: " << avgParallel / MEASURES_COUNT
         << std::endl;

    return 0;
}
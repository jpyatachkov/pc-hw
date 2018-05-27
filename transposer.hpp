#ifndef GOLOVKOV_HW_TRANSPOSER_HPP
#define GOLOVKOV_HW_TRANSPOSER_HPP

#include <cassert>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include "measuredelegate.hpp"
#include "timeutils.hpp"

using std::cout;


template <class T>
static void transposeSequential(std::vector<std::vector<T > > &matrix)
{
    assert(matrix.size() == matrix.at(0).size());

    auto beginIdx = 1;
    for (auto i = 0; i < matrix.size() - 1; ++i) {
        for (auto j = beginIdx; j < matrix.size(); ++j)
            std::swap(matrix[i][j], matrix[j][i]);

        ++beginIdx;
    }
}

template <class T>
static void transposeParallel(std::vector<std::vector<T > > &matrix)
{
    assert(matrix.size() == matrix.at(0).size());

    auto beginIdx = 1;
#pragma omp parallel for num_threads(4)
    for (auto i = 0; i < matrix.size() - 1; ++i) {
#pragma omp parallel for num_threads(4)
        for (auto j = beginIdx; j < matrix.size(); ++j)
            std::swap(matrix[i][j], matrix[j][i]);

        ++beginIdx;
    }
}

class TransposerDelegate : public MeasureDelegate
{
public:

    explicit TransposerDelegate(const std::string &outFilePath);

    void makeMeasures(std::vector<std::size_t> matrixSizeVariants,
                      std::size_t measuresCount);
};

TransposerDelegate::TransposerDelegate(const std::string &outFilePath)
        : MeasureDelegate(outFilePath)
{
    ofs << "Размер массива;"
        << "Время последовательного алгоритма (нс);"
        << "Время параллельного алгоритма (нс);"
        << "Ускорение (раз);" << std::endl;
}

void TransposerDelegate::makeMeasures(std::vector<std::size_t> matrixSizeVariants,
                                      std::size_t measuresCount)
{
    using DataType = int;

    std::default_random_engine engine;
    std::uniform_int_distribution<DataType > distribution(-1000, 1000);
    auto dice = std::bind(distribution, engine);

    for (const auto &size : matrixSizeVariants) {
        cout << "Размер матрицы " << size << "X" << size << std::endl;

        std::vector<std::vector<DataType > > matrix;
        matrix.reserve(size);
        for (auto i = 0; i < size; ++i) {
            std::vector<DataType > row;
            row.reserve(size);
            for (auto j = 0; j < size; ++j)
                row.push_back(dice());

            matrix.push_back(std::move(row));
        }

        long avgSequential = 0;
        long avgParallel = 0;

        for (auto i = 0; i < measuresCount; ++i) {
            auto sequentialTimePoint = getTime();
            transposeSequential(matrix);
            avgSequential += timeDiff(sequentialTimePoint);

            auto parallelTimePoint = getTime();
            transposeParallel(matrix);
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

#endif //GOLOVKOV_HW_TRANSPOSER_HPP

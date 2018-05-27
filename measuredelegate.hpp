#ifndef PC_HW_MEASUREDELEGATE_HPP
#define PC_HW_MEASUREDELEGATE_HPP

#include <fstream>
#include <iostream>

using std::cerr;


class MeasureDelegate
{
public:

    explicit MeasureDelegate(const std::string &outFilePath);

    virtual ~MeasureDelegate();

protected:

    std::fstream ofs;
};

MeasureDelegate::MeasureDelegate(const std::string &outFilePath)
        : ofs(outFilePath, std::ios_base::out | std::ios_base::trunc)
{
    if (!ofs.is_open()) {
        cerr << "Невозможно открыть файл " << outFilePath << std::endl;
        throw std::exception();
    }
}

MeasureDelegate::~MeasureDelegate()
{
    if (ofs.is_open())
        ofs.close();
}

#endif //PC_HW_MEASUREDELEGATE_HPP

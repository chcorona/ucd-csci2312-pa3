//
// Created by Christopher Corona on 3/24/2016.
//

#include "Exceptions.h"
#include "Cluster.h"
#include <sstream>

namespace Clustering
{


    OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r)
    {
        __name = "OutOfBoundsEx";
        __current = c;
        __rhs = r;
    }

    unsigned int OutOfBoundsEx::getCurrent() const
    {
        return __current;
    }

    int OutOfBoundsEx::getRhs() const
    {
        return __rhs;
    }

    std::string OutOfBoundsEx::getName() const
    {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx &ex)
    {
        os << "OutOfBoundsEx (" << "current= " << ex.__current << ","
        << "rhs = " << ex.__rhs << ')';

        return os;
    }

    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r)
    {
        __name = "DimensionalityMismatchEx";
        __current = c;
        __rhs = r;
    }

    unsigned int DimensionalityMismatchEx::getCurrent() const
    {
        return __current;
    }

    unsigned int DimensionalityMismatchEx::getRhs() const
    {
        return __rhs;
    }

    std::string DimensionalityMismatchEx::getName() const
    {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx &ex)
    {
        os << "DimenstionalityMisMatchEx (" << "current= " << ex.__current << ","
        << "rhs = " << ex.__rhs << ')';

        return os;
    }


    ZeroClustersEx::ZeroClustersEx()
    {
        __name = "ZeroClustersEX";
    }

    std::string ZeroClustersEx::getName() const
    {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroClustersEx &ex)
    {
        os << ex.getName();
        return os;
    }

    DataFileOpenEx::DataFileOpenEx(std::string filename)
    {
        __name = "DataFileOpenEX";
        __filename = filename;
    }

    std::string DataFileOpenEx::getFilename() const
    {
        return __filename;
    }

    std::string DataFileOpenEx::getName() const
    {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const DataFileOpenEx &ex)
    {
        os << ex.__name << ":" << ex.getFilename() << std::endl;
        return os;
    }

    ZeroDimensionsEx::ZeroDimensionsEx()
    {
        __name = "ZeroDimensionsEx";
    }

    std::string ZeroDimensionsEx::getName() const
    {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroDimensionsEx &ex)
    {
        os << ex.getName();
        return os;
    }


    EmptyClusterEx::EmptyClusterEx()
    {
        __name = "EmptyClusterEx";
    }

    std::string EmptyClusterEx::getName() const
    {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex)
    {
        os << ex.getName();
        return os;
    }

}


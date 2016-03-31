//
// Created by Christopher Corona on 2/24/2016.
//

#include <cmath>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "Point.h"
#include "Exceptions.h"

using namespace std;
namespace Clustering
{

    // Static ID generator
    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';

    // Constructors
    Point::Point(unsigned int intDim)
    {
        if (intDim == 0)
        {
            throw ZeroDimensionsEx();
        }

        __id = __idGen;
        ++__idGen;
        __dim = intDim;
        __values = new double[__dim];

        for (int i = 0; i < __dim; ++i)
        {
            __values[i] = 0;
        }
    }

    Point::Point(unsigned int initDim, double *initValues)
    {
        __id = __idGen++;
        __values = initValues;
        __dim = initDim;
    }

    // Big three: cpy ctor, overloaded operator=, dtor
    Point::Point(const Point &point)
    {
        __id = point.__id;
        __dim = point.getDims();
        __values = new double[__dim];

        for(int i = 0; i < __dim; ++i)
        {
            __values[i] = point.__values[i];
        }
    }

    Point &Point::operator=(const Point &point)
    {
        if (__dim != point.__dim)
        {
            throw Clustering::DimensionalityMismatchEx(__dim, point.__dim);
        }
        if (this == &point)
        {
            return *this;
        }
        else
        {
            __dim = point.__dim;
            __id = point.__id;
            for (int count = 0; count < __dim; ++count)
            {
                __values[count] = point.__values[count];
            }
        }
        return *this;
    }

    Point::~Point()
    {

    }

    void Point::rewindIdGen()
    {
        __idGen -= 1;
    }

    // Accessors & mutators
    int Point::getId() const
    {
        return __id;
    }

    unsigned int Point::getDims() const
    {
        return __dim;
    }

    void Point::setValue(unsigned int dim, double val)
    {
        if (__dim <= dim || dim < 0)
        {
            throw Clustering::OutOfBoundsEx(__dim, dim);
        }
        __values[dim] = val;
    }
    double Point::getValue(unsigned int dim) const
    {

        if (__dim <= dim || dim < 0)
        {
            throw Clustering::OutOfBoundsEx(__dim, dim);
        }

        return __values[dim];
    }

    double Point::distanceTo(const Point &point) const
    {
        double distance = 0.0;
        if (__dim != point.__dim)
        {
            throw Clustering::DimensionalityMismatchEx(__dim, point.__dim);
        }

        for(int i = 0; i < __dim; ++i)
        {
            distance = distance + ((__values[i] - point.getValue(i)) * (__values[i] - point.getValue(i)));
        }
        return sqrt(distance);
    }

    const Point Point::operator*(double m) const
    {
        Point nPoint(*this);
        for (int i = 0; i <__dim; ++i)
        {
            nPoint[i] *= m;
        }
        return nPoint;
    }

    const Point Point::operator/(double d) const
    {
        Point nPoint(*this);
        for (int i = 0; i <__dim; ++i)
        {
            nPoint[i] /= d;
        }
        return nPoint;
    }

    Point & Point::operator*=(double m)
    {
        for (int i = 0; i <__dim; ++i)
        {
            __values[i] *= m;
        }
        return *this;
    }

    Point & Point::operator/=(double d)
    {
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] /= d;
        }
        return *this;
    }

    double &Point::operator[](unsigned int dindex)
    {
        if(dindex >= __dim)
            throw Clustering::OutOfBoundsEx(__dim, dindex);
        else
            return __values[dindex];
    }

    const double &Point::operator[](unsigned int cdindex) const
    {
        if(cdindex >= __dim)
            throw Clustering::OutOfBoundsEx(__dim, cdindex);
        else
            return __values[cdindex];
    }

    bool operator<=(const Point &point1, const Point &point2)
    {
        return !(point1 > point2);
    }

    bool operator>=(const Point &point1, const Point &point2)
    {
        return !(point1 < point2);
    }

    bool operator!=(const Point &point1, const Point &point2)
    {
        return (!(point1 == point2));
    }

    const Point operator+(const Point & point1, const Point & point2)
    {
        Point nPoint(point1);
        nPoint += point2;
        return nPoint;
    }

    const Point operator-(const Point & point1, const Point & point2)
    {
        Point nPoint(point1);
        nPoint -= point2;
        return nPoint;
    }

    Point &operator+=(Point &point1, const Point &point2)
    {
        if (point1.__dim != point2.__dim)
            throw Clustering::DimensionalityMismatchEx(point1.__dim, point2.__dim);

        Point *nPoint = new Point(point2);
        for (int i = 0; i < point1.__dim; ++i)
        {
            point1.__values[i] = point1.__values[i] + nPoint->getValue(i);
        }
        delete nPoint;
        return point1;
    }

    Point &operator-=(Point &point1, const Point &point2)
    {
        if (point1.__dim != point2.__dim)
            throw Clustering::DimensionalityMismatchEx(point1.__dim, point2.__dim);


        Point *nPoint = new Point(point2);
        for (int i = 0; i < point1.__dim; ++i)
        {
            point1.__values[i] = point1.__values[i] - nPoint->getValue(i);
        }
        delete nPoint;
        return point1;
    }

    bool operator==(const Point & point1, const Point & point2)
    {
        bool pass = true;

        if (point1.__id != point2.__id)return false;
        if (point1.__dim != point2.__dim)return false;
        for (int i = 0; i < point1.getDims(); ++i)
        {
            if (point1.__values[i] != point2.__values[i])pass = false;
        }
        return pass;
    }

    bool operator<(const Point &point1, const Point &point2)
    {
        bool result = false;
        if (point1.__dim != point2.__dim)
            throw Clustering::DimensionalityMismatchEx(point1.__dim, point2.__dim);

        for (unsigned int i = 0; i < point1.__dim; ++i)
        {
            if (point1.getValue(i) < point2.getValue(i))
            {
                return true;
            }
            if (point1.getValue(i) > point2.getValue(i))
            {
                return false;
            }
        }
        return result;
    }

    bool operator>(const Point &point1, const Point &point2)
    {
        return (point2 < point1);
    }

    std::ostream &operator<<(std::ostream &out, const Point &point)
    {
        int i =0;
        for(; i<point.getDims()-1;++i)
        {
            out<< point.getValue(i);
            out <<", ";
        }
        out << point.getValue(i);
        return out;
    }

    std::istream &operator>>(std::istream &in, Point &point)
    {
        string str;
        getline(in, str);
        unsigned int size = (unsigned int)count(str.begin(), str.end(), point.POINT_VALUE_DELIM) + 1;

        stringstream ss(str);

        if (point.getDims() != size)
        {
            throw Clustering::DimensionalityMismatchEx(point.__dim, size);
        }

        int i = 0;
        while (!ss.eof()) {
            string svalue;
            getline(ss, svalue, ',');
            stringstream streamvalue(svalue);
            streamvalue >> point.__values[i];
            ++i;
        }
        return in;
    }
}


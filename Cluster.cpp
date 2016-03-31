//
// Created by Christopher Corona on 2/24/2016.
//

#include <sstream>
#include <iomanip>
#include <cassert>
#include <limits>
#include "Cluster.h"
#include "Exceptions.h"

using namespace std;
namespace Clustering
{

    unsigned int Cluster::__idGenerator = 0;
    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';

    LNode::LNode(const Point &p, LNodePtr n) :
            point(p),
            next(n)
    {}

    void Cluster::__cpy(LNodePtr points)
    {
        assert(!__size);
        LNodePtr reached = points, *prev = &__points;
        while(reached)
        {
            *prev = new LNode(reached->point, nullptr);
            __size++;
            prev = &(*prev)->next;
            reached = reached->next;
        }
    }

    void Cluster::__del()
    {

    }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) :
            __dimensions(d),
            __p(d),
            __valid(false),
            __c(c)
    {}

    // getters/setters
    const Point Cluster::Centroid::get() const
    {
        return __p;
    }

    void Cluster::Centroid::set(const Point &point)
    {
        __valid = true;
        __p = point;
    }

    bool Cluster::Centroid::isValid() const
    {
        return __valid;
    }

    void Cluster::Centroid::setValid(bool valid)
    {
        __valid = valid;
    }

    void Cluster::Centroid::toInfinity()
    {
        for (unsigned int i = 0; i < __dimensions; ++i)
        {
            __p[i] = numeric_limits<double>::max();
        }
    }

    bool Cluster::Centroid::equal(const Point &point) const
    {
        bool eq = true;
        for (unsigned int i = 0; i < __dimensions; ++i) {
            if (__p[i] != point[i]) {
                eq = false;
                break;
            }
        }
        return eq;
    }

    void Cluster:: Centroid::compute()
    {
        if(__c.__points == nullptr)
        {
            assert(__c.__size ==0);
            toInfinity();
            return;
        }
        LNodePtr curr= __c.__points;
        Point p(__c.getDimensionality());
        unsigned int sizeCheck =0;
        while (curr != nullptr)
        {
            p+= curr->point/__c.getSize();
            curr = curr->next;
            sizeCheck++;
        }
        assert(sizeCheck == __c.getSize());

        set(p);
    }

    Cluster::Cluster(unsigned int d) :
        __dimensionality(d),
        __size(0),
        __points(nullptr),
        __id(__idGenerator++),
        centroid(d, *this)
    {};

    Cluster::Cluster(const Cluster &cluster) :
            __dimensionality(cluster.__dimensionality),
            __size(0),
            __points(nullptr),
            __id(cluster.__id),
            centroid(__dimensionality, *this)
    {
        if(cluster.__points != nullptr)
        {
            assert(cluster.__size);
            __cpy(cluster.__points);
        }
        assert(__size == cluster.__size);
        centroid.compute();
    };

    Cluster &Cluster::operator=(const Cluster &cluster)
    {
        if (__size > 0)
        {
            LNodePtr destroy;
            LNodePtr cursor = __points;

            while (cursor != nullptr)
            {
                destroy = cursor;
                cursor = cursor->next;
                delete destroy;
            }
            __size = 0;
        }

        for (int i = 0; i < cluster.getSize(); ++i)
        {
            add(cluster[i]);
        }

        __id = cluster.__id;
        centroid.compute();

        return *this;
    }

    unsigned int Cluster::getSize() const
    {
        return __size;
    }

    unsigned int Cluster::getDimensionality() const
    {
        return __dimensionality;
    }

    unsigned int Cluster::getId() const
    {
        return __id;
    }

    Cluster::~Cluster()
    {

    }

    bool Cluster::contains(const Point &p) const
    {
        if (__dimensionality != p.getDims())
            throw Clustering::DimensionalityMismatchEx(__dimensionality, p.getDims());

        LNodePtr curr = __points;

        while (curr != nullptr)
        {

            if (curr->point.getId() == p.getId())
                return true;
            else
                curr = curr->next;
        }

        return false;
    }

    void Cluster::add(const Point &point)
    {
        if (__dimensionality != point.getDims())
            throw Clustering::DimensionalityMismatchEx(__dimensionality, point.getDims());

        LNodePtr next;
        LNodePtr prev;

        if (__size == 0)
        {
            ++__size;
            __points = new LNode(point, nullptr);
            centroid.setValid(false);
        }
        else
        {
            next = __points;
            prev = nullptr;
            if (contains(point))
                return;
            centroid.setValid(false);

            while (next != nullptr)
            {
                if (point < next->point)
                {
                    if (prev == nullptr)
                    {
                        __points = new LNode(point, next);
                        ++__size;
                        return;
                    }
                    else
                    {
                        prev->next = new LNode(point, next);
                        ++__size;
                        return;
                    }
                }
                prev = next;
                next = next->next;

            }

            prev->next = new LNode(point, nullptr);
            ++__size;
        }
    }

    const Point &Cluster::remove(const Point &point)
    {
        if (__dimensionality != point.getDims())
            throw Clustering::DimensionalityMismatchEx(__dimensionality, point.getDims());

        if (contains(point))
        {
            centroid.setValid(false);
            LNodePtr next;
            LNodePtr prev = nullptr;

            next = __points;

            while (next != nullptr)
            {
                if (next->point.getId() == point.getId())
                {
                    if (prev == nullptr)
                    {
                        if (__size > 1)
                            __points = next->next;
                        else
                            __points = nullptr;
                        delete next;
                        --__size;
                        break;
                    }
                    else
                    {
                        prev->next = next->next;
                        delete next;
                        --__size;
                        break;
                    }
                }
                prev = next;
                next = next->next;
            }
        }
        return point;
    }

    void Cluster::pickCentroids(unsigned int k, Point **pointArray)
    {
        if (k >= __size)
        {
            for (unsigned int i = 0; i < __size; ++i)
            {
                if(i<__size)
                *pointArray[i] = (*this)[i];
            }
            if (k > __size) {
                for (unsigned int i = __size; i < k; ++i)
                {
                    for (unsigned int d = 0; d < __dimensionality; ++d)
                    {
                        pointArray[i]->setValue(d, numeric_limits<double>::max());
                    }
                }
            }
        }
        else
        {
            *pointArray[0] = __points->point;
            double avg = 0;
            unsigned int Indx = 0;

            for (unsigned int a = 1; a < k; ++a)
            {
                for (unsigned int b = 0; b < __size; ++b)
                {
                    double next = 0;
                    for (unsigned int c = 0; c < a; ++c)
                    {
                        next += ((*this)[b]).distanceTo(*(pointArray[c]));
                    }
                    next /= a;
                    if (next > avg)
                    {
                        avg = next;
                        Indx = b;
                    }
                }
                *pointArray[a] = (*this)[Indx];
            }
        }
    }

    const Point &Cluster::operator[](unsigned int index) const
    {
        if (__size == 0)
            throw Clustering::EmptyClusterEx();

        if (index >= __size)
            throw Clustering::OutOfBoundsEx(__size, index);

        LNodePtr mark = __points;

        for (int i = 0; i < index; ++i)
        {
            mark  = mark->next;
        }
        return mark->point;
    }

    Cluster &Cluster::operator+=(const Point &point)
    {
        add(point);
        return *this;
    }

    Cluster &Cluster::operator-=(const Point &point)
    {
        remove(point);
        return *this;
    }

    Cluster &Cluster::operator+=(const Cluster &cluster)
    {
        if (__dimensionality != cluster.__dimensionality)
            throw Clustering::DimensionalityMismatchEx(__dimensionality, cluster.__dimensionality);

        for (int i = 0; i < cluster.getSize(); ++i)
        {
            add(cluster[i]);
        }

        return *this;
    }

    Cluster &Cluster::operator-=(const Cluster &cluster)
    {
        if (__dimensionality != cluster.__dimensionality)
            throw Clustering::DimensionalityMismatchEx(__dimensionality, cluster.__dimensionality);

        for (int i = 0; i < cluster.getSize(); ++i)
        {
            remove(cluster[i]);
        }
        return *this;
    }

    bool operator==(const Cluster &cluster1, const Cluster &cluster2)
    {
        if (cluster1.__dimensionality != cluster2.__dimensionality)
            throw Clustering::DimensionalityMismatchEx(cluster1.__dimensionality, cluster2.__dimensionality);

        if (cluster1.getSize() != cluster2.getSize())
            return false;

        for (int i = 0; i < cluster1.getSize(); ++i)
        {
            if (cluster1[i] != cluster2[i])
                return false;
        }

        return true;
    }

    bool operator!=(const Cluster &cluster1, const Cluster &cluster2)
    {
        return !(cluster1 == cluster2);
    }

    const Cluster operator+(const Cluster &cluster1, const Point &cluster2)
    {
        Cluster sum(cluster1);
        sum += cluster2;
        return sum;
    }

    const Cluster operator-(const Cluster &cluster1, const Point &cluster2)
    {
        Cluster sub(cluster1);
        sub -= cluster2;
        return sub;
    }

    const Cluster operator+(const Cluster &cluster1, const Cluster &cluster2)
    {
        if (cluster1.__dimensionality != cluster2.__dimensionality)
            throw Clustering::DimensionalityMismatchEx(cluster1.__dimensionality, cluster2.__dimensionality);

        Cluster sum(cluster1);
        sum += cluster2;
        return sum;
    }

    const Cluster operator-(const Cluster &cluster1, const Cluster &cluster2)
    {
        if (cluster1.__dimensionality != cluster2.__dimensionality)
            throw Clustering::DimensionalityMismatchEx(cluster1.__dimensionality, cluster2.__dimensionality);

        Cluster sub(cluster1);
        sub -= cluster2;
        return sub;
    }

    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to)
            : __p(p), __from(from), __to(to)
    {}

    void Cluster::Move::perform()
    {
        __to.add(__p);
        __from.remove(__p);
    }

    std::ostream &operator<<(std::ostream &out, const Cluster &cluster)
    {
        //failed at this

    }

    std::istream &operator>>(std::istream &in, Cluster &cluster)
    {
        //failed at this

    }

}


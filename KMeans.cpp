//
// Created by Christopher Corona on 3/24/2016.
//

#include "KMeans.h"
#include "Exceptions.h"

using namespace std;
namespace Clustering
{

    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter)

    {
        //failed at this
    }

    KMeans::~KMeans()
    {
        //failed at this
    }

    unsigned int KMeans::getMaxIter()
    {
        return __maxIter;
    }
    unsigned int KMeans::getNumIters()
    {
        return __numIter;
    }
    unsigned int KMeans::getNumNonemptyClusters()
    {
        return __numNonempty;
    }
    unsigned int KMeans::getNumMovesLastIter()
    {
        return __numMovesLastIter;
    }

    Cluster &KMeans::operator[](unsigned int u)
    {
        return *__clusters[u];
    }
    const Cluster &KMeans::operator[](unsigned int u) const
    {
        return *__clusters[u];
    }

    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans)
    {
        //failed at this
    }

    void KMeans::run()
    {
        //failed at this
    }
}



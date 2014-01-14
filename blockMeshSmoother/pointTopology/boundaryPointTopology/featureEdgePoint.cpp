#include "featureEdgePoint.h"

#include "blockMeshTopology.h"

#include <algorithm>
#include <map>

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::featureEdgePoint::featureEdgePoint
(
    const std::set<label> &pointLinked,
    const std::set<std::set<Foam::label> > &triangles,
    const point &initialPoint,
    blockMeshTopology *topo
)
    :
    boundaryPoint(triangles, initialPoint, topo),
    pointLinked_(pointLinked),
    pointLinkedNew_(pointLinked)
{
}

Foam::featureEdgePoint::~featureEdgePoint()
{

}

// * * * * * * * * * * * * * * * Private Functions * * * * * * * * * * * * * //

std::map<Foam::scalar, Foam::point> Foam::featureEdgePoint::mapNeiborFeaturePts
(
    const point &guessedPoint,
    const label &pointRef
)
{
    std::map<scalar,point> minDist;

    for
    (
        std::set<label>::iterator ptI = pointLinkedNew_.begin();
        ptI != pointLinkedNew_.end();
        ++ptI
    )
    {
        const point p1(topo_->getPointCoord(*ptI));
        const point p2(topo_->getPointCoord(pointRef));
        const point p3(guessedPoint);
        const scalar u
        (
            (
                (p3.x() - p1.x())*(p2.x() - p1.x()) +
                (p3.y() - p1.y())*(p2.y() - p1.y()) +
                (p3.z() - p1.z())*(p2.z() - p1.z())
            ) /
            (magSqr(p2 - p1) + VSMALL)
        );
        const point px
        (
            p1.x() + u*(p2.x() - p1.x()),
            p1.y() + u*(p2.y() - p1.y()),
            p1.z() + u*(p2.z() - p1.z())
        );
        if (u > 0.0 && u <= 1.0)
        { // closest point fall within the line segment
            minDist.insert(std::make_pair<scalar,point>(mag(p3 - px), px));
        }
    }

    return minDist;
}

std::set<Foam::label> Foam::featureEdgePoint::getPointLinked() const
{
    return pointLinked_;
}

Foam::point Foam::featureEdgePoint::getFeatureEdgePoint
(
    const point &guessedPoint,
    const label &ref
)
{
    std::map<scalar,point> minDists(mapNeiborFeaturePts(guessedPoint, ref));

    if (minDists.empty())
    {
        const scalar distCenter
        (
            mag(guessedPoint - topo_->getPointCoord(ref))
        );
        const scalar distExtrem1
        (
            mag(guessedPoint - topo_->getPointCoord(*pointLinkedNew_.begin()))
        );
        const scalar distExtrem2
        (
            mag(guessedPoint - topo_->getPointCoord(*pointLinkedNew_.rbegin()))
        );

        if (distCenter < distExtrem1 && distCenter < distExtrem2)
        { // Boundary is convex
            return topo_->getPointCoord(ref);
        }
        else if (distExtrem1 < distExtrem2)
        { // Nearest boundary point is begin

            return changeFeatureEdgeLinkedsPoint
            (
                *pointLinkedNew_.begin(),
                guessedPoint
            );
        }
        else
        { // Nearest boundary point is rbegin

            return changeFeatureEdgeLinkedsPoint
            (
                *pointLinkedNew_.rbegin(),
                guessedPoint
            );
        }
    }
    else
    {
        return minDists.begin()->second;
    }
}

Foam::point Foam::featureEdgePoint::changeFeatureEdgeLinkedsPoint
(
    const label &newRef,
    const point &guessedPoint
)
{
    // Update the points linked
    pointLinkedNew_ = topo_->getPointTopoPtr(newRef)->getPointLinked();

    return getFeatureEdgePoint(guessedPoint, newRef);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::point Foam::featureEdgePoint::smoothedPoint
(
    const point &guessedPoint,
    const label &pointRef
)
{
    return getFeatureEdgePoint(guessedPoint, pointRef);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //
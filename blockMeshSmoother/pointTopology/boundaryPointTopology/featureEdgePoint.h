#ifndef FEATUREEDGEPOINT_H
#define FEATUREEDGEPOINT_H

#include "boundaryPoint.h"

#include <set>


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
             Class pointTopo Declaration
\*---------------------------------------------------------------------------*/

class featureEdgePoint : public boundaryPoint
{
    //- Private data

        //- Point link at beginning
        std::set<label> pointLinked_;

        //- Point link during move
        std::set<label> pointLinkedNew_;

    //- Private member functions

        //- Get linked point of feature edge point
        std::set<label> getPointLinked() const;

public:
    //- Constructors

        //- Construct from
        featureEdgePoint
        (
            const std::set<label> &pointLinked,
            const std::set<std::set<Foam::label> > &triangles,
            const point &initialPoint,
            blockMeshTopology *topo
        );

    //- Destructor
        ~featureEdgePoint();

    //- Member functions

        //- Get optimal point with repect of point topo
        point smoothedPoint
        (
            const point &guessedPoint,
            const label &pointRef
        );

        //- Get map of points sorted by minimal distance
        std::map<scalar,point> mapNeiborFeaturePts
        (
            const point &guessedPoint,
            const label &pointRef
        );

        //- Get optimal feature edge point from guessed
        point getFeatureEdgePoint(const point &guessedPoint, const label &ref);

        //- Change feature edge linked points and get optimal point
        point changeFeatureEdgeLinkedsPoint
        (
            const label &newRef,
            const point &guessedPoint
        );
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // FEATUREEDGEPOINT_H

// ************************************************************************* //

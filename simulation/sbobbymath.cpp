#include <sbobbymath.h>

float c4PiDiv3 = 4 * M_PI / 3;
float cPiDiv3 = M_PI / 3;

float sphereCapVolume(float sphereRadius, float capHeight)
{
    //  Assert(Abs(CapHeight)<=2 * SphereRadius,
    //  Format('Cap must be smaller than sphere diameter, Abs(%f) > 2 * %f!',[CapHeight, SphereRadius]));//}

    // Calculate the volume
    return cPiDiv3 * capHeight * (3 * sphereRadius * capHeight - capHeight * capHeight);
}


float sphereCapCentroidHeight(float sphereRadius, float capHeight)
{
    //Assert(Abs(CapHeight)<=2 * SphereRadius,
    //Format('Cap must be smaller than sphere diameter, Abs(%f) > 2 * %f!',[CapHeight, SphereRadius]));//}

    return 3 * (2 * sphereRadius - capHeight) * (2 * sphereRadius - capHeight) / (4 * (3 * sphereRadius - capHeight));  
}


float submergedSphereCapVolume(float sphereRadius, float centerDepth)
{
    // Is it not submerged at all?
    if(centerDepth >= sphereRadius)
        return 0;
    // Is it fully submerged?
    else if(centerDepth <= -sphereRadius)
        return c4PiDiv3 * sphereRadius * sphereRadius * sphereRadius;
    else
    // Partially submerged, the amount submerged is calculated by the cap volume
        return sphereCapVolume(sphereRadius, sphereRadius - centerDepth );
}

float submergedSphereCapCentroidHeight(float sphereRadius, float centerDepth)
{
    // Is it not submerged at all? If it's not submerged, it has no centroid!
    if(centerDepth >= sphereRadius)
        return 0;
    // Is it fully submerged?
    else if(centerDepth <= -sphereRadius)
        return 0;    
    else    
    // Partially submerged, the amount submerged is calculated by the cap volume
        return -sphereCapCentroidHeight(sphereRadius, sphereRadius - centerDepth);
}

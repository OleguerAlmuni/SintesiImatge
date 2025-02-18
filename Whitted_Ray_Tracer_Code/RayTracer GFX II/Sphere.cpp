#include "Sphere.h"

Sphere::Sphere( const Vec3 &cent, float rad )
{
    center = cent;
    radius = rad;
}


Object *Sphere::ReadString( const char *params ) // Reads params from a string.
{
    float x, y, z, r;
    if( sscanf( params, "sphere (%f,%f,%f) %f", &x, &y, &z, &r ) == 4 )
        return new Sphere( Vec3( x, y, z ), r );
    return NULL;
}

Box3 Sphere::GetBounds() const // Returns a bounding box.
{
    Box3 box;
    box.X.min = center.x - radius;  box.X.max = center.x + radius;
    box.Y.min = center.y - radius;  box.Y.max = center.y + radius;
    box.Z.min = center.z - radius;  box.Z.max = center.z + radius;
    return box;
}

bool Sphere::Intersect( const Ray &ray, HitGeom &hitgeom ) const
{
    Vec3 A = ray.origin - center;
    Vec3 R = ray.direction;
    double b = 2.0 * ( A * R );
    double discr = b * b - 4.0 * ( A * A - radius * radius );  // The discriminant.

    // If the discriminant if negative, the quadratic equation had negative
    // roots, and the ray misses the sphere.

    if( discr < 0.0 ) return false;

    discr = sqrt( discr );

    // First try the smaller of the two roots.  If this is positive, it is the
    // closest intersection.

    double s = ( -b - discr ) * 0.5;
    if( s > 0.0 )
        {
        // If the closest intersection is too far away, report a miss.
        if( s > hitgeom.distance ) return false;
        }
    else
        {
        // Now try the other root, since the smallest root puts the
        // point of intersection "behind"us.  If the larger root is now
        // positive, it means we are inside the sphere.
        s = ( -b + discr ) * 0.5;
        if( s <= 0 ) return false;
        if( s > hitgeom.distance ) return false;
        }

    // We have an actual hit.  Fill in all the geometric information so
    // that the shader can shade this point.

    hitgeom.distance = s;
    hitgeom.point    = ray.origin + s * R;
    hitgeom.normal   = Unit( hitgeom.point - center );
    hitgeom.origin   = ray.origin;
    return true;
}

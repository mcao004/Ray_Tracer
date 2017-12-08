#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    double denom = dot(this->normal, ray.direction);
    Hit h;
    
    if (denom != 0.0) { // single or no intersection
        h.t = -dot(this->normal, ray.endpoint - this->x1) / denom;
        
        // no intersection
        if (h.t < 0) return false;

        h.object = this;
        h.ray_exiting = (denom < 0) ? false : true;

        hits.push_back(h);
        return true;
    } else { // either infinite or no intersection
        if (dot(this->normal, ray.endpoint - this->x1) == 0.0) {
            h.t = 0;
            h.object = this;
            h.ray_exiting = (denom < 0) ? false : true;
            hits.push_back(h); 
            return true;
        }
    }


    return false;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}

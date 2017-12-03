#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    // TODO
    double discriminant, t1, t2;
    vec3 v = ray.endpoint - this->center;
    Hit h;
    discriminant = pow(dot(ray.direction,v),2)-(dot(ray.direction,ray.direction)*(dot(v,v)-radius*radius));
    if (discriminant < 0.0) {
        return false;
    }
    t1 = -(dot(ray.direction,ray.endpoint) + sqrt(discriminant));
    // add hit to list
    h.object = this;
    h.t = t1;
    h.ray_exiting = false;
    hits.push_back(h);

    if (discriminant > 0.0) {
        t2 = -(dot(ray.direction,ray.endpoint) - sqrt(discriminant));
        h.t = t2;
        h.ray_exiting = true;
        hits.push_back(h);
    }
    return true;
}

vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
    // TODO: set the normal
    return normal;
}

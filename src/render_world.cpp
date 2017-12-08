#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"


Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray, Hit& hit)
{
    // TODO
    Object *closest_object = 0;
    double min_t = std::numeric_limits<double>::max();
    for (size_t i = 0; i < objects.size(); ++i) {
        std::vector<Hit> hits;
        if (objects.at(i)->Intersection(ray,hits)) {
            for (size_t j = 0; j < hits.size(); ++j) {
                if (hits.at(j).t < min_t && hits.at(j).t >= small_t) {
                    // update closest
                    closest_object = objects.at(i);
                    hit = hits.at(j);
                    min_t = hits.at(j).t;
                }
            }
        }
    }
    return closest_object;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    Ray ray; // TODO: set up the initial view ray here
    ray.endpoint = camera.position;
    ray.direction = (camera.World_Position(pixel_index) - camera.position).normalized();
    vec3 color=Cast_Ray(ray,recursion_depth_limit);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    // TODO: replace dummies
    vec3 color;
    Hit h;
    Object *closest_object = Closest_Intersection(ray,h); 

    if (closest_object != 0) { // there is an object
        // intersection point
        vec3 intPoint = ray.Point(h.t);
        vec3 normal = closest_object->Normal(intPoint).normalized();
        // if exiting, reverse normal to same side as ray
        normal = (h.ray_exiting) ? -1.0 * normal : normal;
        color = closest_object->material_shader->Shade_Surface(
                ray,
                intPoint,
                normal,
                recursion_depth);
    } else { // no object
        vec3 dummy;
        color = background_shader->Shade_Surface(ray,dummy,dummy,recursion_depth);
    }


    // determine the color here

    return color;
}

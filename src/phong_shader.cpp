#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"


vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth) const
{
    vec3 color;
    vec3 L, /*R*/h, light_ambient, light_diffuse, light_specular, light_color;
    
    // ambient light is calculated independent of light source
    light_ambient = color_ambient * (world.ambient_intensity * world.ambient_color);
    
    for (unsigned i = 0 ; i < world.lights.size(); ++i) {
        // check if light source is obstructed by object
        if (world.enable_shadows) {
            Ray shadow_ray(intersection_point, world.lights.at(i)->position - intersection_point);
            Hit h;
            Object *obstruction = world.Closest_Intersection(shadow_ray, h);
            // there exists an object that casts a shadow here
            if (obstruction != 0) {
                // no contribution of diffuse and specular light
                continue;
            }
        }

        // diffuse
        L = (world.lights.at(i)->position - intersection_point);
        light_color = (world.lights.at(i)->Emitted_Light(ray) / pow(L.magnitude(),2));
        light_diffuse = (color_diffuse * std::max(0.0, dot(same_side_normal, L.normalized()))) * light_color; 
        
        // specular
        vec3 R = (2 * dot(L.normalized(),same_side_normal) * same_side_normal - L.normalized()).normalized();
        vec3 V = (ray.endpoint - intersection_point);
        light_specular = pow(std::max(0.0, dot(R,V.normalized())), specular_power) * color_specular * light_color;

        // add to color
        color += light_diffuse + light_specular;
    }
    color += light_ambient;
        
    //std::cout << "Wow light: "<< color << std::endl;
    return color;
}

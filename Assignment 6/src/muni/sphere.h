#pragma once
#include "common.h"
#include "linalg.h"
#include "math_helpers.h"
#include <tuple>

namespace muni
{
struct Sphere
{
    Vec3f position;
    float radius;
    Vec3f color;

    /** Intersect a ray with a sphere.
        \param[in] sphere The sphere to intersect with.
        \param[in] ray_origin The origin of the ray.
        \param[in] ray_direction The direction of the ray.
        \param[in] t_near The minimum t value to consider.
        \param[in] t_far The maximum t value to consider.
        \return A tuple containing a boolean indicating whether the ray intersects
        the sphere and the t value of the intersection point along the ray.
     */
    static std::tuple<bool, float> ray_sphere_intersect(Sphere sphere, Vec3f ray_origin, Vec3f ray_direction, float t_near, float t_far)
    {
        Vec3f d = linalg::normalize(ray_direction);

        float a = linalg::dot(d, d);
        float b = 2 * linalg::dot((ray_origin - sphere.position), d);
        float c = linalg::dot((ray_origin - sphere.position), (ray_origin - sphere.position)) - (sphere.radius * sphere.radius);

        float discriminant = (b * b) - (4 * a * c);

        if (discriminant < 0.0f){
            return {false, 0.0f};
        }

        float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

        if (t1 >= t_near && t1 <= t_far){
            return {true, t1};
        }
        else if (t2 >= t_near && t2 < t_far){
            return {true, t2};
        }

        return {false, 0.0f};
    };

    /** Find the closest intersection of a ray with a set of spheres.
        \param[in] ray_pos The origin of the ray.
        \param[in] ray_dir The direction of the ray.
        \param[in] spheres The set of spheres to intersect with.
        \return A tuple containing a boolean indicating whether the ray intersects
        any sphere, the t value of the intersection point along the ray, and the nearest sphere.
     */
    static std::tuple<bool, float, Sphere> closest_hit(Vec3f ray_pos, Vec3f ray_dir, const std::vector<Sphere>& spheres)
    {
        float t_near = EPS;
        float t_far = std::numeric_limits<float>::infinity();
        Sphere nearest_sphere;
        bool hit_one_sphere = false;
        for (const Sphere& sphere : spheres)
        {
            auto [hit, t] = Sphere::ray_sphere_intersect(sphere, ray_pos, ray_dir, t_near, t_far);
            if (hit)
            {
                t_far = t;
                hit_one_sphere = true;
                nearest_sphere = sphere;
            }
        }
        return {hit_one_sphere, t_far, nearest_sphere};
    }
};
} // namespace muni

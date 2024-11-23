#pragma once
#include "common.h"
#include "math_helpers.h"
#include <tuple>

namespace muni
{
struct Triangle
{
    Vec3f v0, v1, v2;
    Vec3f color;
    unsigned int material_id;

    /** Ray-Triangle intersection
        \param[in] tri The triangle to intersect with.
        \param[in] ray_origin The origin of the ray.
        \param[in] ray_direction The direction of the ray.
        \return A tuple containing a boolean indicating whether the ray intersects
        the triangle and the t value of the intersection point along the ray.
    */
    static std::tuple<bool, float> ray_triangle_intersect(Triangle tri, Vec3f ray_origin, Vec3f ray_direction)
    {
        Vec3f E1 = tri.v1 - tri.v0; 
        Vec3f E2 = tri.v2 - tri.v0;

        Vec3f S = ray_origin - tri.v0;
        Vec3f S1 = linalg::cross(ray_direction, E2);
        Vec3f S2 = linalg::cross(S, E1);

        Vec3f t_matrix = (1 / linalg::dot(S1, E1)) * Vec3f{{linalg::dot(S2, E2)}, {linalg::dot(S1, S)}, {linalg::dot(S2, ray_direction)}};

        if ( (1 - t_matrix[1] - t_matrix[2]) >= 0 && t_matrix[1] >= 0 && t_matrix[2] >= 0){
            return {true, t_matrix[0]};
        }
        
        return {false, 0.0f};
    }

    /** Find the closest intersection of a ray with a list of triangles.
        \param[in] tri The triangle to intersect with.
        \param[in] ray_origin The origin of the ray.
        \param[in] ray_direction The direction of the ray.
        \return A tuple containing a boolean indicating whether the ray intersects
        the triangle, the t value of the intersection point along the ray, and the
        triangle that was hit.
    */
    static std::tuple<bool, float, Triangle> closest_hit(Vec3f ray_pos, Vec3f ray_dir, const std::vector<Triangle>& triangles)
    {
        float t_min = std::numeric_limits<float>::infinity();
        Triangle nearest_tri;
        bool hit_one_tri = false;
        for (const Triangle& tri : triangles)
        {
            auto [hit, t] = Triangle::ray_triangle_intersect(tri, ray_pos, ray_dir);
            if (t < EPS)
                hit = false;
            if (hit && t < t_min)
            {
                t_min = t;
                nearest_tri = tri;
                hit_one_tri = true;
            }
        }
        return {hit_one_tri, t_min, nearest_tri};
    }
};

} // namespace muni

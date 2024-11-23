#pragma once
#include "common.h"
#include "math_helpers.h"

using namespace muni;

// Evaluate the outgoing radiance of a diffuse surface under a point light source
// based on the Lambertian shading model.
// Inputs:
//   normal: the normal of the surface at the hit point.
//   light_dir: the direction from the hit point to the light source.
//   light_dist: the distance between the hit point and the light source.
//   light_radiance: the radiance of the light source.
//   albedo: the albedo of the surface.
// Returns:
//   The radiance of the surface under the light source.
Vec3f eval_radiance_diffuse(const Vec3f normal, const Vec3f light_dir, float light_dist, const Vec3f light_radiance, const Vec3f albedo)
{
    // to avoid division by zero
    light_dist = std::max(light_dist, 0.001f);
    return INV_PI * albedo * light_radiance / (light_dist * light_dist) * std::max(0.0f, dot(normal, light_dir));
}
// Reflect a ray direction using the surface normal.
// Inputs:
//   incident_dir: the incident ray direction.
//   normal: the normal of the surface at the hit point.
// Returns:
//   The reflected ray direction.
Vec3f mirror_reflect(const Vec3f incident_dir, const Vec3f normal)
{
    return incident_dir - 2 * dot(incident_dir, normal) * normal;
}

// Offset the ray origin to avoid self-intersection
// Inputs:
//   ray_pos: the original ray origin.
//   normal: the normal of the surface at the hit point.
// Returns:
//   The offset ray origin.
Vec3f offset_ray_origin(Vec3f ray_pos, Vec3f normal)
{
    return ray_pos + EPS * normal;
}

#pragma once
#include "common.h"

#undef M_PI
#define M_PI 3.14159265358979323846f
#define INV_PI 0.31830988618379067154f
#define INV_TWOPI 0.15915494309189533577f
#define INV_FOURPI 0.07957747154594766788f
#define SQRT_TWO 1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f
#define EPS 0.0001f
namespace muni
{

template<typename T>
T length_squared(Vec3<T> v)
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

} // namespace muni

/** Numerically stable quadratic equation solver at^2 + bt + c = 0
    See https://people.csail.mit.edu/bkph/articles/Quadratics.pdf
    \param[in] a The a coefficient.
    \param[in] b The b coefficient.
    \param[in] c The c coefficient.
    \return A tuple containing a boolean indicating whether the equation has real roots,
    the first root, and the second root.
 */
std::tuple<bool, float, float> solve_quadratic(float a, float b, float c)
{
    float t0, t1;
    // Degenerated case
    if (a == 0.f)
    {
        if (b == 0.f)
        {
            return {false, 0.f, 0.f};
        }
        t0 = t1 = -c / b;
        return {true, t0, t1};
    }

    float discriminant = b * b - 4.f * a * c;
    if (discriminant < 0.f)
    {
        return {false, 0.f, 0.f};
    }
    float root_discriminant = std::sqrt(discriminant);
    if (b >= 0.f)
    {
        t0 = (-b - root_discriminant) / (2.f * a);
        t1 = 2.f * c / (-b - root_discriminant);
    }
    else
    {
        t0 = 2.f * c / (-b + root_discriminant);
        t1 = (-b + root_discriminant) / (2.f * a);
    }
    return {true, t0, t1};
}

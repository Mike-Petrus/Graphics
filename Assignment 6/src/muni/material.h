#pragma once
#include "common.h"
#include "math_helpers.h"

namespace muni
{
struct Lambertian
{
    Vec3f albedo;

    Vec3f eval() const { return albedo * INV_PI; }
};

struct Mirror
{};

} // namespace muni

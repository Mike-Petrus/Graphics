#pragma once
#include "common.h"
#include "sphere.h"

namespace muni::ThreeBodyScene
{
static const std::vector<Sphere> spheres = {
    Sphere{.position = Vec3f{0.f, 0.f, 0.f}, .radius = 0.25f, .color = Vec3f{0.839039981f, 0.839039981f, 0.839999974f}},
    Sphere{.position = Vec3f{0.8f, 0.5f, 0.0f}, .radius = 0.5f, .color = Vec3f{0.0f, 0.2117f, 0.3765f}},
    Sphere{.position = Vec3f{-0.8f, 0.5f, 0.0f}, .radius = 0.5f, .color = Vec3f{0.996f, 0.7373f, 0.0667f}},
};
} // namespace muni::ThreeBodyScene

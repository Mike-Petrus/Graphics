#include "muni/common.h"
#include "muni/image.h"
#include "muni/camera.h"
#include "muni/scenes/box.h"
#include "muni/scenes/three-body.h"
#include "muni/triangle.h"

using namespace muni;

int main(int argc, char** argv)
{
    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Welcome to CS 180/280 Assignment 6\n"
        "----------------------------------------------"
    );

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 1: Ray intersection with spheres\n"
        "----------------------------------------------"
    );
    // In this part, your job is to visualize the intersection of a ray with spheres.
    // You will use the Sphere::closest_hit() function to find the closest intersection of a ray with a list of spheres.
    // The function returns a tuple containing a boolean indicating whether the ray intersects the sphere,
    // the t value of the intersection point along the ray, and the sphere that was hit.
    // You will then use the color of the nearest sphere to color the pixel.

    // Note that you don't need to modify any code in this file, instead, you will (only) need to implement the
    // Sphere::ray_sphere_intersect() function in src/muni/sphere.h. [Lecture 14 page 27-30].
    constexpr int image_width = 512;
    constexpr int image_height = 512;
    Image image = {.width = image_width, .height = image_height, .pixels = std::vector<Vec3f>(image_width * image_height)};
    Camera camera_for_sphere{
        .vertical_field_of_view = 35.0f,
        .aspect = static_cast<float>(image_width) / image_height,
        .focal_distance = 2.f,
        .position = Vec3f{0.f, 2.f, 0.f},
        .view_direction = Vec3f{0.0f, -1.0f, 0.0f},
        .up_direction = Vec3f{0.0f, 0.0f, 1.0f},
        .right_direction = Vec3f{-1.0f, 0.0f, 0.0f}
    };
    camera_for_sphere.init();

    for (int x = 0; x < image_width; x++)
    {
        for (int y = 0; y < image_height; y++)
        {
            const float u = (x + 0.5f) / image.width;
            const float v = (y + 0.5f) / image.height;
            Vec3f ray_origin = camera_for_sphere.position;
            Vec3f ray_direction = camera_for_sphere.generate_ray(u, v);
            const auto [hit, hit_t, nearest_sphere] = Sphere::closest_hit(ray_origin, ray_direction, ThreeBodyScene::spheres);
            if (hit)
                image(x, y) = nearest_sphere.color;
            else
                image(x, y) = Vec3f{0.0f};
        }
    }
    spdlog::info("Saving the image to part-1.png.");
    image.save("part-1.png");

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 2: Ray intersection with triangles\n"
        "----------------------------------------------"
    );
    // In this part, your job is to visualize the intersection of a ray with triangles.
    // You will use the Triangle::closest_hit() function to find the closest intersection of a ray with a list of triangles.
    // The function returns a tuple containing a boolean indicating whether the ray intersects the triangle,
    // the t value of the intersection point along the ray, and the triangle that was hit.
    // You will then use the color of the nearest triangle to color the pixel.

    // Note that you don't need to modify any code in this file, instead, you will (only) need to implement the
    // Triangle::ray_triangle_intersect() function in src/muni/triangle.h. [Lecture 14 page 32-36].
    Camera camera_for_triangle{
        .vertical_field_of_view = 38.6f,
        .aspect = static_cast<float>(image_width) / image_height,
        .focal_distance = 0.8f,
        .position = Vec3f{0.278f, 0.8f, 0.2744f},
        .view_direction = Vec3f{0.0f, -1.0f, 0.0f},
        .up_direction = Vec3f{0.0f, 0.0f, 1.0f},
        .right_direction = Vec3f{-1.0f, 0.0f, 0.0f}
    };
    camera_for_triangle.init();
    image.reset();

    for (int x = 0; x < image_width; x++)
    {
        for (int y = 0; y < image_height; y++)
        {
            const float u = (x + 0.5f) / image.width;
            const float v = (y + 0.5f) / image.height;
            Vec3f ray_origin = camera_for_triangle.position;
            Vec3f ray_direction = camera_for_triangle.generate_ray(u, v);
            const auto [hit, hit_t, nearest_triangle] = Triangle::closest_hit(ray_origin, ray_direction, BoxScene::triangles);
            if (hit)
                image(x, y) = nearest_triangle.color;
            else
                image(x, y) = Vec3f{0.0f};
        }
    }
    spdlog::info("Saving the image to part-2.jpg");
    image.save("part-2.png");

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 3: Whitted-style ray tracing\n"
        "----------------------------------------------"
    );
    // Part3: Whitted-style ray tracing
    // In this part, you will implement the Whitted-style ray tracing.
    // Trace rays from the camera to the scene, stop at diffuse surface -- a triangle with lambertian material (by checking the material id
    // and the corresponding material type of that triangle, see `src/muni/scenes/box.h` for details), then shoot a shadow ray to the light
    // source located at 'BoxScene::light_position' to check whether the light is blocked by any other triangles. If nothing is between the
    // light and the diffuse surface, evaluate the outgoing radiance using 'eval_radiance_diffuse()' function in `src/muni/bonus_helpers.h`
    // and BoxScene::light_color. If the intersected surface is a mirror surface -- a triangle with mirror material, reflect the ray using
    // 'mirror_reflect()' function in `src/muni/bonus_helpers.h` and continue tracing the reflected ray until it hits a diffuse surface or
    // reaches the maximum number of bounces 'max_bounce'. If the ray hits nothing, just simply return Vec3f(0.0f). Note that, to avoid
    // self-intersection, the rays started from a surface should be offset a little bit along the normal direction of the surface using
    // 'offset_ray_origin()' function in `src/muni/bonus_helpers.h`.
    const int max_bounce = 5;
    for (int x = 0; x < image_width; x++)
    {
        for (int y = 0; y < image_height; y++)
        {
            const float u = (x + 0.5f) / image.width;
            const float v = (y + 0.5f) / image.height;
            Vec3f ray_origin = camera_for_triangle.position;
            Vec3f ray_direction = camera_for_triangle.generate_ray(u, v);
            image(x, y) = Vec3f{0.0f};
        }
    }
    spdlog::info("Saving the image to bonus.png.");
    image.save("bonus.png");
    return 0;
}

#include "linalg.h"
#include "muni/common.h"
#include "muni/image.h"
#include "spdlog/spdlog.h"

using namespace muni;

int main(int argc, char** argv)
{
    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Welcome to CS 180/280 Assignment 4\n"
        "----------------------------------------------"
    );

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 1: Blinn-Phong reflection model\n"
        "----------------------------------------------"
    );
    // In this part, your job is to implement the Blinn-Phong reflection model [Lecture 8 page 9-25].
    // We here assume we already passed the Vertex Processing stage, Triangle Processing stage, and Rasterization stage.
    // Now we are at the Fragment Processing stage, we have some interpolated per-pixel (per-fragment) value, such as the world space
    // position, normal, and uv coordinates of each point on the triangle. We will use these values to calculate the color of each pixel.
    Image position_world_space;
    if (!position_world_space.load_exr("pos.exr"))
    {
        spdlog::error(
            "Failed to load the pos.exr image. Please make sure to run `xmake run -w .` in the same directory where the .exr images are "
            "located."
        );
        return 1;
    }

    Image normal;
    if (!normal.load_exr("normal.exr"))
    {
        spdlog::error(
            "Failed to load the normal.exr image. Please make sure to run `xmake run -w .` in the same directory where the .exr images are "
            "located."
        );
        return 1;
    }

    Image uv;
    if (!uv.load_exr("uv.exr"))
    {
        spdlog::error(
            "Failed to load the uv.exr image. Please make sure to run `xmake run -w .` in the same directory where the .exr images are "
            "located."
        );
        return 1;
    }

    // Also, some program parameters (that do not change across pixels) related to Blinn-Phong shading.
    Vec3f camera_position{3.0f, 4.5f, 5.0f};

    Vec3f light_position{0.0f, 4.0f, 3.5f};
    Vec3f light_intensity{4.0f, 4.0f, 4.0f};

    Vec3f ambient_color{0.05f, 0.05f, 0.05f};
    Vec3f ambient_light_intensity{3.0f, 3.0f, 3.0f};

    Vec3f diffuse_color{0.5f, 0.5f, 0.5f};
    Vec3f specular_color{0.5f, 0.5f, 0.5f};
    float p = 8.0f;

    constexpr int image_width = 512;
    constexpr int image_height = 512;
    // You can refer to the Image struct in muni/image.h for more details.
    Image image = {.width = image_width, .height = image_height, .pixels = std::vector<Vec3f>(image_width * image_height)};

    // Now we loop over each pixel in the image and calculate the color of the pixel using the Blinn-Phong reflection model.
    // This is essentially simulating a fragment shader.
    for (int x = 0; x < image_width; x++)
    {
        for (int y = 0; y < image_height; y++)
        {
            Vec3f world_position = position_world_space(x, y);
            Vec3f world_normal = normal(x, y);
            if (world_normal.x == 0.0f && world_normal.y == 0.0f && world_normal.z == 0.0f)
            {
                image(x, y) = Vec3f{0.0f, 0.0f, 0.0f};
                continue;
            }

            float r = length(world_position - light_position);
            Vec3f v = normalize(camera_position - world_position);
            Vec3f l = normalize(camera_position - light_position);
            Vec3f h = normalize(v + l);

            Vec3f light_ambient = ambient_color * ambient_light_intensity;
            Vec3f light_diffuse = diffuse_color * (light_intensity/(r * r)) * max(Vec3f{0.0f, 0.0f, 0.0f}, (dot(world_normal,l)));
            Vec3f light_specular = specular_color * (light_intensity/(r * r)) * pow(max(Vec3f{0.0f, 0.0f, 0.0f}, (dot(world_normal,h))), p);

            image(x,y) = light_ambient + light_diffuse + light_specular;

        }
    }
    spdlog::info("Saving the image to part-1.png.");
    image.save("part-1.png");

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 2: Texture mapping\n"
        "----------------------------------------------"
    );
    // In this part, your job is to implement texture mapping [Lecture 9 page 24-36] to have different "diffuse_color" at different places
    // on the object [Lecture 9 page 36]. You need to load the diffuse_color.exr image and use "uv_coords" to query its pixel values as the
    // "diffuse_color" in your Blinn-Phong reflection model (you can reuse the code from Part 1).

    Image diffuse_color_image = {.width = image_width, .height = image_height, .pixels = std::vector<Vec3f>(image_width * image_height)};
    if (!diffuse_color_image.load_exr("diffuse_color.exr"))
    {
        spdlog::error(
            "Failed to load the diffuse_color.exr image. Please make sure to run `xmake run -w .` in the same directory where the .exr "
            "images are located."
        );
        return 1;
    }
    image.reset();

    for (int x = 0; x < image_width; x++)
    {
        for (int y = 0; y < image_height; y++)
        {
            Vec3f world_position = position_world_space(x, y);
            Vec3f world_normal = normal(x, y);
            Vec2f uv_coords = uv(x, y).xy(); // Use this uv_coords value to query the pixel value from the diffuse_color_image.
            uv_coords.y = 1.0f - uv_coords.y;
            // Hint:
            // uv_coords values are in the range [0, 1], so you need to map them to the range [0, width - 1] and [0, height - 1] to query
            // the pixel value from the diffuse_color_image.
            if (world_normal.x == 0.0f && world_normal.y == 0.0f && world_normal.z == 0.0f)
            {
                image(x, y) = Vec3f{0.0f, 0.0f, 0.0f};
                continue;
            }

            int uvx = uv_coords.x * (diffuse_color_image.width - 1);
            int uvy = uv_coords.y * (diffuse_color_image.height -1);

            Vec3f tex_color = diffuse_color_image(uvx, uvy);

            float r = length(world_position - light_position);
            Vec3f v = normalize(camera_position - world_position);
            Vec3f l = normalize(camera_position - light_position);
            Vec3f h = normalize(v + l);

            Vec3f light_ambient = ambient_color * ambient_light_intensity;
            Vec3f light_diffuse = tex_color * (light_intensity/(r * r)) * max(Vec3f{0.0f, 0.0f, 0.0f}, (dot(world_normal,l)));
            Vec3f light_specular = specular_color * (light_intensity/(r * r)) * pow(max(Vec3f{0.0f, 0.0f, 0.0f}, (dot(world_normal,h))), p);

            image(x,y) = light_ambient + light_diffuse + light_specular;
            //image(x, y) = tex_color;
        }
    }
    spdlog::info("Saving the image to part-2.png.");
    image.save("part-2.png");
    return 0;
}

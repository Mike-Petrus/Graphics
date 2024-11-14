#include "muni/common.h"
#include "muni/image.h"
#include "spdlog/spdlog.h"

using namespace muni;

struct Triangle {
    // World space vertex positions
    Vec3f v[3];
    // Color of the triangle (RGB, 0.0f - 1.0f)
    Vec3f color;
    // NDC space vertex positions
    Vec3f v_ndc[3];
    // Screen space vertex positions
    Vec2f v_screen[3];
    // Bounding box of the triangle in screen space
    Vec2i bounding_box_min;
    Vec2i bounding_box_max;
};

struct Camera {
    Vec3f position;
    Vec3f look_at_direction;
    Vec3f up_direction;
};

/**
 * @brief Output the interpolated z value at a given point (x, y) inside a triangle.
 * 
 * @param triangle The triangle to interpolate.
 * @param x x-coordinate of the screen space point.
 * @param y y-coordinate of the screen space point.
 * @return float The interpolated z value at the given point, the less the value, the closer to the camera.
 */
float interpolate_z(const Triangle &triangle, float x, float y) {
    Vec2f v0 = triangle.v_screen[0];
    Vec2f v1 = triangle.v_screen[1];
    Vec2f v2 = triangle.v_screen[2];
    Vec2f p = {x, y};

    float denominator = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
    float u = ((v1.y - v2.y) * (p.x - v2.x) + (v2.x - v1.x) * (p.y - v2.y)) / denominator;
    float v = ((v2.y - v0.y) * (p.x - v2.x) + (v0.x - v2.x) * (p.y - v2.y)) / denominator;
    float w = 1 - u - v;

    float interpolated_z_ndc = u * triangle.v_ndc[0].z + v * triangle.v_ndc[1].z + w * triangle.v_ndc[2].z;
    return 1 - interpolated_z_ndc;
}

/**
 * @brief Check if a point is inside a triangle.
 * 
 * @param triangle The triangle to check.
 * @param x x-coordinate of the screen space point.
 * @param y y-coordinate of the screen space point.
 * @return true if the point is inside the triangle, false otherwise.
 */
bool inside_triangle(const Triangle &triangle, float x, float y) {
    float alpha = (-(x - triangle.v_screen[1].x)*(triangle.v_screen[2].y - triangle.v_screen[1].y) + (y - triangle.v_screen[1].y)*(triangle.v_screen[2].x - triangle.v_screen[1].x)) /
        (-(triangle.v_screen[0].x - triangle.v_screen[1].x)*(triangle.v_screen[2].y - triangle.v_screen[1].y) + (triangle.v_screen[0].y - triangle.v_screen[1].y)*(triangle.v_screen[2].x - triangle.v_screen[1].x));

    float beta = (-(x - triangle.v_screen[2].x)*(triangle.v_screen[0].y - triangle.v_screen[2].y) + (y - triangle.v_screen[2].y)*(triangle.v_screen[0].x - triangle.v_screen[2].x)) /
        (-(triangle.v_screen[1].x - triangle.v_screen[2].x)*(triangle.v_screen[0].y - triangle.v_screen[2].y) + (triangle.v_screen[1].y - triangle.v_screen[2].y)*(triangle.v_screen[0].x - triangle.v_screen[2].x));

    float gamma = 1.0f - alpha - beta;

    if (alpha > 0 && beta > 0 && gamma > 0){return true;}

    return false;
}

int main(int argc, char **argv) {
    spdlog::info("\n"
                 "----------------------------------------------\n"
                 "Welcome to CS 180/280 Assignment 3\n"
                 "----------------------------------------------");
    
    // Here we define the scene for you to render. There are three triangles, each with a different color.
    Triangle triangles[3] = {
        Triangle {
            .v = { Vec3f{2.0f, 2.0f, -1.0f}, Vec3f{1.0f, 3.0f, -1.0f}, Vec3f{-2.0f, 0.0f, -10.0f} },
            .color = Vec3f {1.f, 0.f, 0.f}
        },
        Triangle {
            .v = { Vec3f{-1.0f, 3.0f, -3.0f}, Vec3f{-3.0f, 1.0f, -3.0f}, Vec3f{0.5f, -3.0f, -8.0f} },
            .color = Vec3f {0.f, 1.f, 0.f}
        },
        Triangle {
            .v = { Vec3f{-2.0f, -3.0f, -2.0f}, Vec3f{0.0f, -5.0f, -2.0f}, Vec3f{1.5f, 4.0f, -8.0f} },
            .color = Vec3f {0.f, 0.f, 1.f}
        }
    };

    Camera camera = {
        .position = Vec3f{0.0f, 0.0f, 5.0f},
        .look_at_direction = Vec3f{0.0f, 0.0f, -1.0f},
        .up_direction = Vec3f{0.0f, 1.0f, 0.0f}
    };
    float n = -0.1f, f = -50.0f;
    float l = -0.0414214f, r = 0.0414214f, b = -0.0414214f, t = 0.0414214f;
    
    constexpr int image_width = 512;
    constexpr int image_height = 512;
    // You can refer to the Image struct in muni/image.h for more details.    
    Image image = {
        .width = image_width,
        .height = image_height,
        .pixels = std::vector<Vec3f>(image_width * image_height)
    };
    
    spdlog::info("\n"
                 "----------------------------------------------\n"
                 "Part 1: NDC (Canonical Cube) to Screen Transformation\n"
                 "----------------------------------------------");
    // In this part, your job is to transform the vertices of the triangles from world space into NDC space (Canonical Cube) [Lecture 6 page 8]
    // first (which is what we did in the previous assignment), and then transform them from NDC space to screen space [Lecture 6 page 10-12].

    Vec3f g_t_cross = cross(camera.look_at_direction, camera.up_direction);

    Mat4f t_view_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-(camera.position.x), -(camera.position.y), -(camera.position.z), 1}};
    Mat4f r_view_matrix = {{(g_t_cross.x), camera.up_direction.x, -(camera.look_at_direction.x), 0},
    {(g_t_cross.y), camera.up_direction.y, -(camera.look_at_direction.y), 0},
    {(g_t_cross.z), camera.up_direction.z, -(camera.look_at_direction.z), 0},
    {0, 0, 0, 1}};

    Mat4f view_transform_matrix = mul(r_view_matrix, t_view_matrix);

    Mat4f orthographic_projection_matrix = mul(Mat4f{{(2.0f/(r-l)), 0, 0, 0}, {0, (2.0f/(t-b)), 0, 0}, {0, 0, (2.0f/(n-f)), 0}, {0, 0, 0, 1}},
                                                Mat4f{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-((r+l)/2.0f), -((t+b)/2.0f), -((n+f)/2.0f), 1}});   

    Mat4f perspective_matrix = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, (n + f), 1}, {0, 0, -(n * f), 0}};
    Mat4f perspective_projection_matrix = mul(orthographic_projection_matrix, perspective_matrix);

    // Note that the w coordinate of vertices after perspective transformation may not be 1.0, so you need to divide the x, y, and z coordinates by w.
    // After this normalization, you can safely handle further transformations.


    for (Triangle &triangle : triangles) {
        // TODO: Transform the vertices from world space to NDC space (exactly what you did in assignment 2).
        // Note: You should fill in the v_ndc field of the triangle struct with the transformed vertices.
        spdlog::warn("part 1: World -> NDC transformation not implemented.");
        Vec4f v0_homogeneous = Vec4f{triangle.v[0], 1.0f};
        Vec4f v1_homogeneous = Vec4f{triangle.v[1], 1.0f};
        Vec4f v2_homogeneous = Vec4f{triangle.v[2], 1.0f};

        v0_homogeneous = mul(view_transform_matrix, v0_homogeneous);
        v1_homogeneous = mul(view_transform_matrix, v1_homogeneous);
        v2_homogeneous = mul(view_transform_matrix, v2_homogeneous);

        spdlog::info("The transformed triangle vertices in homogenous space are: v0 = {}, v1 = {}, v2 = {}.",
        v0_homogeneous,
        v1_homogeneous,
        v2_homogeneous
        );

        Vec4f v0_homogeneous_persp = mul(perspective_projection_matrix, v0_homogeneous);
        Vec4f v1_homogeneous_persp = mul(perspective_projection_matrix, v1_homogeneous);
        Vec4f v2_homogeneous_persp = mul(perspective_projection_matrix, v2_homogeneous);

        spdlog::info("The transformed triangle vertices after perspective projection are: v0 = {}, v1 = {}, v2 = {}.",
        v0_homogeneous_persp,
        v1_homogeneous_persp,
        v2_homogeneous_persp
        );

        float w0 = v0_homogeneous_persp.w;
        float w1 = v1_homogeneous_persp.w;
        float w2 = v2_homogeneous_persp.w;

        spdlog::info("The transformed triangle vertices in canonical space are: v0 = {}, v1 = {}, v2 = {}.",
        (v0_homogeneous_persp/w0),
        (v1_homogeneous_persp/w1),
        (v2_homogeneous_persp/w2)
        );

        triangle.v_ndc[0] = {(v0_homogeneous_persp.x/w0), (v0_homogeneous_persp.y/w0), (v0_homogeneous_persp.z/w0)};
        triangle.v_ndc[1] = {(v1_homogeneous_persp.x/w1), (v1_homogeneous_persp.y/w1), (v1_homogeneous_persp.z/w1)};
        triangle.v_ndc[2] = {(v2_homogeneous_persp.x/w2), (v2_homogeneous_persp.y/w2), (v2_homogeneous_persp.z/w2)};

        spdlog::info(
        "The transformed triangle vertices in the canonical cube [-1, 1]^3"
        " after the perspective projection are: v0 = {}, v1 = {}, v2 = {}.",
        triangle.v_ndc[0],
        triangle.v_ndc[1],
        triangle.v_ndc[2]
        );

        // =============================================================================================
        // TODO: Transform the vertices from NDC space to screen space, also, calculate the bounding box for the triangle.
        // Note: You should fill in the v_screen, bounding_box_min and bounding_box_max field of the triangle struct with the transformed vertices.
        // Note: Be careful with the bounding box, you should not go out of the image boundary.
        // =============================================================================================

        Mat4f viewport_transform_matrix = {{(image_width/2.0f), 0, 0, 0}, {0, (image_height/2.0f), 0, 0}, {0, 0, 1, 0}, {(image_width/2.0f), (image_height/2.0f), 0, 1}};


        Vec4f v0_screen_space = Vec4f{triangle.v_ndc[0], 1.0f};
        Vec4f v1_screen_space = Vec4f{triangle.v_ndc[1], 1.0f};
        Vec4f v2_screen_space = Vec4f{triangle.v_ndc[2], 1.0f};

        v0_screen_space = mul(viewport_transform_matrix, v0_screen_space);
        v1_screen_space = mul(viewport_transform_matrix, v1_screen_space);
        v2_screen_space = mul(viewport_transform_matrix, v2_screen_space);

        spdlog::info("The transformed triangle vertices in screen space are: v0 = {}, v1 = {}, v2 = {}.",
        v0_screen_space,
        v1_screen_space,
        v2_screen_space
        );

        triangle.v_screen[0] = {v0_screen_space.x, v0_screen_space.y};
        triangle.v_screen[1] = {v1_screen_space.x, v1_screen_space.y};        
        triangle.v_screen[2] = {v2_screen_space.x, v2_screen_space.y};

        spdlog::info(
        "The transformed triangle vertices in screen space after the viewport projection are: v0 = {}, v1 = {}, v2 = {}.",
        triangle.v_screen[0],
        triangle.v_screen[1],
        triangle.v_screen[2]
        );

        int ymax = std::max((int)v0_screen_space.y, (int)v1_screen_space.y);
        ymax = std::max(ymax, (int)v2_screen_space.y);
        ymax = std::min(ymax, image_height);

        int ymin = std::min((int)v0_screen_space.y, (int)v1_screen_space.y);
        ymin = std::min(ymin, (int)v2_screen_space.y);
        ymin = std::max(ymin, 0);

        int xmax = std::max((int)v0_screen_space.x, (int)v1_screen_space.x);
        xmax = std::max(xmax, (int)v2_screen_space.x);
        xmax = std::min(xmax, image_height);

        int xmin = std::min((int)v0_screen_space.x, (int)v1_screen_space.x);
        xmin = std::min(xmin, (int)v2_screen_space.x);
        xmin = std::max(xmin, 0);

        triangle.bounding_box_min = {xmin, ymin};
        triangle.bounding_box_max = {xmax, ymax};

        spdlog::info(
        "The bounding box for the triangle is: min = {}, max = {}",
        triangle.bounding_box_min,
        triangle.bounding_box_max
        );               
    
        image.reset();
        for (const Vec2f &screen_space_vertex : triangle.v_screen) {
            int screen_x = static_cast<int>(screen_space_vertex[0] + 0.5f);
            int screen_y = static_cast<int>(screen_space_vertex[1] + 0.5f);
            if (screen_x >= 0 && screen_x < image_width && screen_y >= 0 && screen_y < image_height) {
                for (int i = -2; i <= 2; i++) {
                    for (int j = -2; j <= 2; j++) {
                        if (screen_x + i >= 0 && screen_x + i < image_width && screen_y + j >= 0 && screen_y + j < image_height) {
                            image(screen_x + i, screen_y + j) = Vec3f{1.0f, 1.0f, 1.0f};
                        }
                    }
                }
            }
        }
        std::string filename = "part-1-triangle-" + std::to_string(&triangle - triangles + 1) + ".png";
        spdlog::info("Saving the image to {}", filename);
        image.save(filename);
    }

    spdlog::info("\n"
                 "----------------------------------------------\n"
                 "Part 2: Inside Triangle Test\n"
                 "----------------------------------------------");
    // In this part, your job is to implement the inside triangle test [Lecture 6 page 37-39].
    // You don't need to touch the code below, but you should implement the inside_triangle function above,
    // where you determine whether a given point is inside the triangle or not.
    for (const Triangle &triangle : triangles) {
        image.reset();
        for (int x = triangle.bounding_box_min.x; x < triangle.bounding_box_max.x; x++) {
            for (int y = triangle.bounding_box_min.y; y < triangle.bounding_box_max.y; y++) {
                if (inside_triangle(triangle, x, y)) {
                    image(x, y) = Vec3f{1.0f, 1.0f, 1.0f};
                }
            }
        }
        std::string filename = "part-2-triangle-" + std::to_string(&triangle - triangles + 1) + ".png";
        spdlog::info("Saving the image to {}", filename);
        image.save(filename);
    }

    // =============================================================================================

    spdlog::info("\n"
                 "----------------------------------------------\n"
                 "Part 3: Z-Buffering\n"
                 "----------------------------------------------");
    // In this part, your job is to implement the Z-Buffer algorithm [Lecture 7 page 42-44].
    image.reset();
    Image z_buffer = {
        .width = image_width,
        .height = image_height,
        .pixels = std::vector<Vec3f>(image_width * image_height)
    };

    // TODO: Initialize the z-buffer value to inifity for each pixel
    // (you may use `std::numeric_limits<float>::infinity()`)
    // and then implement the Z-Buffer algorithm.

    for (int i = 0; i < z_buffer.pixels.size(); i++){
        z_buffer.pixels[i].z = std::numeric_limits<float>::infinity();
    }

    for (const Triangle &triangle : triangles) {
        for (int x = triangle.bounding_box_min.x; x < triangle.bounding_box_max.x; x++) {
            for (int y = triangle.bounding_box_min.y; y < triangle.bounding_box_max.y; y++) {
                if (inside_triangle(triangle, x, y)) {
                    if (interpolate_z(triangle, x, y) < z_buffer(x,y).z){
                        z_buffer(x,y).z = interpolate_z(triangle, x, y);
                        image(x, y) = triangle.color;
                    }
                }
            }
        }

    }

    // Note: We haven't covered how to calculate the z value at each screen space point in the lecture.
    // We have provided you with the interpolate_z function above, which you can use to calculate the z value at each screen space point.
    // With a triangle t and screen space point (x, y), you can call interpolate_z(t, x, y) to get the z value at that point.
    // Note that z is positive, and the less the value, the closer this point is to the camera.


    spdlog::info("Saving the image to part-3.png.");
    image.save("part-3.png");
   
    return 0;
}

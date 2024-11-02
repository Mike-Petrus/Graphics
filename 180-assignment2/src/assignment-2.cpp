#include "muni/common.h"

using namespace muni;

struct Camera
{
    Vec3f position;
    Vec3f look_at_direction;
    Vec3f up_direction;
};

struct Triangle
{
    Vec3f v0, v1, v2;
};

int main(int argc, char** argv)
{
    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Welcome to CS 180 / CS 280 Assignment 2\n"
        "----------------------------------------------"
        "\n"
        "----------------------------------------------\n"
        "Part 1: view transformation (30 points)\n"
        "----------------------------------------------"
    );
    // (Before diving into this part, we recommend you to read carefully the pages 27-32 of the lecture 04.)
    // In this part, your job is to first construct a view/camera transformation matrix using the given camera parameters;
    // then apply the transformation to the given triangle vertices to transform them from world space to camera space.
    // The camera is defined by the following parameters:
    Camera camera{
        .position = Vec3f{4.0f, 2.0f, 6.0f},
        .look_at_direction = linalg::normalize(Vec3f{-4.0f, 0.0f, -6.0f}), // Make sure the look_at_direction is a unit vector
        .up_direction = linalg::normalize(Vec3f{0.0f, 1.0f, 0.0f})         // Assume perpendicular to the look_at_direction
    };
    // The triangle vertices in world space are defined as follows:
    Triangle triangle_world_space{.v0 = Vec3f{2.0f, 0.5f, 1.0f}, .v1 = Vec3f{1.0f, 0.3f, 3.0f}, .v2 = Vec3f{0.7f, 1.0f, 2.0f}};

    // =============================================================================================
    // Write your code below.
    // First, construct the view transformation matrix using the given camera parameters.
    Vec3f g_t_cross = cross(camera.look_at_direction, camera.up_direction);

    //spdlog::info("g x t = {}", g_t_cross);
    //spdlog::info("camera look_at: {}", camera.look_at_direction);

    Mat4f t_view_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-(camera.position.x), -(camera.position.y), -(camera.position.z), 1}};

    Mat4f r_view_matrix = {{(g_t_cross.x), camera.up_direction.x, -(camera.look_at_direction.x), 0},
    {(g_t_cross.y), camera.up_direction.y, -(camera.look_at_direction.y), 0},
    {(g_t_cross.z), camera.up_direction.z, -(camera.look_at_direction.z), 0},
    {0, 0, 0, 1}};

    //spdlog::info("Tview matrix = \n {}", t_view_matrix);
    //spdlog::info("Rview matrix = \n {}", r_view_matrix);

    Mat4f view_transform_matrix = mul(r_view_matrix, t_view_matrix);

    spdlog::info("Mview matrix = \n {}", view_transform_matrix);
    // Then, apply the transformation to the given triangle vertices to transform them from world space to camera space.
    // Note that all the points in this assignment are not written in homogeneous coordinates, so you'll need to fisrt convert them to
    // homogeneous coordinates by adding a 1 at the end before applying the transformation. Like this:
    Vec4f v0_homogeneous = Vec4f{triangle_world_space.v0, 1.0f};
    Vec4f v1_homogeneous = Vec4f{triangle_world_space.v1, 1.0f};
    Vec4f v2_homogeneous = Vec4f{triangle_world_space.v2, 1.0f};

    v0_homogeneous = mul(view_transform_matrix, v0_homogeneous);
    v1_homogeneous = mul(view_transform_matrix, v1_homogeneous);
    v2_homogeneous = mul(view_transform_matrix, v2_homogeneous);

    // spdlog::info("The transformed triangle vertices in homogenous space are: v0 = {}, v1 = {}, v2 = {}.",
    //     v0_homogeneous,
    //     v1_homogeneous,
    //     v2_homogeneous
    // );


    Triangle triangle_camera_space = {.v0 = Vec3f{v0_homogeneous.x, v0_homogeneous.y, v0_homogeneous.z},
    .v1 = Vec3f{v1_homogeneous.x, v1_homogeneous.y, v1_homogeneous.z},
    .v2 = Vec3f{v2_homogeneous.x, v2_homogeneous.y, v2_homogeneous.z}};

    // Finally, print out the transformed triangle vertices in camera space.
    spdlog::info(
        "The transformed triangle vertices in camera space are: v0 = {}, v1 = {}, v2 = {}.",
        triangle_camera_space.v0,
        triangle_camera_space.v1,
        triangle_camera_space.v2
    );
    // =============================================================================================

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 2: orthographic projection (30 points)\n"
        "----------------------------------------------"
    );
    // (Before diving into this part, we recommend you to read carefully the pages 5-13 of the lecture 05.)
    // In this part, your job is to first construct an orthographic projection matrix using the given parameters;
    // then apply the projection to the given triangle vertices to transform them from world space to the “canonical” cube [-1, 1]^3.

    // The orthographic projection is defined by the following parameters:
    // The left, right, bottom, top, near, and far planes of the cubic.
    float l = -4.0f, r = 4.0f, b = -2.0f, t = 2.0f, n = -1.0f, f = -10.0f;

    // =============================================================================================
    // Write your code below.
    // First, construct the orthographic projection matrix using the given parameters.
    Mat4f orthographic_projection_matrix = mul(Mat4f{{(2.0f/8.0f), 0, 0, 0}, {0, (2.0f/4.0f), 0, 0}, {0, 0, (2.0f/9.0f), 0}, {0, 0, 0, 1}},
                                                Mat4f{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, (11.0f/2.0f), 1}});

    //spdlog::info("Orthographic projection matrix = \n{}", orthographic_projection_matrix);

    // Then, apply the projection to the given triangle vertices to transform them from world space to the “canonical” cube [-1, 1]^3.
    // Note that you'll need to first transform the triangle vertices from world space to camera space (using the view_transform_matrix you
    // implemented in part 1) before applying the projection.


    Vec4f v0_homogeneous_ortho = mul(orthographic_projection_matrix, v0_homogeneous);
    Vec4f v1_homogeneous_ortho = mul(orthographic_projection_matrix, v1_homogeneous);
    Vec4f v2_homogeneous_ortho = mul(orthographic_projection_matrix, v2_homogeneous);

    Triangle triangle_canonical_cube = {.v0 = Vec3f{v0_homogeneous_ortho.x, v0_homogeneous_ortho.y, v0_homogeneous_ortho.z},
    .v1 = Vec3f{v1_homogeneous_ortho.x, v1_homogeneous_ortho.y, v1_homogeneous_ortho.z},
    .v2 = Vec3f{v2_homogeneous_ortho.x, v2_homogeneous_ortho.y, v2_homogeneous_ortho.z}};
    // Finally, print out the transformed triangle vertices in the “canonical” cube [-1, 1]^3.
    spdlog::info(
        "The transformed triangle vertices in the canonical cube [-1, 1]^3"
        " after the orthographic projection are: v0 = {}, v1 = {}, v2 = {}.",
        triangle_canonical_cube.v0,
        triangle_canonical_cube.v1,
        triangle_canonical_cube.v2
    );
    // =============================================================================================

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 3: perspective projection (40 points)\n"
        "----------------------------------------------"
    );
    // (Before diving into this part, we recommend you to read carefully the pages 14-29 of the lecture 05.)
    // In this part, your job is to first construct a perspective projection matrix using the given parameters;
    // then apply the projection to the given triangle vertices to transform them from world space to the “canonical” cube [-1, 1]^3.

    // The perspective projection is defined by the following parameters:
    // The near, and far planes of the cubic.
    n = -1.0f, f = -10.0f;
    // And the left, right, bottom, top values specified on the near plane.
    l = -4.0f, r = 4.0f, b = -2.0f, t = 2.0f;

    // =============================================================================================
    // Write your code below.
    // First, construct the perspective matrix that "squishes" the frustum into a cube.
    Mat4f perspective_matrix = {{n, 0, 0, 0}, {0, n, 0, 0}, {0, 0, (n + f), 1}, {0, 0, -(n * f), 0}};
    // Then, compose the perspective matrix and the orthographic projection matrix to get the final perspective projection matrix.
    Mat4f perspective_projection_matrix = mul(orthographic_projection_matrix, perspective_matrix);
    // Finally, apply the projection to the given triangle vertices to transform them from world space to the “canonical” cube [-1, 1]^3.
    // Note that you'll need to first transform the triangle vertices from world space to camera space (using the view_transform_matrix you
    // implemented in part 1) before applying the projection.

    //spdlog::info("The perspective projection matrix is \n {}", perspective_projection_matrix);

    Vec4f v0_homogeneous_persp = mul(perspective_projection_matrix, v0_homogeneous);
    Vec4f v1_homogeneous_persp = mul(perspective_projection_matrix, v1_homogeneous);
    Vec4f v2_homogeneous_persp = mul(perspective_projection_matrix, v2_homogeneous);

    Triangle triangle_canonical_cube_perspective = {.v0 = Vec3f{v0_homogeneous_persp.x, v0_homogeneous_persp.y, v0_homogeneous_persp.z},
    .v1 = Vec3f{v1_homogeneous_persp.x, v1_homogeneous_persp.y, v1_homogeneous_persp.z},
    .v2 = Vec3f{v2_homogeneous_persp.x, v2_homogeneous_persp.y, v2_homogeneous_persp.z}};
    // Finally, print out the transformed triangle vertices in the “canonical” cube [-1, 1]^3.
    spdlog::info(
        "The transformed triangle vertices in the canonical cube [-1, 1]^3"
        " after the perspective projection are: v0 = {}, v1 = {}, v2 = {}.",
        triangle_canonical_cube_perspective.v0,
        triangle_canonical_cube_perspective.v1,
        triangle_canonical_cube_perspective.v2
    );

    return 0;
}

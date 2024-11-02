#include "muni/common.h"

using namespace muni;

int main(int argc, char** argv)
{
    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Welcome to CS 180/CS280 Assignment 1\n"
        "----------------------------------------------"
    );
    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 1: vector and matrix\n"
        "----------------------------------------------"
    );
    // Let's first get familiar with the data structures we will use in the
    // assignment. To create a 3D vector containing float type, we use the
    // 'Vec3f'. Here the postfix '3' means 3D, and 'f' means float.
    Vec3f v1(1.0f, 2.0f, 3.0f);
    // A vector can also be initialized by using curly braces (array initializer):
    Vec3f v2{4.0f, 5.0f, 6.0f};
    // We can also give one value to initialize all elements in the vector:
    Vec3f v3(1.0f); // v3 = {1.0f, 1.0f, 1.0f}
    // We provide a logger class 'spdlog' that we can use the 'spdlog::info()'
    // function to print out any information to the console.
    spdlog::info("v1 = {},  v2 = {}, v3 = {}.", v1, v2, v3);
    // To access the elements of a vector, we can use the '[]' operator:
    float v1_x = v1[0];
    // But a better way is to directly access the member variables: 'x', 'y', 'z':
    spdlog::info("v1[0] = {}, v1.x = {}, v1.y = {}, v1.z = {}.", v1_x, v1.x, v1.y, v1.z);

    spdlog::info("============== Part 1.1 ==============");
    // =============================================================================================
    // Part 1.1: Create a 3D float vector 'p1' with the specific values of (0.1f,
    // 0.2f, 0.3f), then print it out to the console using spdlog::info.
    // =============================================================================================
    // TODO: Write your code below.

    Vec3f p1(0.1f, 0.2f, 0.3f);
    spdlog::info("{}", p1);

    // =============================================================================================
    spdlog::info("Your answer should be: {0.1f, 0.2f, 0.3f}");
    spdlog::info("========== End of Part 1.1 ===========");

    // Operations '+', '-', '*','/' refer to element-wise add, subtract, multiply
    // and divide vectors:
    spdlog::info("{} + {} = {}", v1, v2, v1 + v2);
    spdlog::info("{} - {} = {}", v1, v2, v1 - v2);
    spdlog::info("{} * {} = {}", v1, v2, v1 * v2);
    spdlog::info("{} / {} = {}", v1, v2, v1 / v2);

    // Those operations can also be used between a vector and a scalar:
    float s = 2.0f;
    spdlog::info("{} + {} = {}", v1, s, v1 + s);
    spdlog::info("{} - {} = {}", v1, s, v1 - s);
    spdlog::info("{} * {} = {}", v1, s, v1 * s);
    spdlog::info("{} / {} = {}", v1, s, v1 / s);

    // We can use 'length()' function to compute the length of a vector:
    spdlog::info("The length of {} is {}", v1, length(v1));
    // The 'normalize()' function can be used to normalize a vector:
    spdlog::info("The normalized vector of {} is {}", v1, normalize(v1));
    // We can use 'distance()' function to compute the distance between two
    // vectors:
    spdlog::info("The distance between {} and {} is {}", v1, v2, distance(v1, v2));

    spdlog::info("============== Part 1.2 ==============");
    // =============================================================================================
    // Part 1.2: Normalize v2, print out the normalized vector and the length of
    // the normalized vector.
    // =============================================================================================
    // TODO: Write your code below.

    spdlog::info("{} and {}", normalize(v2), length(normalize(v2)));

    // =============================================================================================
    spdlog::info("Your answer should be: {0.45584232, 0.5698029, 0.6837635} and 1");
    spdlog::info("========== End of Part 1.2 ===========");

    // We can use 'dot()' or 'cross()'function to compute the dot or cross product
    // of two vectors:
    spdlog::info("The dot product of {} and {} is {}", v1, v2, dot(v1, v2));
    spdlog::info("The cross product of {} and {} is {}", v1, v2, cross(v1, v2));

    spdlog::info("============== Part 1.3 ==============");
    // =============================================================================================
    // Part 1.3: Find a vector p3 that is orthogonal to v2 and v3 using 'cross()'
    // function. Print out p3, the dot product of p3 and v2, and the dot product
    // of p3 and v3 respectively.
    // =============================================================================================
    // TODO: Write your code below.

    Vec3f p3 = cross(v2, v3);
    spdlog::info("{}, {}, {}", p3, dot(p3, v2), dot(p3, v3));
    // =============================================================================================
    spdlog::info("Your answer should be:  {-1, 2, -1}, 0, 0");
    spdlog::info("========== End of Part 1.3 ===========");

    // We can create a 3x3 float matrix using 'Mat3f':
    Mat3f mat1{{0, 0, 1}, {2, 0, 0}, {0, 3, 0}};
    // Please note that the matrix is stored in **column-major** order,
    // i.e., the first row of mat1 is {0, 2, 0}, not {0, 0, 1}.
    // mat1 = |0 2 0|
    //        |0 0 3|
    //        |1 0 0|
    // Matrix multiplication can be done using mul() function:
    Vec3f v4 = mul(mat1, v1);
    spdlog::info("mat1 * v1 = {}", v4);
    // We can use inverse() function to compute the inverse of a matrix:
    Mat3f inv_mat1 = inverse(mat1);
    spdlog::info("The inverse of mat1 is \n{}", inv_mat1);

    spdlog::info("============== Part 1.4 ==============");
    // =============================================================================================
    // Part 1.4: Multiply mat1 with its inverse, print out the result.
    // =============================================================================================
    // TODO: Write your code below.

    spdlog::info("\n{}", mul(mat1, inverse(mat1)));

    // =============================================================================================
    spdlog::info("Your answer should be:  \n{{1, 0, 0},\n {0, 1, 0},\n {0, 0, 1}}");
    spdlog::info("========== End of Part 1.4 ===========");

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 2: transformations\n"
        "----------------------------------------------"
    );
    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 2.1:\n"
        "Write the 3x3 transformation matrix for a 120 degree clockwise "
        "rotation in 2D\n"
        "and output it with `spdlog::info`:\n"
        "----------------------------------------------"
    );

    // Rotation CCW = {{cos, sin, 0}, {-sin, cos, 0}, {0, 0, 1}}
    // Rotation CW  = {cos, -sin, 0}, {sin, cos, 0}, {0, 0, 1}}
    // So...
    // Rotation 120 CW = {{cos(120), -sin(120), 0}, {sin(120), cos(120), 0, {0, 0, 1}}

    Mat3f rotation_matrix{{-(1.0f/2), -(sqrt(3)/2), 0}, {(sqrt(3)/2), -(1.0f/2), 0}, {0, 0, 1}}; // TODO: Your rotation matrix goes here
    spdlog::info("\n{}", rotation_matrix);

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Problem 2.2:\n"
        "Write the 4x4 transformation matrix to move a 3D point by (11,5,-4)\n"
        "and output it with `spdlog::info`:\n"
        "----------------------------------------------"
    );

    Mat4f translation_matrix{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {11, 5, -4, 1}}; // TODO: Your translation matrix goes here
    spdlog::info("\n{}", translation_matrix);

    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Part 2.3:\n"
        "In computer graphics, we often need to map points in one rectangle to a "
        "new rectangle\n"
        "area. Suppose the bottom left corner and top right corner of original "
        "rectangle are (1,6) and (4,8).\n"
        "The bottom left corner and top right corner of new rectangle are (-1,0) "
        "and (0,4). This can be\n"
        "achieved by a sequence of three steps:\n"
        "   (a) Move point (1,6) to the origin\n"
        "   (b) Scale the rectangle so that its width (along x-axis) is 4 and "
        "height (along y-axis) is 1.\n"
        "   (c) Rotate the rectangle 90 degrees counterclockwise\n"
        "Write the 3x3 transformation matrix for each step (a,b,c) and the "
        "multiplication result in the correct\n"
        "order of the three matrices, and output them with `spdlog::info`:\n"
        "----------------------------------------------"
    );
    Mat3f move_to_origin{{1, 0, 0}, {0, 1, 0}, {-1, -6, 1}}; // TODO: Your matrix for step (a) goes here
    Mat3f scale{{(4.0f/3), 0, 0}, {0, 0.5f, 0}, {0, 0, 1}};          // TODO: Your matrix for step (b) goes here
    Mat3f rotate{{0, 1, 0,}, {-1, 0, 0}, {0, 0, 1}};         // TODO: Your matrix for step (c) goes here
    Mat3f combined = mul(scale, move_to_origin); 
    combined = mul(rotate, combined);      // TODO: Your combined matrix goes here, multiply the three
                          // matrices in the correct order

    spdlog::info("\nMove to origin:\n{}", move_to_origin);
    spdlog::info("\nScale:\n{}", scale);
    spdlog::info("\nRotate:\n{}", rotate);
    spdlog::info("\nCombined:\n{}", combined);

    return 0;
}

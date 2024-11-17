#include "muni/common.h"
#include "muni/image.h"

using namespace muni;

Vec2f de_casteljau(const std::vector<Vec2f>& control_points, float t) {
    
    if (control_points.size() == 1){return control_points[0];}

    std::vector<Vec2f> new_points;

    for (size_t i = 0; i < control_points.size() - 1; ++i){

        Vec2f p1 = (1 - t) * control_points[i] + t * control_points[i + 1];
        new_points.push_back(p1);
    }

    return de_casteljau(new_points, t);
}

void draw_bezier_curve(const std::vector<Vec2f>& control_points, Image& image, float step) {
    image.reset();

    // Draw the control points in white.
    for (const auto& control_point : control_points) {
        int x = static_cast<int>(control_point[0] * image.width + 0.5f);
        int y = static_cast<int>(control_point[1] * image.height + 0.5f);
        for (int xx = x - 1; xx <= x + 1; xx++) {
            for (int yy = y - 1; yy <= y + 1; yy++) {
                if (xx >= 0 && xx < image.width && yy >= 0 && yy < image.height) {
                    image(xx, yy) = Vec3f(1.0f, 1.0f, 1.0f);
                }
            }
        }
    }

        
        for (float t = 0; t <= 1; t += step){
            Vec2f curve_point = de_casteljau(control_points, t);
            int x = static_cast<int>(curve_point.x * image.width + 0.5f);
            int y = static_cast<int>(curve_point.y * image.height + 0.5f);

            image(x,y) = Vec3f(1.0f, 0.0f, 0.0f);    
        }
    
}

int main(int argc, char** argv) {
    spdlog::info(
        "\n"
        "----------------------------------------------\n"
        "Welcome to CS 180/280 Assignment 5\n"
        "----------------------------------------------"
    );

    constexpr int width = 512;
    constexpr int height = 512;
    Image image {
        .width = width,
        .height = height,
        .pixels = std::vector<Vec3f>(width * height, Vec3f(0.0f, 0.0f, 0.0f))
    };

    constexpr float bezier_step = 1e-3f;

    std::vector<Vec2f> control_points_0 = {
        Vec2f { 0.2f, 0.4f },
        Vec2f { 0.3f, 0.8f },
        Vec2f { 0.5f, 0.9f },
        Vec2f { 0.8f, 0.2f }
    };
    std::vector<Vec2f> control_points_1 = {
        Vec2f { 0.2f, 0.4f },
        Vec2f { 0.3f, 0.8f },
        Vec2f { 0.5f, 0.3f },
        Vec2f { 0.8f, 0.6f }
    };

    // ========== Uncomment these code to check your Part 1 implementation ==========
    // for (float t = 0.0f; t <= 1.0f; t += 0.1f) {
    //     Vec2f point_0 = de_casteljau(control_points_0, t);
    //     Vec2f point_1 = de_casteljau(control_points_1, t);
    //     spdlog::info("t = {:.1f}, point_0 = ({:.2f}, {:.2f}), point_1 = ({:.2f}, {:.2f})",
    //                  t, point_0[0], point_0[1], point_1[0], point_1[1]);
    // }
    // ==============================================================================

    // The following output is expected:
    // t = 0.0, point_0 = (0.20, 0.40), point_1 = (0.20, 0.40)
    // t = 0.1, point_0 = (0.23, 0.51), point_1 = (0.23, 0.49)
    // t = 0.2, point_0 = (0.27, 0.60), point_1 = (0.27, 0.55)
    // t = 0.3, point_0 = (0.32, 0.67), point_1 = (0.32, 0.56)
    // t = 0.4, point_0 = (0.37, 0.70), point_1 = (0.37, 0.56)
    // t = 0.5, point_0 = (0.42, 0.71), point_1 = (0.42, 0.54)
    // t = 0.6, point_0 = (0.49, 0.69), point_1 = (0.49, 0.52)
    // t = 0.7, point_0 = (0.56, 0.63), point_1 = (0.56, 0.50)
    // t = 0.8, point_0 = (0.63, 0.53), point_1 = (0.63, 0.50)
    // t = 0.9, point_0 = (0.71, 0.39), point_1 = (0.71, 0.53)

    draw_bezier_curve(control_points_0, image, bezier_step);
    image.save("bezier_curve_0.png");
    draw_bezier_curve(control_points_1, image, bezier_step);
    image.save("bezier_curve_1.png");
    
    return 0;
}

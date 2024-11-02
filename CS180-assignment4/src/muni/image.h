#pragma once
#include <cmath>
#include <vector>
#include <string>
#include <cstdint>

#include "common.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define TINYEXR_USE_STB_ZLIB 1
#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"

namespace muni
{
/** An image is just a 2D array of pixels.
 */
struct Image
{
    int width, height;
    std::vector<Vec3f> pixels;

    /**
     * @brief Load an EXR image from a file using TinyEXR.
     * \param[in] filename The name of the file to load the image from.
     * \return True if the image was loaded successfully, false otherwise.
     */
    bool load_exr(const std::string& filename)
    {
        const char* err = nullptr;
        float* out; // Pointer to the loaded EXR data
        int channels;

        int ret = LoadEXR(&out, &width, &height, filename.c_str(), &err);
        if (ret != TINYEXR_SUCCESS)
        {
            FreeEXRErrorMessage(err); // Free error message if present
            return false;
        }

        channels = 4; // TinyEXR usually loads 4 channels (RGBA)
        pixels.resize(width * height);

        for (int i = 0; i < width * height; ++i)
        {
            pixels[i] = Vec3f(out[i * channels], out[i * channels + 1], out[i * channels + 2]);
        }

        free(out); // Free the loaded EXR data
        return true;
    }

    /**
     * @brief Save the image to a file.
     * \param[in] filename The name of the file to save the image to.
     * \return True if the image was saved successfully, false otherwise.
     */
    bool save(const std::string& filename) const
    {
        // Convert the floating-point data to 8-bit per channel.
        std::vector<uint8_t> data(3 * width * height);
        for (int i = 0; i < width * height; i++)
        {
            for (int j = 0; j < 3; j++)
            {

                data[3 * i + j] = static_cast<uint8_t>(255.0f * std::max(0.0f, std::min(1.0f, pixels[i][j])));
            }
        }
        // Save the image to a png file.
        return stbi_write_png(filename.c_str(), width, height, 3, data.data(), sizeof(uint8_t) * 3 * width) != 0;
    }

    /**
     * @brief Reset the image to all black.
     */
    void reset() { std::fill(pixels.begin(), pixels.end(), Vec3f{0.0f, 0.0f, 0.0f}); }

    // Access the pixel at (x, y).
    // Note that `stb_image_write` uses top-left origin while we use bottom-left, so we need to flip the y-axis.
    Vec3f& operator()(int x, int y) { return pixels[(height - y - 1) * width + x]; }
    const Vec3f& operator()(int x, int y) const { return pixels[(height - y - 1) * width + x]; }
};

} // namespace muni

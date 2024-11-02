{
    files = {
        "src/assignment-4.cpp"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-Wall",
            "-O3",
            "-std=c++20",
            "-Isrc/muni",
            "-isystem",
            "/home/mike/.xmake/packages/s/spdlog/v1.13.0/cca5b628022e49229205fe5617e4245e/include",
            "-isystem",
            "/home/mike/.xmake/packages/s/stb/2024.06.01/1411cb81068a4769b53cf2d71650c82b/include",
            "-isystem",
            "/home/mike/.xmake/packages/s/stb/2024.06.01/1411cb81068a4769b53cf2d71650c82b/include/stb",
            "-isystem",
            "/home/mike/.xmake/packages/l/linalg/v2.2/d050b935567d4e7fa562bb4b897d215d/include",
            "-isystem",
            "/home/mike/.xmake/packages/t/tinyexr/v1.0.9/c5577d93e3a6430bbfaffac89e44cae4/include",
            "-isystem",
            "/home/mike/.xmake/packages/m/miniz/2.2.0/e90355603be8464cb24eeed6a1b7a39e/include",
            "-DNDEBUG"
        }
    },
    depfiles_gcc = "assignment-4.o: src/assignment-4.cpp src/muni/common.h src/muni/image.h\
"
}
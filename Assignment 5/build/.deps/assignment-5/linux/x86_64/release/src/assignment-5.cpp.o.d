{
    files = {
        "src/assignment-5.cpp"
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
            "/home/mike/.xmake/packages/s/stb/2024.06.01/1411cb81068a4769b53cf2d71650c82b/include",
            "-isystem",
            "/home/mike/.xmake/packages/s/stb/2024.06.01/1411cb81068a4769b53cf2d71650c82b/include/stb",
            "-isystem",
            "/home/mike/.xmake/packages/s/spdlog/v1.13.0/cca5b628022e49229205fe5617e4245e/include",
            "-isystem",
            "/home/mike/.xmake/packages/l/linalg/v2.2/d050b935567d4e7fa562bb4b897d215d/include",
            "-DNDEBUG"
        }
    },
    depfiles_gcc = "assignment-5.o: src/assignment-5.cpp src/muni/common.h src/muni/image.h\
"
}
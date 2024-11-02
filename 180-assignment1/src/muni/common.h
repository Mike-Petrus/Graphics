#pragma once
#include <spdlog/spdlog.h>
#include <linalg.h>

namespace muni {

// linalg::vec<T,M> defines a fixed-length vector containing exactly M elements of type T
template<int N, class T> using Vec = linalg::vec<T, N>;
template<class T> using Vec3 = Vec<3, T>;
using Vec3f = Vec3<float>;

// linalg::mat<T,M,N> defines a fixed-size matrix containing exactly M rows and N columns of type T, in column-major order.
template<class T, int M, int N> using Mat = linalg::mat<T, M, N>;
template<class T> using Mat2 = Mat<T, 2, 2>;
template<class T> using Mat3 = Mat<T, 3, 3>;
template<class T> using Mat4 = Mat<T, 4, 4>;

using Mat2f = Mat2<float>;
using Mat3f = Mat3<float>;
using Mat4f = Mat4<float>;
}  // namespace muni

template<typename T, int N>
struct fmt::formatter<linalg::vec<T, N>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        return underlying_formatter.parse(ctx);
    }
    template<typename FormatContext>
    auto format(const linalg::vec<T, N> &v, FormatContext &ctx) {
        fmt::format_to(ctx.out(), "{{");
        auto it = begin(v);
        while (true) {
            ctx.advance_to(underlying_formatter.format(*it, ctx));
            if (++it == end(v)) {
                fmt::format_to(ctx.out(), "}}");
                break;
            } else {
                fmt::format_to(ctx.out(), ", ");
            }
        }
        return ctx.out();
    }
protected:
    fmt::formatter<T> underlying_formatter;
};

template<typename T, int M, int N>
struct fmt::formatter<linalg::mat<T, M, N>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        return underlying_formatter.parse(ctx);
    }
    template<typename FormatContext>
    auto format(const linalg::mat<T, M, N> &v, FormatContext &ctx) {
        fmt::format_to(ctx.out(), "{{");
        const linalg::mat<T, N, M> &vv = transpose(v);
        auto it = begin(vv);
        while (true) {
            ctx.advance_to(underlying_formatter.format(*it, ctx));
            if (++it == end(vv)) {
                fmt::format_to(ctx.out(), "}}");
                break;
            } else {
                fmt::format_to(ctx.out(), ",\n");
            }
        }
        return ctx.out();

    }
protected:
    fmt::formatter<linalg::vec<T, M>> underlying_formatter;
};

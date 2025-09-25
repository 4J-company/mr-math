#ifndef __MR_COLOR_HPP_
#define __MR_COLOR_HPP_

#include "def.hpp"
#include "mr-math/matr.hpp"
#include "vec.hpp"

namespace mr {
inline namespace math {
  // forward declarations
  struct RGBAColor;
  struct HSVAColor;
  struct HSLAColor;
  struct CMYKColor;
  struct OKLABColor;

  template <typename InputT, typename OutputT>
    OutputT convert(InputT input) {
      static_assert(false, "You have to specify this function for your particular types");
    }

  template <typename ColorDerived>
  struct ColorBase {
  public:
    using ValueT = float;

    // structured binding support
    template <size_t I> requires (I < 4)
      ValueT get(this const ColorDerived &self) { return self._data[I]; }

    operator Vec3f(this ColorDerived &self) noexcept {
      return (Vec3f)self._data;
    }

    operator Vec4f(this ColorDerived &self) noexcept {
      return self._data;
    }

    friend ColorDerived operator+(ColorDerived lhs, const ColorDerived &rhs) noexcept {
      lhs += rhs;
      return lhs;
    }

    ColorDerived &operator+=(const ColorDerived &other) noexcept {
      static_cast<ColorDerived*>(this)->_data += other._data;
      return *(ColorDerived*)this;
    }

    bool operator==(this const ColorDerived &self, const ColorDerived &other) noexcept {
      return self._data == other._data;
    }

    bool equal(this const ColorDerived &self, const ColorDerived &other, ValueT eps = epsilon<ValueT>()) noexcept {
      return self._data.equal(other._data, eps);
    }

    friend std::ostream & operator<<(std::ostream &s, const ColorDerived &color) noexcept {
      Vec4u comps = color._data * 255;
      comps.clamp(0, 256);
      s << '#' << std::hex << std::uppercase << comps[0] << comps[1] << comps[2] << comps[3] << std::nouppercase << std::dec;
      return s;
    }

  };

  // color in RGBA float format
  struct alignas(float) [[nodiscard]] RGBAColor : ColorBase<RGBAColor> {
    friend class ColorBase<RGBAColor>;

  private:
    Vec4f _data;

  public:
    using ValueT = float;

    RGBAColor() = default;
    RGBAColor(const RGBAColor &) = default;
    RGBAColor& operator=(const RGBAColor &) = default;
    RGBAColor(RGBAColor &&) = default;
    RGBAColor& operator=(RGBAColor &&) = default;

    template <std::floating_point T>
      RGBAColor(T r, T g, T b, T a = 1) noexcept
        : _data(r, g, b, a) {}

    RGBAColor(Vec4f rgba) noexcept
      : _data(rgba) {}

    template <std::integral T>
      RGBAColor(T r, T g, T b, T a = 255) noexcept
        : _data(r, g, b, a) {
        assert(r <= 255);
        assert(g <= 255);
        assert(b <= 255);
        assert(a <= 255);
        _data /= 255;
      }

    explicit RGBAColor(uint32_t rgba) noexcept
      : RGBAColor(
        uint8_t((rgba & 0xFF'00'00'00) >> 24),
        uint8_t((rgba & 0x00'FF'00'00) >> 16),
        uint8_t((rgba & 0x00'00'FF'00) >> 8),
        uint8_t((rgba & 0x00'00'00'FF))
      ) {}

    // setters
    void r(ValueT r) noexcept { _data.x(r); }
    void g(ValueT g) noexcept { _data.y(g); }
    void b(ValueT b) noexcept { _data.z(b); }
    void a(ValueT a) noexcept { _data.w(a); }
    void set(size_t i, ValueT value) noexcept {
      assert(i < 4);
      _data.set(i, value);
    }

    // getters
    [[nodiscard]] constexpr ValueT r() const noexcept { return _data.x(); }
    [[nodiscard]] constexpr ValueT g() const noexcept { return _data.y(); }
    [[nodiscard]] constexpr ValueT b() const noexcept { return _data.z(); }
    [[nodiscard]] constexpr ValueT a() const noexcept { return _data.w(); }
    [[nodiscard]] constexpr ValueT operator[](size_t i) const {
      assert(i < 4);
      return _data[i];
    }

    // format conversions
    // TODO: implement using shuffle
    Vec4f argb() const noexcept {
      return {a(), r(), g(), b()};
    }

    Vec4f bgra() const noexcept {
      return {b(), g(), r(), a()};
    }

    Vec4f abgr() const noexcept {
      return {a(), b(), g(), r()};
    }
  };

  struct HSLAColor {
    friend class ColorBase<HSLAColor>;

  private:
    Vec4f _data;

  public:
    using ValueT = float;

    HSLAColor() = default;

    template <std::floating_point T>
      HSLAColor(T h, T s, T l, T a = 1) noexcept
        : _data(h, s, l, a) {}

    HSLAColor(Vec4f hsla) noexcept
      : _data(hsla) {}

    template <std::integral T>
      HSLAColor(T h, T s, T l, T a = 255) noexcept
        : _data(h, s, l, a) {
        assert(h <= 255);
        assert(s <= 255);
        assert(l <= 255);
        assert(a <= 255);
        _data /= 255;
      }

    explicit HSLAColor(uint32_t hsla) noexcept
      : HSLAColor(
        uint8_t((hsla & 0xFF'00'00'00) >> 24),
        uint8_t((hsla & 0x00'FF'00'00) >> 16),
        uint8_t((hsla & 0x00'00'FF'00) >> 8),
        uint8_t((hsla & 0x00'00'00'FF))
      ) {}

    // setters
    void h(ValueT h) noexcept { _data.x(h); }
    void s(ValueT s) noexcept { _data.y(s); }
    void l(ValueT l) noexcept { _data.z(l); }
    void a(ValueT a) noexcept { _data.w(a); }
    void set(size_t i, ValueT value) noexcept {
      assert(i < 4);
      _data.set(i, value);
    }

    // getters
    [[nodiscard]] constexpr ValueT h() const noexcept { return _data.x(); }
    [[nodiscard]] constexpr ValueT s() const noexcept { return _data.y(); }
    [[nodiscard]] constexpr ValueT l() const noexcept { return _data.z(); }
    [[nodiscard]] constexpr ValueT a() const noexcept { return _data.w(); }
    [[nodiscard]] constexpr ValueT operator[](size_t i) const {
      assert(i < 4);
      return _data[i];
    }

  };

  struct HSVAColor {
    friend class ColorBase<HSVAColor>;

  private:
    Vec4f _data;

  public:
    using ValueT = float;

    HSVAColor() = default;

    template <std::floating_point T>
      HSVAColor(T h, T s, T v, T a = 1) noexcept
        : _data(h, s, v, a) {}

    HSVAColor(Vec4f hsva) noexcept
      : _data(hsva) {}

    template <std::integral T>
      HSVAColor(T h, T s, T v, T a = 255) noexcept
        : _data(h, s, v, a) {
        assert(h <= 255);
        assert(s <= 255);
        assert(v <= 255);
        assert(a <= 255);
        _data /= 255;
      }

    explicit HSVAColor(uint32_t hsva) noexcept
      : HSVAColor(
        uint8_t((hsva & 0xFF'00'00'00) >> 24),
        uint8_t((hsva & 0x00'FF'00'00) >> 16),
        uint8_t((hsva & 0x00'00'FF'00) >> 8),
        uint8_t((hsva & 0x00'00'00'FF))
      ) {}

    // setters
    void h(ValueT h) noexcept { _data.x(h); }
    void s(ValueT s) noexcept { _data.y(s); }
    void v(ValueT v) noexcept { _data.z(v); }
    void a(ValueT a) noexcept { _data.w(a); }
    void set(size_t i, ValueT value) noexcept {
      assert(i < 4);
      _data.set(i, value);
    }

    // getters
    [[nodiscard]] constexpr ValueT h() const noexcept { return _data.x(); }
    [[nodiscard]] constexpr ValueT s() const noexcept { return _data.y(); }
    [[nodiscard]] constexpr ValueT v() const noexcept { return _data.z(); }
    [[nodiscard]] constexpr ValueT a() const noexcept { return _data.w(); }
    [[nodiscard]] constexpr ValueT operator[](size_t i) const {
      assert(i < 4);
      return _data[i];
    }

  };

  struct CMYKColor {
    friend class ColorBase<CMYKColor>;

  private:
    Vec4f _data;

  public:
    using ValueT = float;

    CMYKColor() = default;

    template <std::floating_point T>
      CMYKColor(T c, T m, T y, T k) noexcept
        : _data(c, m, y, k) {}

    CMYKColor(Vec4f cmyk) noexcept
      : _data(cmyk) {}

    template <std::integral T>
      CMYKColor(T c, T m, T y, T k) noexcept
        : _data(c, m, y, k) {
        assert(c <= 255);
        assert(m <= 255);
        assert(y <= 255);
        assert(k <= 255);
        _data /= 255;
      }

    explicit CMYKColor(uint32_t cmyk) noexcept
      : CMYKColor(
        uint8_t((cmyk & 0xFF'00'00'00) >> 24),
        uint8_t((cmyk & 0x00'FF'00'00) >> 16),
        uint8_t((cmyk & 0x00'00'FF'00) >> 8),
        uint8_t((cmyk & 0x00'00'00'FF))
      ) {}

    // setters
    void c(ValueT c) noexcept { _data.x(c); }
    void m(ValueT m) noexcept { _data.y(m); }
    void y(ValueT y) noexcept { _data.z(y); }
    void k(ValueT k) noexcept { _data.w(k); }
    void set(size_t i, ValueT value) noexcept {
      assert(i < 4);
      _data.set(i, value);
    }

    // getters
    [[nodiscard]] constexpr ValueT c() const noexcept { return _data.x(); }
    [[nodiscard]] constexpr ValueT m() const noexcept { return _data.y(); }
    [[nodiscard]] constexpr ValueT y() const noexcept { return _data.z(); }
    [[nodiscard]] constexpr ValueT k() const noexcept { return _data.w(); }
    [[nodiscard]] constexpr ValueT operator[](size_t i) const {
      assert(i < 4);
      return _data[i];
    }

  };

  struct OKLABColor {
    friend class ColorBase<OKLABColor>;

  private:
    Vec4f _data;

  public:
    using ValueT = float;

    OKLABColor() = default;

    template <std::floating_point T>
      OKLABColor(T l, T m, T s, T a = 1) noexcept
        : _data(l, m, s, a) {}

    OKLABColor(Vec4f lmsa) noexcept
      : _data(lmsa) {}

    template <std::integral T>
      OKLABColor(T l, T m, T s, T a = 255) noexcept
        : _data(l, m, s, a) {
        assert(l <= 255);
        assert(m <= 255);
        assert(s <= 255);
        assert(a <= 255);
        _data /= 255;
      }

    explicit OKLABColor(uint32_t lmsa) noexcept
      : OKLABColor(
        uint8_t((lmsa & 0xFF'00'00'00) >> 24),
        uint8_t((lmsa & 0x00'FF'00'00) >> 16),
        uint8_t((lmsa & 0x00'00'FF'00) >> 8),
        uint8_t((lmsa & 0x00'00'00'FF))
      ) {}

    // setters
    void l(ValueT l) noexcept { _data.x(l); }
    void m(ValueT m) noexcept { _data.y(m); }
    void s(ValueT s) noexcept { _data.z(s); }
    void a(ValueT a) noexcept { _data.w(a); }
    void set(size_t i, ValueT value) noexcept {
      assert(i < 4);
      _data.set(i, value);
    }

    // getters
    [[nodiscard]] constexpr ValueT l() const noexcept { return _data.x(); }
    [[nodiscard]] constexpr ValueT m() const noexcept { return _data.y(); }
    [[nodiscard]] constexpr ValueT s() const noexcept { return _data.z(); }
    [[nodiscard]] constexpr ValueT a() const noexcept { return _data.w(); }
    [[nodiscard]] constexpr ValueT operator[](size_t i) const {
      assert(i < 4);
      return _data[i];
    }
  };

  template <>
    inline OKLABColor convert<RGBAColor, OKLABColor>(RGBAColor rgba) {
      auto v = static_cast<Vec3f>(rgba) * Matr3f {
        Matr3f::RowT {0.4122214708f, 0.2119034982f, 0.0883024619f},
        Matr3f::RowT {0.5363325363f, 0.6806995451f, 0.2817188376f},
        Matr3f::RowT {0.0514459929f, 0.1073969566f, 0.6299787005f},
      };

      // TODO: rewrite using mr-math adaptation of <cmath>
      float l_ = cbrtf(v[0]);
      float m_ = cbrtf(v[1]);
      float s_ = cbrtf(v[2]);

      return {
          0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
          1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
          0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_,
          rgba.a()
      };
    }

namespace literals {

  inline RGBAColor operator""_rgba(unsigned long long value) {
    assert(value <= 0xFF'FF'FF'FF);
    return RGBAColor{static_cast<uint32_t>(value)};
  }

} // namespace literals
} // namespace math
} // namespace mr

#ifdef __cpp_structured_bindings
// specializations for structured binding support
namespace std
{
  template <>
    struct tuple_size<mr::RGBAColor>
      : std::integral_constant<size_t, 4> {};

  template <size_t I>
    struct tuple_element<I, mr::RGBAColor> {
      using type = mr::RGBAColor::ValueT;
    };
}
#endif // __cpp_structured_bindings

#endif // __MR_COLOR_HPP_

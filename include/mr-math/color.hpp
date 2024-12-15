#ifndef __color_hpp_
#define __color_hpp_

#include "def.hpp"
#include "vec.hpp"

namespace mr {

  // color in RGBA float format
  struct [[nodiscard]] Color {
  public:
    using ValueT = float;

    template <std::floating_point T>
      constexpr Color(T r, T g, T b, T a = 1) noexcept
        : _data{Vec4f{r, g, b, a}} {}

    constexpr Color(Vec4f rgba) noexcept
      : _data{rgba} {}

    template <std::integral T>
      constexpr Color(T r, T g, T b, T a = 255) noexcept
        : _data{r, g, b, a} {
        assert(r <= 255);
        assert(g <= 255);
        assert(b <= 255);
        assert(a <= 255);
        _data /= 255;
      }

    constexpr explicit Color(uint32_t rgba) noexcept
      : Color(
        uint8_t((rgba & 0xFF'00'00'00) >> 24),
        uint8_t((rgba & 0x00'FF'00'00) >> 16),
        uint8_t((rgba & 0x00'00'FF'00) >> 8),
        uint8_t((rgba & 0x00'00'00'FF))
      ) {}

    // setters
    constexpr void r(ValueT r) noexcept { _data.x(r); }
    constexpr void g(ValueT g) noexcept { _data.y(g); }
    constexpr void b(ValueT b) noexcept { _data.z(b); }
    constexpr void a(ValueT a) noexcept { _data.w(a); }
    constexpr void set(size_t i, ValueT value) noexcept {
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

    // structured binding support
    template <size_t I> requires (I < 4)
      constexpr ValueT get() const { return _data[I]; }

    constexpr operator Vec4f() noexcept {
      return _data;
    }

    // format conversions
    // TODO: implement using shuffle
    constexpr Vec4f argb() const noexcept {
      return {a(), r(), g(), b()};
    }

    constexpr Vec4f bgra() const noexcept {
      return {b(), g(), r(), a()};
    }

    constexpr Vec4f abgr() const noexcept {
      return {a(), b(), g(), r()};
    }

    friend Color operator+(Color lhs, const Color &rhs) noexcept {
      lhs += rhs;
      return lhs;
    }

    Color &operator+=(const Color &other) noexcept {
      _data += other._data;;
      return *this;
    }

    constexpr bool operator==(const Color &other) const noexcept {
      return _data == other._data;
    }

    constexpr bool equal(const Color &other, ValueT eps = epsilon<ValueT>()) const noexcept {
      return _data.equal(other._data, eps);
    }

    friend std::ostream & operator<<(std::ostream &s, const Color &color) noexcept {
      Vec4u comps = color._data * 255;
      comps.clamp(0, 256);
      s << '#' << std::hex << std::uppercase << comps[0] << comps[1] << comps[2] << comps[3] << std::nouppercase << std::dec;
      return s;
    }

  private:
    Vec4f _data;
  };

namespace literals {

  constexpr Color operator"" _rgba(unsigned long long value) {
    assert(value <= 0xFF'FF'FF'FF);
    return Color{static_cast<uint32_t>(value)};
  }

} // namespace literals

} // namespace mr

#ifdef __cpp_structured_bindings
// specializations for structured binding support
namespace std
{
  template <>
    struct tuple_size<mr::Color>
      : std::integral_constant<size_t, 4> {};

  template <size_t I>
    struct tuple_element<I, mr::Color> {
      using type = mr::Color::ValueT;
    };
}
#endif // __cpp_structured_bindings

#endif // __color_hpp_

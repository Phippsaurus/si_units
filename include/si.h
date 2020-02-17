#if !defined(SI_H)
#define SI_H

#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>

namespace si_units {

struct s {
  static std::string name() { return {"s"}; }
};

struct m {
  static std::string name() { return {"m"}; }
};

struct kg {
  static std::string name() { return {"kg"}; }
};

struct N {
  static std::string name() { return {"N"}; }
};

template <typename Unit, int64_t Exponent> struct pow {
  static std::string name() {
    if constexpr (Exponent == 0) {
      return {""};
    } else if constexpr (Exponent == 1) {
      return Unit::name();
    } else {
      return Unit::name() + '^' + std::to_string(Exponent);
    }
  }
};

template <typename... Units> struct units {};

template <typename... Units> struct si {
  long double val;

  friend std::ostream &operator<<(std::ostream &stream, si measure) {
    stream << measure.val;
    return ((stream << ' ' << Units::name()), ...);
  }

  auto operator+(si rhs) -> si { return {val + rhs.val}; }

  auto operator-(si rhs) -> si { return {val - rhs.val}; }

  auto operator*(double rhs) -> si { return {val * rhs}; }

  friend auto operator*(double lhs, si rhs) -> si { return {lhs * rhs.val}; }

  auto operator/(double rhs) -> si { return {val / rhs}; }
};

constexpr si<s> operator"" _s(long double val) { return si<s>{val}; }

constexpr si<m> operator"" _m(long double val) { return si<m>{val}; }

constexpr si<kg> operator"" _kg(long double val) { return si<kg>{val}; }

constexpr si<N> operator"" _N(long double val) { return si<N>{val}; }

template <int64_t Count> struct counter {
  constexpr static int64_t value = Count;
  template <int64_t Other>
  constexpr auto operator+(counter<Other> other) -> counter<Count + Other>;
};

template <typename Unit, int64_t Exponent, typename... Units>
constexpr auto count(units<Unit> unit, units<pow<Unit, Exponent>, Units...>)
    -> decltype(count(unit, units<Units...>()) + counter<Exponent>());

template <typename Unit, typename... Units>
constexpr auto count(units<Unit> unit, units<Unit, Units...>)
    -> decltype(count(unit, units<Units...>()) + counter<1>());

template <typename Unit, typename First, typename... Units,
          typename = std::enable_if_t<!std::is_same<Unit, First>::value>>
constexpr auto count(units<Unit> unit, units<First, Units...>)
    -> decltype(count(unit, units<Units...>()));

template <typename Unit>
constexpr auto count(units<Unit> unit, units<>) -> counter<0>;

template <typename Unit, typename First, typename... Units,
          typename = std::enable_if_t<!std::is_same<Unit, First>::value>>
auto remove(units<Unit> unit, units<First, Units...>)
    -> decltype(concat(units<First>(), remove(unit, units<Units...>())));

template <typename Unit, typename... Units>
auto remove(units<Unit> unit, units<Unit, Units...>)
    -> decltype(remove(unit, units<Units...>()));

template <typename Unit, int64_t Exponent, typename... Units>
auto remove(units<Unit> unit, units<pow<Unit, Exponent>, Units...>)
    -> decltype(remove(unit, units<Units...>()));

template <typename Unit> auto remove(units<Unit>, units<>) -> units<>;

template <typename Unit, typename... Units>
auto concat(units<Unit>, units<Units...>) -> units<Unit, Units...>;

template <typename Unit, typename... Units>
auto concat(si<Unit>, si<Units...>) -> si<Unit, Units...>;

template <typename Unit, int64_t Exponent, typename... Units>
auto over(units<pow<Unit, Exponent>, Units...>)
    -> decltype(concat(units<pow<Unit, -Exponent>>(), over(units<Units...>())));

template <typename Unit, int64_t Exponent>
auto over(units<pow<Unit, Exponent>>) -> units<pow<Unit, -Exponent>>;

template <typename Unit, typename... Units>
auto simplify(units<Unit, Units...> unit) -> decltype(
    concat(units<pow<Unit, decltype(count(units<Unit>(), unit))::value>>(),
           simplify(remove(units<Unit>(), units<Units...>()))));

template <typename Unit, int64_t Exponent>
auto simplify(units<pow<Unit, Exponent>> unit) -> units<pow<Unit, Exponent>>;

template <typename Unit> auto simplify(units<Unit> unit) -> units<pow<Unit, 1>>;

auto simplify(units<> unit) -> units<>;

template <typename... Units> auto to_si(units<Units...>) -> si<Units...>;

template <typename... Units, typename... OtherUnits>
auto operator*(si<Units...> lhs, si<OtherUnits...> rhs)
    -> decltype(to_si(simplify(units<Units..., OtherUnits...>()))) {
  return {lhs.val * rhs.val};
}

template <typename... Units, typename... OtherUnits>
auto operator/(si<Units...> lhs, si<OtherUnits...> rhs)
    -> decltype(to_si(concat(units<Units...>(),
                             over(simplify(units<OtherUnits...>()))))) {
  return {lhs.val / rhs.val};
}

template <typename... Units>
auto operator/(double lhs, si<Units...> rhs)
    -> decltype(over(simplify(units<Units...>()))) {
  return {lhs / rhs.val};
}

template <typename... Units>
auto operator+(si<Units...> lhs, si<Units...> rhs) -> si<Units...> {
  return {lhs.val + rhs.val};
}
template <typename... Units>
auto operator-(si<Units...> lhs, si<Units...> rhs) -> si<Units...> {
  return {lhs.val - rhs.val};
}

} // namespace si_units

#endif

#include <tuple>
#include <utility>
#include <type_traits>
#include <iostream>

using namespace std;

// prototypes begin
template <typename... Ts>
constexpr double tuple_sum(const tuple<Ts...>& tpl);
template <typename... Ts, size_t... Ind>
constexpr double get_sum(const tuple<Ts...>& tpl, index_sequence<Ind...>);
template <typename T>
constexpr double sum(T el);
template <typename T, typename... U>
constexpr double sum(T el, U... elems);
template <typename T>
constexpr enable_if_t<!is_arithmetic<T>::value, double> value(T);
constexpr double value(char);
template <typename T>
constexpr enable_if_t<is_arithmetic<T>::value, double> value(T el);
// prototypes end

template <typename... Ts>
constexpr double tuple_sum(const tuple<Ts...>& tpl) {
  constexpr size_t size = tuple_size<tuple<Ts...>>();
  return get_sum(tpl, make_index_sequence<size>());
}

template <typename... Ts, size_t... Ind>
constexpr double get_sum(const tuple<Ts...>& tpl, index_sequence<Ind...>) {
  return sum(get<Ind>(tpl)...);
}

template <typename T>
constexpr double sum(T el) {
  return value(el);
}

template <typename T, typename... U>
constexpr double sum(T el, U... elems) {
  return value(el) + sum(elems...);
}

template <typename T>
constexpr enable_if_t<!is_arithmetic<T>::value, double> value(T) {
  return 0.0;
}

constexpr double value(char) {
  return 0.0;
}

template <typename T>
constexpr enable_if_t<is_arithmetic<T>::value, double> value(T el) {
  return static_cast<double>(el);
}

int main() {
  constexpr double result = tuple_sum(tuple<int, char, float, const char*>(2, 'c', 3.14, "hello"));
  cout << result << endl;
}

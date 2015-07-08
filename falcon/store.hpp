/* The MIT License (MIT)

Copyright (c) 2015 jonathan poelen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef FALCON_STORE_HPP
#define FALCON_STORE_HPP

#include <utility>
#include <type_traits>


namespace falcon {

constexpr struct default_item_t {
  constexpr default_item_t() noexcept {}
} default_item;

constexpr struct ignore_item_t {
  constexpr ignore_item_t() noexcept {}
} ignore_item;

namespace detail_ {
  template<class T>
  struct store_item
  {
    constexpr store_item() noexcept(noexcept(T()))
    : x()
    {}

    constexpr store_item(default_item_t) noexcept(noexcept(T()))
    : x()
    {}

    constexpr store_item(ignore_item_t) noexcept(noexcept(T()))
    {}

    template<class U>
    constexpr store_item(U && x_)
    : x(std::forward<U>(x_))
    {}

    T x;
  };
}

template<class... Ts>
struct store : detail_::store_item<Ts>... // error: you have defined a store with a type present more than once
{
  store() = default;
  store(store &&) = default;
  store(store const &) = default;

  template<class... Us>
  constexpr store(Us && ... elems)
  : detail_::store_item<Ts>{std::forward<Us>(elems)}...
  {}

  store & operator=(store &&) = default;
  store & operator=(store const &) = default;
};

namespace detail_ {
  template<class... Ts>
  struct make_strict_store : detail_::store_item<typename std::remove_cv<Ts>::type>... // error: you have defined a store with a type present more than once 
  { using type = ::falcon::store<Ts...>; };
}
template<class... Ts>
using strict_store = typename detail_::make_strict_store<Ts...>::type;

template<class T, class... Ts>
constexpr T & get(store<Ts...> & store) noexcept
{ return static_cast<detail_::store_item<T>&>(store).x; }

template<class T, class... Ts>
constexpr T && get(store<Ts...> && store) noexcept
{ return std::move(static_cast<detail_::store_item<T>&>(store).x); }

template<class T, class... Ts>
constexpr T const & get(store<Ts...> const & store) noexcept
{ return static_cast<detail_::store_item<T> const &>(store).x; }

template<class Fn, class... Ts>
Fn apply_from_store(store<Ts...> const & store, Fn fn)
{
  void(std::initializer_list<int>{(void(fn(get<Ts>(store))), 1)...});
  return fn;
}

template<class Fn, class... Ts>
Fn apply_from_store(store<Ts...> && store, Fn fn)
{
  void(std::initializer_list<int>{(void(fn(get<Ts>(std::move(store)))), 1)...});
  return fn;
}

template<class Fn, class... Ts>
Fn apply_from_store(store<Ts...> & store, Fn fn)
{
  void(std::initializer_list<int>{(void(fn(get<Ts>(store))), 1)...});
  return fn;
}

}

#endif // FALCON_STORE_HPP

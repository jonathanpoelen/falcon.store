#include "falcon/store_set.hpp"
#include <type_traits>

using namespace falcon;

using store_int_vint = store<int, volatile int>;

static_assert(std::is_same<decltype(get<int>(store_int_vint())), int&&>::value, "should be int&&");
static_assert(std::is_same<decltype(get<int volatile >(store_int_vint())), int volatile &&>::value, "should be volatile int&&");
static_assert(std::is_same<decltype(get<int>(std::declval<store_int_vint&>())), int&>::value, "should be int&");
static_assert(std::is_same<decltype(get<int volatile>(std::declval<store_int_vint&>())), int volatile&>::value, "should be int volatile&");
static_assert(std::is_same<decltype(get<int>(std::declval<store_int_vint const &>())), int const&>::value, "should be int const &");
static_assert(std::is_same<decltype(get<int volatile >(std::declval<store_int_vint const&>())), int volatile const&>::value, "should be int volatile const &");

using store_int_long = store<int, long>;
static_assert(get<int>(store_int_long{2,4}) == 2, "should be 2");
//static_assert(get<int volatile>(store_int_long{2,4}) == 4, "should be 4");
static_assert(get<int>(store_int_long{default_item,4}) == 0, "should be 0");
static_assert(get<int>(store_int_long{}) == 0, "should be 0");

static_assert(std::is_same<decltype(store_int_vint() = store_int_vint()), store_int_vint&>::value, "should be store_int_vint&");

struct Accu {
  int n = 0;
  template<class T>
  void operator()(T const & x) { n += x; }
};

int main(){
  store_int_vint store;
  get<int>(store) = 2;
  get<int volatile>(store) = 3;
  if (5 != apply_from_store(store, Accu{}).n) {
    throw 0;
  }
  store = store;
}

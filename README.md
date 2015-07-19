[![Build Status](https://travis-ci.org/jonathanpoelen/falcon.store.svg?branch=master)](https://travis-ci.org/jonathanpoelen/falcon.store)

# Falcon.store
Store of types with a type present once

Header only.


# Documentation

All Classes and Functions are in the namespace `falcon`.

## Classes
- `store<T...>`: The type of store. The conpilation fails if a type present more than once
- `strict_store<T...>`: Alias for store<T...>, but the compilation fails if the result `std::remove_cv<T>::type` present more than once.

## Accessors
- `T & get<T>(store<T...> &)`: Extract the type item from the store
- `T && get<T>(store<T...> &&)`: idem
- `T const& get<T>(store<T...> const &)`: idem

## Utils
- `Fn apply_from_store(store<T...> &, Fn fn)`: apply `fn` on each item of store.
- `Fn apply_from_store(store<T...> &&, Fn fn)`: idem
- `Fn apply_from_store(store<T...> const &, Fn fn)`: idem

#include <functional>
#include <string>
#include <iostream>
#include "tuple_iterator.h"

template <class ... Fs>
struct overload : Fs... {
  overload(Fs&&... fs) : Fs{fs}... {}
  using Fs::operator()...;
};

template <class ... Fs>
struct overload_unref : overload<Fs...> {
  overload_unref(Fs&&... fs) 
    : overload<Fs...>{std::forward<Fs>(fs)...}      
  {}

  using overload<Fs...>::operator();

  template <class T>
  auto operator()(std::reference_wrapper<T> rw){
    return (*this)(rw.get());
  }
};


int main() {
  std::tuple<int, std::string, double> tup{42, "foo", 3.14};
  auto print_elem = overload_unref(
    [](int i) { std::cout << "int: " << i << '\n'; },
    [](std::string const& s) { std::cout << "string: " << s << '\n'; },
    [](double d) { std::cout << "double: " << d << '\n'; }
  );

  for (auto const& elem : to_range(tup)) { 
    std::visit(print_elem, elem);
  }

  std::visit(print_elem, to_range(tup)[1]);
}


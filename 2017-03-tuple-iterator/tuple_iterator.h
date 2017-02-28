#include <tuple>
#include <functional>
#include <variant>
#include <array>

template <typename Tup, typename R, typename F, std::size_t... Idxs>
struct tuple_runtime_access_table {
  using tuple_type = Tup;
  using return_type = R;
  using converter_fun = F;

  template <std::size_t N>
  static return_type access_tuple(tuple_type& t, converter_fun& f) {
    return f(std::get<N>(t));
  }

  using accessor_fun_ptr = return_type(*)(tuple_type&, converter_fun&);
  const static auto table_size = sizeof...(Idxs);

  constexpr static std::array<accessor_fun_ptr, table_size> lookup_table = {
    {&access_tuple<Idxs>...}
  };
};


template <typename R, typename Tup, typename F, std::size_t... Idxs>
auto call_access_function(Tup& t, std::size_t i, F f, std::index_sequence<Idxs...>) {
  auto& table = tuple_runtime_access_table<Tup, R, F, Idxs...>::lookup_table;
  auto* access_function = table[i];
  return access_function(t, f);
}



template <typename Tup> struct common_tuple_access;

template <typename... Ts>
struct common_tuple_access<std::tuple<Ts...>> {
  using type = std::variant<std::reference_wrapper<Ts>...>;
};

template <typename T1, typename T2>
struct common_tuple_access<std::pair<T1, T2>> {
  using type = std::variant<std::reference_wrapper<T1>, std::reference_wrapper<T2>>;
};

template <typename T, auto N>
struct common_tuple_access<std::array<T, N>> {
  using type = std::variant<std::reference_wrapper<T>>;
};

template <typename Tup>
using common_tuple_access_t = typename common_tuple_access<Tup>::type;
    


template <typename Tup>
auto runtime_get(Tup& t, std::size_t i) {
  return call_access_function<common_tuple_access_t<Tup>>(
    t, i, 
    [](auto & element){ return std::ref(element); },
    std::make_index_sequence<std::tuple_size_v<Tup>>{}
  );
}



template <typename Tup> class tuple_iterator {
  Tup& t;
  size_t i;
public:
  tuple_iterator(Tup& tup, size_t idx)
    : t{tup}, i{idx} 
  {}
     
  tuple_iterator& operator++() { 
    ++i; return *this; 
  }
  bool operator==(tuple_iterator const& other) const {
    return std::addressof(other.t) == std::addressof(t)
      && other.i == i;
  }
      
  bool operator!=(tuple_iterator const& other) const {
    return !(*this == other);
  }

  auto operator*() const{ 
    return runtime_get(t, i); 
  }
};

template <typename Tup>
class to_range {
  Tup& t;
public:    
  to_range(Tup& tup) : t{tup}{}
  auto begin() {
    return tuple_iterator{t, 0};
  }
  auto end() {
    return tuple_iterator{t, std::tuple_size_v<Tup>};
  }
         
  auto operator[](std::size_t i){
    return runtime_get(t, i);
  }
};
        


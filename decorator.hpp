#pragma once

#include <any>
#include <cassert>
#include <map>
#include <tuple>
#include <type_traits>

#include "traits.hpp"

namespace deco {

template <class Fn, template <class> class DecoType>
class Decorate {
  class Interface {
    template <class>
    struct ArgsHelper {};

    template <class... Args>
    struct ArgsHelper<traits::ArgList<traits::Dummy&, Args...>> {
      using type = traits::ArgList<Args...>;
    };

    static_assert(traits::IsDecoratable<Fn>::value);

   public:
    template <class... Args>
    auto Call(Args&&... args) const noexcept(noexcept(static_cast<Fn&>(*self_).Call(*self_, std::forward<Args>(args)...))) {
      return static_cast<Fn&>(*self_).Call(*self_, std::forward<Args>(args)...);
    }

    explicit Interface(Decorate* self) noexcept : self_(self) {}

    using return_type = typename traits::SimpleFunctorTraits2<Fn>::return_type;
    using args_type = typename ArgsHelper<typename traits::SimpleFunctorTraits2<Fn>::args_type>::type;

   private:
    Decorate* self_;
  };

  using decorator_type = DecoType<Interface>;

 public:
  template <class... Args>
  auto operator()(Args&&... args) noexcept(noexcept(decorator_(Interface(this), std::forward<Args>(args)...))) {
    return decorator_(Interface(this), std::forward<Args>(args)...);
  }

 protected:
  Decorate() noexcept(noexcept(decorator_type(Interface(this)))) : decorator_(Interface(this)) {}

 private:
  decorator_type decorator_;
};

template <class Interface>
class Memorization {
 public:
  template <class... Args>
  auto operator()(const Interface& interface, Args&&... args) {
    using storage_type = std::map<typename Interface::args_type::tuple_type, typename Interface::return_type>;
    storage_type* real_storage = std::any_cast<storage_type>(&storage_);
    assert(real_storage != nullptr);
    auto key = typename Interface::args_type::tuple_type(args...);
    auto it = real_storage->find(key);
    if (it != real_storage->end()) {
      return it->second;
    }
    return (*real_storage)[key] = interface.Call(std::forward<Args>(args)...);
  }

  explicit Memorization(const Interface& interface) {
    storage_ = std::map<typename Interface::args_type::tuple_type, typename Interface::return_type>{};
  }

 private:
  std::any storage_;
};
}  // namespace deco

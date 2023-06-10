#pragma once

#include <tuple>
#include <type_traits>

namespace deco::traits {

template <class... Tps>
struct ArgList {
  using tuple_type = std::tuple<Tps...>;
};

struct Dummy;

template <class Op>
struct MemberFunctionPointerTraits {};

template <class R, class Class, class... Args>
struct MemberFunctionPointerTraits<R (Class::*)(Args...)> {
  using class_type = Class;
  using return_type = R;
  using args_type = ArgList<Args...>;
};

template <class R, class Class, class... Args>
struct MemberFunctionPointerTraits<R (Class::*)(Args...) const> {
  using class_type = Class;
  using return_type = R;
  using args_type = ArgList<Args...>;
};

template <class R, class Class, class... Args>
struct MemberFunctionPointerTraits<R (Class::*)(Args...) volatile> {
  using class_type = Class;
  using return_type = R;
  using args_type = ArgList<Args...>;
};

template <class R, class Class, class... Args>
struct MemberFunctionPointerTraits<R (Class::*)(Args...) const volatile> {
  using class_type = Class;
  using return_type = R;
  using args_type = ArgList<Args...>;
};

template <class R, class Class, class... Args>
struct MemberFunctionPointerTraits<R (Class::*)(Args...) noexcept> {
  using class_type = Class;
  using return_type = R;
  using args_type = ArgList<Args...>;
};

template <class R, class Class, class... Args>
struct MemberFunctionPointerTraits<R (Class::*)(Args...) const noexcept> {
  using class_type = Class;
  using return_type = R;
  using args_type = ArgList<Args...>;
};

template <class R, class Class, class... Args>
struct MemberFunctionPointerTraits<R (Class::*)(Args...) volatile noexcept> {
  using class_type = Class;
  using return_type = R;
  using args_type = ArgList<Args...>;
};

template <class R, class Class, class... Args>
struct MemberFunctionPointerTraits<R (Class::*)(Args...) const volatile noexcept> {
  using class_type = Class;
  using return_type = R;
  using args_type = ArgList<Args...>;
};

template <class F, class = void>
struct SimpleFunctorTraits2;

template <class F>
struct SimpleFunctorTraits2<F, decltype((void)&F::template Call<Dummy>)>
    : MemberFunctionPointerTraits<decltype(&F::template Call<Dummy>)> {};

template <class Tp>
auto IsDecoratableImpl(...) -> std::false_type;

template <class Tp>
auto IsDecoratableImpl(int) -> decltype(SimpleFunctorTraits2<Tp>{}, std::true_type{});

template <class Tp>
struct IsDecoratable : decltype(IsDecoratableImpl<Tp>(0)) {};

}  // namespace decorator
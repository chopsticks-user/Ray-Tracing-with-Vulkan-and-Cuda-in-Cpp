#ifndef NEKO_CORE_MACRO_UTILS_HPP
#define NEKO_CORE_MACRO_UTILS_HPP

#include "CoreDefines.hpp"

#define NKM_CONCAT_HELPER(x, y) x##y
#define NKM_CONCAT(x, y) NKM_CONCAT_HELPER(x, y)

#define NKM_UP_NAMESPACE_NAME_PREFIX nekoUP_
#define NKM_UP_DEFAULT_NAMESPACE_DEFINE_HELPER(name) namespace name
#define NKM_UP_DEFAULT_NAMESPACE_DEFINE \
  NKM_UP_DEFAULT_NAMESPACE_DEFINE_HELPER(NKM_UP_NAMESPACE_NAME_PREFIX)

// #define NKM_UP_NAME test

#define NKM_NP NKM_CONCAT(NKM_UP_DEFAULT_NAMESPACE_DEFINE, NKM_UP_NAME)

#define __neko__ NKM_CONCAT(NKM_UP_NAMESPACE_NAME_PREFIX, NKM_UP_NAME)::

NKM_NP
{
  inline void test();
  struct A
  {
    static int a;
  };
}

inline void __neko__ test()
{
  int a = 0;
  a++;
}

inline int __neko__ A::a = 0;

#endif /* NEKO_CORE_MACRO_UTILS_HPP */
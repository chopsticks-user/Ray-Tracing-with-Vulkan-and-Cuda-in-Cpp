#ifndef NEKO_CORE_BASE_OBJECTS_HPP
#define NEKO_CORE_BASE_OBJECTS_HPP

#include "CoreDefines.hpp"

namespace neko::base_object {

template <typename TAllocatedMember, TAllocatedMember tDefaultStateValue,
          typename TDependenceWrapper>
class OAMMoveOnlyObject {
public:
  OAMMoveOnlyObject() = default;
  OAMMoveOnlyObject(const OAMMoveOnlyObject &) = delete;
  OAMMoveOnlyObject &operator=(const OAMMoveOnlyObject &) = delete;

  const TAllocatedMember &operator*() const noexcept { return mValue; }

  operator const TAllocatedMember &() const noexcept { return mValue; }

protected:
  typedef OAMMoveOnlyObject Base;

  OAMMoveOnlyObject(OAMMoveOnlyObject &&rhs) noexcept
      : mDependents{std::move(rhs.mDependents)}, mValue{std::exchange(
                                                     rhs.mValue,
                                                     tDefaultStateValue)} {}

  OAMMoveOnlyObject &operator=(OAMMoveOnlyObject &&rhs) noexcept {
    release();
    mDependents = std::move(rhs.mDependents);
    mValue = std::exchange(rhs.mValue, tDefaultStateValue);
    return *this;
  }

  virtual void releaseImpl() noexcept {}

  virtual void release() noexcept {
    if (mValue != tDefaultStateValue) {
      releaseImpl();
      mValue = tDefaultStateValue;
    }
  }

  virtual ~OAMMoveOnlyObject() noexcept { release(); }

protected:
  TDependenceWrapper mDependents = {};
  TAllocatedMember mValue = tDefaultStateValue;
};

} // namespace neko::base_object

#define NEKO_INHERIT_MOVE_ONLY_SPECIAL_METHODS(derivedClass)                   \
  derivedClass() = default;                                                    \
  derivedClass(derivedClass &&rhs) noexcept : Base{std::move(rhs)} {};         \
  derivedClass &operator=(derivedClass &&rhs) noexcept {                       \
    release();                                                                 \
    Base::operator=(std::move(rhs));                                           \
    return *this;                                                              \
  };                                                                           \
  virtual ~derivedClass() noexcept override{};                                 \
  using Base::release

#endif /* NEKO_CORE_BASE_OBJECTS_HPP */
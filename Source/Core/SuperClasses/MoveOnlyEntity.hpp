#ifndef HDG_CORE_SUPERCLASSES_MOVEONLYENTITY_HPP
#define HDG_CORE_SUPERCLASSES_MOVEONLYENTITY_HPP

#include "Common/Common.hpp"

#include <vector>
#include <string>
#include <fstream>

namespace Neko::Core
{
    namespace Super
    {
        template <typename TUnallocatedMembers,
                  typename TAllocatedMember,
                  TAllocatedMember tUnallocatedState = TAllocatedMember{}>
        class MoveOnlyEntity
        {
        public:
            MoveOnlyEntity() = default;
            MoveOnlyEntity(const MoveOnlyEntity &) = delete;
            MoveOnlyEntity &operator=(const MoveOnlyEntity &) = delete;

            const TAllocatedMember &operator*() const noexcept { return mAllocated; }

            operator const TAllocatedMember &() const noexcept { return mAllocated; }

        protected:
            typedef MoveOnlyEntity Super;

            MoveOnlyEntity(MoveOnlyEntity &&rhs) noexcept
                : mDependents{std::move(rhs.mDependents)},
                  mAllocated{std::exchange(rhs.mAllocated, tUnallocatedState)} {}

            MoveOnlyEntity &operator=(MoveOnlyEntity &&rhs) noexcept
            {
                release();
                mDependents = std::move(rhs.mDependents);
                mAllocated = std::exchange(rhs.mAllocated, tUnallocatedState);
                return *this;
            }

            virtual void releaseImpl() noexcept {}

            virtual void release() noexcept
            {
                if (mAllocated != tUnallocatedState)
                {
                    releaseImpl();
                    mAllocated = tUnallocatedState;
                }
            }

            virtual ~MoveOnlyEntity() noexcept { release(); }

        protected:
            TUnallocatedMembers mDependents = {};
            TAllocatedMember mAllocated = tUnallocatedState;
        };

    } // namespace Neko::Core::Super

} // namespace Neko::Core

#define NEKO_BODY_MOVE_ONLY_ENTITY(derivedClass)                          \
    derivedClass() = default;                                             \
    derivedClass(derivedClass &&rhs) noexcept : Super{std::move(rhs)} {}; \
    derivedClass &operator=(derivedClass &&rhs) noexcept                  \
    {                                                                     \
        release();                                                        \
        Super::operator=(std::move(rhs));                                 \
        return *this;                                                     \
    };                                                                    \
    virtual ~derivedClass() noexcept override{};                          \
    using Super::release

#endif // HDG_CORE_SUPERCLASSES_MOVEONLYENTITY_HPP
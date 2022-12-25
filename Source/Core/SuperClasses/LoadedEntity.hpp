#ifndef HDG_CORE_SUPERCLASSES_LOADEDENTITY_HPP
#define HDG_CORE_SUPERCLASSES_LOADEDENTITY_HPP

#include "Common/Common.hpp"

#include <fstream>

namespace Neko::Core
{
    namespace Super
    {
        class ILoadedEntity
        {
        public:
            virtual ILoadedEntity *clone() = 0;

            bool isBindedToAPath() { return !mPath.empty(); }

            const std::string &getPath() const noexcept { return mPath; }

            virtual void load(const std::string &path) = 0;

            virtual void save(const std::string &path) = 0;

            virtual void changePath(const std::string &path = "")
            {
                mPath = path;
            }

            virtual ~ILoadedEntity() = default;

        protected:
            ILoadedEntity() = default;

            virtual void
            open(const std::string &path, std::ios_base::openmode openMode) const
            {
                std::fstream fs{path, openMode};
                if (!fs.is_open())
                {
                    throw std::runtime_error("Failed to open file " + path);
                }
            }

            std::string mPath = "";
        };

    } // namespace Neko::Core::Super

} // namespace Neko::Core

#endif // HDG_CORE_SUPERCLASSES_LOADEDENTITY_HPP
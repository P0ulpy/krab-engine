//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_UUID_HPP
#define PATHFINDER_UUID_HPP

#include <cstdint>

namespace KrabEngine::Core
{
    /**
     * @brief The UUID class is a 64 bits unique identifier.
     */
    class UUID
    {
    private:
        std::uint64_t m_UUID = 0;

        UUID();
    public:
        static UUID CreateNew();
        static const UUID Null;

        UUID(std::uint64_t uuid);
        UUID(const UUID&) = default;

        inline operator std::uint64_t() const { return m_UUID; }
    };
}

namespace std {
    template <typename T> struct hash;

    template<>
    struct hash<Engine::Core::UUID>
    {
        size_t operator()(const Engine::Core::UUID& uuid) const
        {
            return (std::uint64_t)uuid;
        }
    };
}

#endif //PATHFINDER_UUID_HPP

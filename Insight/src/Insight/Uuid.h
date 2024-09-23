#pragma once

#include <random>

#include "Insight.h"


namespace Insight
{
    static inline std::random_device s_RandomDevice;
    static inline std::mt19937 s_RNG(s_RandomDevice());

    class Uuid
    {
    public:
        Uuid() = default;
        Uuid(const Uuid& other) = default;
        ~Uuid() = default;

        bool operator==(const Uuid& other) const
        {
            return m_A == other.m_A && m_B == other.m_B && m_C == other.m_C && m_D == other.m_D;
        }

        bool operator!=(const Uuid& other) const
        {
            return m_A != other.m_A || m_B != other.m_B || m_C != other.m_C || m_D != other.m_D;
        }

        Uuid(const u32 a, const u32 b, const u32 c, const u32 d) : m_A(a), m_B(b), m_C(c), m_D(d)
        {
        }

        explicit Uuid(const string& uuid) : Uuid(uuid.c_str())
        {
        }

        explicit Uuid(const char* uuid)
        {
            u32 b = 0, c = 0, d = 0, e = 0;

            sscanf(uuid, "%08X-%04X-%04X-%04X-%04X%08X", &m_A, &b, &c, &d, &e, &m_D);

            m_B = (b << 16) | (c & 0xFFFF);
            m_C = (d << 16) | (e & 0xFFFF);
        }


        Uuid(const std::initializer_list<u32> list)
        {
            auto it = list.begin();
            m_A = *it++;
            m_B = *it++;
            m_C = *it++;
            m_D = *it;
        }

        static Uuid CreateNew()
        {
            return {s_RNG(), s_RNG(), s_RNG(), s_RNG()};
        }

        [[nodiscard]] string ToString() const
        {
            return fmt::format("{0:08X}-{1:04X}-{2:04X}-{3:04X}-{4:04X}{5:08X}",
                               m_A,
                               m_B >> 16,
                               m_B & 0xFFFF,
                               m_C >> 16,
                               m_C & 0xFFFF,
                               m_D
            );
        }

        static Uuid Null;

    private:
        u32 m_A = 0;
        u32 m_B = 0;
        u32 m_C = 0;
        u32 m_D = 0;

        friend struct std::hash<Uuid>;
    };

    inline Uuid Uuid::Null = Uuid(0, 0, 0, 0);
}

template <>
struct fmt::formatter<Insight::Uuid> : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.

    auto format(Insight::Uuid id, format_context& ctx) const
        -> format_context::iterator;
};

template <>
struct std::hash<Insight::Uuid>
{
    std::size_t operator()(Insight::Uuid const& s) const noexcept;
};

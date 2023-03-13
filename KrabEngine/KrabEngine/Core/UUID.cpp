//
// Created by Flo on 26/11/2022.
//

#include "UUID.hpp"

#include <random>

using namespace KrabEngine::Core;

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

const UUID UUID::Null = UUID(0);

UUID UUID::CreateNew() { return UUID(); }

UUID::UUID()
        : m_UUID(s_UniformDistribution(s_Engine))
{}

UUID::UUID(uint64_t uuid)
        : m_UUID(uuid)
{}

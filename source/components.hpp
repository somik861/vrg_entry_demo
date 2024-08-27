#pragma once

#include "custom_au_units.hpp"

namespace components {
struct Position {
    au::QuantityD<au::Meters> x, y, z;
};

struct Velocity {
    au::QuantityD<au::Direction> dx, dy, dz;
};

struct Collision {
    au::QuantityD<au::Meters> radius;
    std::vector<entt::entity> collisions;
};

// independent of objects velocity
struct StaticForce {
    au::QuantityD<au::Acceleration> dx, dy, dz;
};

} // namespace components
#include "custom_au_units.hpp"

namespace constants {
constexpr au::QuantityD<au::Acceleration> gravity = (au::meters / squared(au::second))(9.81);
}
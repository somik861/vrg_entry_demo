#pragma once

#include <au.hh>

namespace au {
using Direction = decltype(Meters{} / Seconds{});
using Acceleration = decltype(Meters{} / squared(Seconds{}));
} // namespace au
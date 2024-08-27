#include <iostream>

#include <au.hh>
#include <entt/entt.hpp>

#include "components.hpp"
#include "constants.hpp"

#include "svg_creator.hpp"
#include "systems.hpp"

entt::entity add_bullet(entt::registry& reg) {
    auto bullet = reg.create();

    auto mps = (au::meters / au::second);

    reg.emplace<components::Position>(bullet, au::meters(0.0), au::meters(0.0), au::meters(0.0));
    reg.emplace<components::Velocity>(bullet, mps(20.0), mps(22.5), mps(0.0));
    reg.emplace<components::Collision>(bullet, au::meters(0.01));

    return bullet;
}

void add_gravity(entt::registry& reg) {
    auto force = reg.create();

    auto mpss = (au::meters / au::squared(au::second));

    reg.emplace<components::StaticForce>(force, mpss(0.0), -constants::gravity, mpss(0.0));
}

entt::entity add_target(entt::registry& reg) {
    auto target = reg.create();

    reg.emplace<components::Position>(target, au::meters(10.0), au::meters(10.0), au::meters(0.0));
    reg.emplace<components::Collision>(target, au::meters(0.1));

    return target;
}

int main() {
    auto timestamp = au::seconds(1e-2);

    svg::SVGCreator svg;
    entt::registry registry;

    auto bullet = add_bullet(registry);
    add_gravity(registry);
    auto target = add_target(registry);

    systems::MovementSystem movement_sys(timestamp);
    systems::ForceSystem force_sys(timestamp);
    systems::CollisionSystem collision_sys;

    // simulation start
    // draw target
    {
        const auto& [position, colision] =
            registry.get<const components::Position, const components::Collision>(target);
        svg.addCircle(svg::Circle(
            colision.radius.in<au::Meters>(), {position.x.in<au::Meters>(), position.y.in<au::Meters>()}, "red"));
    }

    // loop while bullet is not on the ground (or iteration limit is reached)
    for (std::size_t i = 0; i < 1'000'000; ++i) {
        auto before = registry.get<components::Position>(bullet);

        movement_sys.update(registry);
        force_sys.update(registry);
        collision_sys.update(registry);

        auto after = registry.get<components::Position>(bullet);
        auto bullet_col = registry.get<components::Collision>(bullet);

        svg::Line line({before.x.in<au::Meters>(), before.y.in<au::Meters>()},
                       {after.x.in<au::Meters>(), after.y.in<au::Meters>()},
                       bullet_col.radius.in<au::Meters>(),
                       "black");

        svg.addLine(std::move(line));

        if (after.y < au::ZERO)
            break;

        if (!bullet_col.collisions.empty())
            break;
    }

    svg.saveToFile("trajectory.svg");
}
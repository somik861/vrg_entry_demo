#pragma once

#include "components.hpp"

#include <au.hh>
#include <entt/entt.hpp>

namespace systems {
class MovementSystem {
  public:
    MovementSystem(au::QuantityD<au::Seconds> timestep)
        : m_timestep(timestep) {}

    void update(entt::registry& reg) {
        auto view = reg.view<components::Position, const components::Velocity>();
        for (auto&& [entity, pos, vel] : view.each()) {
            pos.x += vel.dx * m_timestep;
            pos.y += vel.dy * m_timestep;
            pos.z += vel.dz * m_timestep;
        }
    }

  private:
    au::QuantityD<au::Seconds> m_timestep;
};

class ForceSystem {
  public:
    ForceSystem(au::QuantityD<au::Seconds> timestep)
        : m_timestep(timestep) {}

    void update(entt::registry& reg) { 
        processStaticForces(reg);
    }

  private:
    void processStaticForces(entt::registry& reg) {
        auto objects = reg.view<components::Velocity>();
        auto forces = reg.view<components::StaticForce>();

        for (auto&& [obj_entt, vel] : objects.each())
            for (auto&& [force_entt, force] : forces.each()) {
                // skip if object and forces are the same entities
                if (obj_entt == force_entt)
                    continue;
                vel.dx += force.dx * m_timestep;
                vel.dy += force.dy * m_timestep;
                vel.dz += force.dz * m_timestep;
            }
    }


    au::QuantityD<au::Seconds> m_timestep;
};

class CollisionSystem {
  public:
    void update(entt::registry& reg) { 
        auto objects = reg.view<components::Position, components::Collision>();
        for (auto&& [first, pos_first, col_first] : objects.each()) {
            // remove previous collisions
            col_first.collisions.clear();

            for (auto&& [snd, pos_snd, col_snd] : objects.each()) {
                if (first == snd) // cant collide with itself
                    continue;
                
                auto dx = pos_first.x - pos_snd.x;
                auto dy = pos_first.y - pos_snd.y;
                auto dz = pos_first.z - pos_snd.z;

                auto distance = au::sqrt(dx * dx + dy * dy + dz * dz);
                if (distance <= (col_first.radius + col_snd.radius))
                    col_first.collisions.push_back(snd); // collided
            }
        }
    }
};

} // namespace systems
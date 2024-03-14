#ifndef UI_SYSTEM_HPP
#define UI_SYSTEM_HPP

#include <entt/entt.hpp>

class UISystem {
public:
    void update(entt::registry& scene);

private:
    void updateScrollView(entt::registry& scene);
};

#endif //UI_SYSTEM_HPP
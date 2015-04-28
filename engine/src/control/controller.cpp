#include "control/controller.hpp"

bool controller::register_character(character &c) {
    if (behaviour::register_character(c)) {
        move_north_ = move_action(c, direction::north);
        move_south_ = move_action(c, direction::south);
        move_east_ = move_action(c, direction::east);
        move_west_ = move_action(c, direction::west);
        move_northwest_ = move_action(c, direction::northwest);
        move_northeast_ = move_action(c, direction::northeast);
        move_southwest_ = move_action(c, direction::southwest);
        move_southeast_ = move_action(c, direction::southeast);

        return true;
    }

    return false;
}

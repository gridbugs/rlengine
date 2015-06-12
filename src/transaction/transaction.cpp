#include "transaction/transaction.hpp"
#include "world/world.hpp"

bool try_move_transaction::attempt(world &w) {
    grid<world_cell> &map = w.maps[character_.level_index];
    world_cell &c = map.get_cell(character_.coord);
    return map.with_neighbour<bool, false>(c, direction_, [&](const world_cell &dest) {
        if (!dest.is_solid()) {
            w.register_transaction<move_transaction>(character_, c, dest);
            return true;
        }
        return false;
    });
}

void try_move_transaction::fail(world &w) {
    w.register_transaction<move_blocked_transaction>(character_);
}

bool try_attack_transaction::attempt(world &w) {
    grid<world_cell> &map = w.maps[attacker_.level_index];
    return map.with_neighbour<bool, false>(map.get_cell(attacker_.coord), direction_, [&](const world_cell &target_cell) {
        bool ret = false;
        w.with_character_at_coord(attacker_.level_index, target_cell.coord, [&](character &target) {
            w.register_transaction<attack_transaction>(attacker_, target);
            ret = true;
        });
        return ret;
    });
}
void try_attack_transaction::fail(world &w) {
     w.register_transaction<miss_attack_transaction>(attacker_);
}

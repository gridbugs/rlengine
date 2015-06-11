#include "transaction/transaction.hpp"
#include "world/world.hpp"

bool try_move_transaction::attempt(world &w) {
    grid<world_cell> &map = w.maps[character_.level_index];
    return map.template with_neighbour<bool, false>(map.get_cell(character_.coord), direction_, [&](const world_cell &dest) {
        if (!dest.is_solid()) {
            w.transactions.register_transaction(
                std::make_unique<move_transaction>(
                    character_,
                    map.get_cell(character_.coord),
                    dest
                )
            );
            return true;
        }
        return false;
    });
}

void try_move_transaction::fail(world &w) {
    w.transactions.register_transaction(
        std::make_unique<move_blocked_transaction>(character_)
    );
}

bool try_attack_transaction::attempt(world &w) {
    grid<world_cell> &map = w.maps[attacker_.level_index];
    return map.template with_neighbour<bool, false>(map.get_cell(attacker_.coord), direction_, [&](const world_cell &target_cell) {
        w.with_character_at_coord(attacker_.level_index, target_cell.coord, [&](character &target) {
            w.transactions.register_transaction(
                std::make_unique<attack_transaction>(
                    attacker_,
                    target
                )
            );
            return true;
        });
        return false;
    });
}
void try_attack_transaction::fail(world &w) {
     w.transactions.register_transaction(
        std::make_unique<miss_attack_transaction>(attacker_)
    );
}

#include "knowledge/knowledge.hpp"
#include "character/character.hpp"

void knowledge_cell::see_character(const character &c) {
    characters.push_back(c.get_image());
}

#include "weapons.h"

// Base Weapons constructor
Weapons::Weapons(const std::string& name, char symbol, const std::string& action, int dexterityBonus, int damageAmount)
    : GameObjects(name, symbol), action(action), dexterityBonus(dexterityBonus), damageAmount(damageAmount) {}

std::string Weapons::GetAction() const {
    return action;
}

int Weapons::GetDexterityBonus() const {
    return dexterityBonus;
}

int Weapons::GetDamageAmount() const {
    return damageAmount;
}

// Specific weapon implementations
Mace::Mace() : Weapons("Mace", '(', "swings mace at ", 0, 2) {}

ShortSword::ShortSword() : Weapons("Short Sword", '(', "slashes short sword at ", 0, 2) {}

LongSword::LongSword() : Weapons("Long Sword", '(', "swings long sword at ", 2, 4) {}

MagicAxe::MagicAxe() : Weapons("Magic Axe", '(', "chops magic axe at ", 5, 5) {}

MagicFangsOfSleep::MagicFangsOfSleep() : Weapons("Magic Fangs of Sleep", '(', "strikes magic fangs at ", 3, 2) {}

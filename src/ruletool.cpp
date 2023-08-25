#include "ruletool.hpp"


const std::array<std::array<bool, 4>, 16> MargolusRuletool::generateTransforms(knownRuleset name) {
    switch (name) {
        case _BINARY:
            return MargolusRuletool::sequential_binary;
        case ROTATIONS_3:
            return MargolusRuletool::rotations_3_ruleset;
        case BILLIARD_BALL:
            return MargolusRuletool::billiard_ball_ruleset;
        case CRITTERS:
            return MargolusRuletool::critters_ruleset;
        case TRON:
            return MargolusRuletool::tron_ruleset;
        default:
            throw std::runtime_error("Unknown ruleset cannot be generated.");
    }
}
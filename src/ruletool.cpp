#include "ruletool.hpp"


const std::array<std::array<bool, 4>, 16> MargolusRuletool::generateTransforms(knownRuleset name) {
    switch (name) {
        case _BINARY:
            return MargolusRuletool::SEQUENTIAL_BINARY;
        case ROTATIONS_3:
            return MargolusRuletool::ROTATIONS_3_RULESET;
        case BILLIARD_BALL:
            return MargolusRuletool::BILLIARD_BALL_RULESET;
        case CRITTERS:
            return MargolusRuletool::CRITTERS_RULESET;
        case TRON:
            return MargolusRuletool::TRON_RULESET;
        default:
            throw std::runtime_error("Unknown ruleset cannot be generated.");
    }
}
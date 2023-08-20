#include "marg_ruletool.hpp"


const std::array<std::array<bool, 4>, 16> MargolusRuletool::generateTransforms(knownRuleset name) {
    switch (name) {
        case BILLIARD_BALL:
            return MargolusRuletool::billiard_ball_ruleset;
        case CRITTERS:
            return MargolusRuletool::critters_ruleset;
        default:
            throw std::runtime_error("Unknown ruleset cannot be generated.");
    }
}
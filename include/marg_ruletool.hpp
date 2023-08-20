#ifndef MARGOLUS_RULETOOL_HEADER_
#define MARGOLUS_RULETOOL_HEADER_

#include <array>
#include <stdexcept>


class MargolusRuletool {
private:
    // Here are the conversions for the rulesets:
    // 1 2   Each block is separated in the following
    // 3 4   four bools, in this order.
    // Then the ruleset determines, in rising binary
    // order, how to convert from one state to the
    // next.
    // E.g.: {0, 0, 0, 0} -> ruleset[0]
    //       {0, 0, 0, 1} -> ruleset[1]
    //       {0, 0, 1, 0} -> ruleset[2]
    //       ...

    static constexpr std::array<std::array<bool, 4>, 16> critters_ruleset = {{
        {1, 1, 1, 1},
        {1, 1, 1, 0},
        {1, 1, 0, 1},
        {0, 0, 1, 1},

        {1, 0, 1, 1},
        {0, 1, 0, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 1},

        {0, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 1, 0},
        {0, 0, 1, 0},

        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0}
    }};

    static constexpr std::array<std::array<bool, 4>, 16> billiard_ball_ruleset = {{
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 1},
        {0, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 0, 1},
        {0, 1, 1, 1},
        {0, 0, 0, 1},
        {0, 1, 1, 0},
        {1, 0, 1, 0},
        {1, 0, 1, 1},
        {1, 1, 0, 0},
        {1, 1, 0, 1},
        {1, 1, 1, 0},
        {1, 1, 1, 1}
    }};

public:
    enum knownRuleset { BILLIARD_BALL, CRITTERS };
    static const std::array<std::array<bool, 4>, 16> generateTransforms(knownRuleset name);
};

#endif
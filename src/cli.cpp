#include "cli.hpp"


MargolusCLI::MargolusCLI()
    : options("Available command line options"),
      renderer(DEFAULT_RENDERER),
      newRuleset(DEFAULT_NEW_RULESET),
      newWidth(DEFAULT_NEW_WIDTH),
      newHeight(DEFAULT_NEW_HEIGHT),
      showHelp(false),
      runAnimated(false),
      createNew(false),
      inputToStdin(false),
      outputToStdout(false),
      oddFlipsColor(false),
      iter(0) {

    options.add<popl::Switch>("h", "help", "Display help message.", &showHelp);

    options.add<popl::Switch>("n", "new", "Create an empty grid.", &createNew);
    options.add<popl::Value<size_t>>("W", "width", "horizontal size of the new grid", newWidth, &newWidth);
    options.add<popl::Value<size_t>>("H", "height", "vertical size of the new grid", newHeight, &newHeight);
    options.add<popl::Value<std::string>>("R", "ruleset", "ruleset to use for the new grid", newRuleset, &newRuleset);

    options.add<popl::Value<std::string>>("l", "load", "Path to the file to load.", loadFile, &loadFile);
    options.add<popl::Switch>("L", "load-stdin", "Load input file from STDIN.", &inputToStdin);
    options.add<popl::Value<std::string>>("s", "save", "Path to the file to save, once finished.", saveFile, &saveFile);
    options.add<popl::Switch>("S", "save-stdout", "Show output file on STDOUT.", &outputToStdout);

    options.add<popl::Value<long>>("i", "iters", "Iterations to run.", iter, &iter);
    options.add<popl::Value<std::string>>("r", "render", "Engine to use to render grid.", renderer, &renderer);
    options.add<popl::Switch>("o", "odd-flip", "Flip render colors on odd iterations.", &oddFlipsColor);
    options.add<popl::Switch>("a", "animated", "Run with a 75ms step animation.", &runAnimated);
}

void MargolusCLI::checkParse(int argc, char **argv) {
    options.parse(argc, argv);

    if (showHelp) {
        std::cout << options << std::endl;

        std::cout << "Available option completions:" << std::endl;
        std::cout << "  iters: \t\tSigned long integer, +forward -backward." << std::endl;
        std::cout << "  ruleset: \t\t\"none\", \"ROTATIONS_3\", \"BILLIARD_BALL\", \"CRITTERS\", \"TRON\"." << std::endl;
        std::cout << "  render: \t\t\"q\", \"ANSI\"." << std::endl;
        std::cout << std::endl;
    }

    if (loadFile.empty() and !inputToStdin)
        throw std::invalid_argument("No input filename provided, use -L for STDIN.");

    if (!loadFile.empty() and inputToStdin)
        throw std::invalid_argument("-l and -L are mutually exclusive.");

    if (!saveFile.empty() and outputToStdout)
        throw std::invalid_argument("-s and -S are mutually exclusive.");

    if (outputToStdout and runAnimated)
        throw std::invalid_argument("Not running animated with output to STDOUT.");

    if (renderer != "q" and renderer != "ANSI")
        throw std::invalid_argument("No such known renderer.");
}
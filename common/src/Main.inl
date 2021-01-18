static_assert(std::is_same<decltype(AoC::main(std::declval<AoC::ArgumentParser>())), void>::value,
              "AoC::main should return void.");

int main(int argc, char **argv) {
  try {
    AoC::ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    AoC::main(argumentParser);
  } catch (const std::exception &exception) {
    std::cout << "Threw an exception:\n\t" << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

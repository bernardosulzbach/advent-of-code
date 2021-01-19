int main(int, char **) {
  try {
    AoC::RunStatistics runStatistics;
    for (auto const runSpecification : AoC::generateRunSpecifications()) {
      runSpecification.run(runStatistics);
    }
    std::cout << runStatistics << '\n';
  } catch (const std::exception &exception) {
    std::cout << "Threw an exception:\n\t" << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
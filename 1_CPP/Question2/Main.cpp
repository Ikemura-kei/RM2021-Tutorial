#include <iostream>
#include "QuestionTwo.hpp"

const std::string inputFile = "Data.txt";
const std::string outputFile = "SwappedData.txt";

int main() {
  int one, two;
  std::cout << "Choose column one to swap: ";
  std::cin >> one;
  std::cout << "Choose column two to swap: ";
  std::cin >> two;

  swapCols(inputFile, outputFile, one, two);
}

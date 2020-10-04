#include <iostream>
#include "QuestionOne.hpp"

constexpr int numSamples = 6;

constexpr char inputs[][16] = {"aabb",        "a",         "aBBbabccac",
                               "ABCDEFGHIJj", "123123321", "abbblollll"};
constexpr char outputs[] = {'a', 'a', 'B', 'A', '3', 'l'};

int main() {
  std::cout << "\n";
  int numFailed = 0;

  for (int i = 0; i < numSamples; i++) {
    char out = getMaxConsecutiveChar(inputs[i]);

    if (out != outputs[i]) {
      std::cout << "Failed test case: " << i + 1
                << "\n| Test input: " << inputs[i]
                << " | expected output: " << outputs[i]
                << " | your output: " << out << " |\n\n";

      numFailed++;
    }
  }

  if (!numFailed)
    std::cout << "Congrats, you passed all test cases.\n\n";
  else
    std::cout << "You failed " << numFailed << " out of " << numSamples
              << " tests.\n\n";
}

#include <fstream>
#include <iostream>
#include <random>

int main() {
  std::ofstream file("./Data.txt");

  if (!file.is_open()) {
    std::cout << "Could not open file, exit\n";
    return 0;
  }

  for (int i = 0; i < 1000; i++) {
    static float values[3];
    static std::string values_str[3];

    for (int j = 0; j < 3; j++) {
      int dividend = rand() % 2000;
      int divisor = rand() % 100 + 1;
      values[j] = dividend / (float)divisor;
      values_str[j] = std::to_string(values[j]);
    }

    std::string line =
        values_str[0] + " " + values_str[1] + " " + values_str[2] + "\n";

    file << line;
  }

  file.close();
}

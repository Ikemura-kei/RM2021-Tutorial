#include "QuestionTwo.hpp"
#include <fstream>

// Implement this function so it can perform column swapping.
bool swapCols(const std::string &srcFile, const std::string &dstFile,
              int swapedCol1, int swapedCol2) {
  std::ifstream src(srcFile);
  std::ofstream dst(dstFile);

  if (!src.is_open() || !dst.is_open()) return false;

  /**********Your code starts here, DO NOT modify anything above**********/
  // Note: You can format code, though, that does not count as modifying.

  /**********Your code ends here, DO NOT modify anything below**********/

  src.close();
  dst.close();

  return true;
}

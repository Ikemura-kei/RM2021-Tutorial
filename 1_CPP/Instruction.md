# Instruction for C++ tutorial module assignment

* For this assignment, you need to choose __ONE__ programming question from the questions listed in [Questions](#questions). 
* To submit your work, please refer to [Submission](#submission). 
* For grading scheme, please refer to [Grading](#grading).

## General

* If you have the capability, we recommend you to choose the higher difficulty question instead of the lower ones. (If you have programming background, all questions should be fairly simple to you).
* For Question 1 and 2, we provide skeleton codes for you to start with. For Question 3, please do the whole thing yourself.

* For Question 1, we provide a test program that you can run to test your code implementation. For question 2 and 3, we provide you some sample inputs and corresponding outputs, if you want to do tests on your program, please read the sample files into your program by yourself and test.

* You could __TRY__ (not compulsury) to write a Makefile to build your codes (no matter which question you choose). Having a usable Makefile with usable `all` and `clean` targets will give you __BONUS points__.

## Questions

> Difficulty Levels: 1, 2, and 3 in the increading order of difficulty.

### Q1: Find Maximum Consecutive Character
* Difficulty Level: 1
* Input to your program: A null-terminated c-string.
* Output of your program: A character.

You need to find the __FIRST__ character that appeared the most, __CONSECUTIVELY__. 

For example, if I input the string `bbabbaaab`, your output should be `a`.

* Your program should be __CASE SENSITIVE__.  
* Inputs __WILL NOT__ be nullptr or empty c-string, you can ignore them.
* White spaces (Tab, Space) __WILL NOT APPEAR__ in inputs.
* Symbols __OTHER THAN__ __{Numbers(0-9), Letters(a-z,A-Z)}__ __WILL NOT__ appear in inputs.
* Please assume the input size will be __NO MORE THAN__ 16 characters.
* __YOU ARE NOT ALLOWED TO USE THE string class__, i.e `std::string`.


#### More Examples:
```
INPUT: aabb
OUTPUT: a

INPUT: a
OUTPUT: a

INPUT: AaaABBB
OUTPUT: B

INPUT: 12233abc
OUTPUT: 2
```

#### Your job:

* In the directory `./Qeustion1` we provide you the skeleton codes. You can see in `QeustionOne.cpp`, there is an unimplemented function called `char getMaxConsecutiveChar(const char inputs[])`, please implement this function, it should return the `Maximum Consecutive Character` apeared in the inputs.

* __DO NOT__ modify any other skeleton files, you should __ONLY__ write codes in `QeustionOne.cpp`

* You can test your implementation by running the test program by typing: `g++ TestProgram.cpp QuestionOne.cpp -o Test` (To compile) and `./Test` (To run) in your terminal.

* The test program will notify you if you have failed a test.

### Q2: Column Swapper
* Difficulty Level: 2
* Input to your program: A file with three columns of data + Two numbers indicating columns to swap.
* Output of your program: A new file called `SwapedData.txt` that completes the column swapping.
We will provide you a text file, `Data.txt` that contains the following:  
```
...
1.23 0.13 1.22
7.0 1.45 10.1
9.8 22.3 0.91
...
...
11.23 34 18
19.18 3.2 5.19
...
```

Some rows are replaced by `...` here. Basically there are 3 columns of data, each pair of neighbouring columns is separated by a __SPACE__, there are totally __1000 rows__. 

Your task is to design a function that can swap any two columns specified by users.

#### Your job

* In the directory `./Qeustion2`, we provide some skeleton codes for you to start with. In `QuestionTwo.cpp`, you will see an unimplemented function called `bool swapCols(std::string srcFile, std::string dstFile, int swapedCol1, int swapedCol2)`, implement this so that it performs the column swapping.


* In the `Debug` directory, we provide you an example file `TestData.txt`, and the corresponding answers of swapping different columns in `Col_x1_x2.txt` where `x1` and `x2` represents the indeces of the columns been swapped. Feel free to use them to debug your code.

#### Notes

* You are __NOT ALLOWED__ to modify skeleton files other than `QeustionTwo.cpp`.
* There are exactly three columns in `Data.txt`, separated by spaces.
* Be careful to __NOT TO__ have empty lines at the end of your output.
* Columns are indexed starting with 1. (Column 1, 2, and 3)
* User may input two same numbers.
* Please assume user __WILL NOT__ input any invalid inputs (e.g. number bigger than 3).
* You are __ALLOWED__ to use `std::string` class.
* You can add helper functions, only in `QuestionTwo.cpp`.

### Q3: How Long Have I Lived?
* Difficulty Level: 3
* Input to your program: Two strings of numbers, indicating birthday and current day respectively.
* Output of your program: Number of days that this person have lived.

You need to calculated how many days have this person lived. Note that you need to take leap year into account, by following the rule below:
```c++
if ((YYYY % 4 == 0 && YYYY % 100 != 0) || YYYY % 400 == 0)
    isLeapYear = true;
```

* Both the day of birth and current day __COUNT__ as days this person have lived (unless these two are the same day, in that case only count as __ONE__ day).
* The format of input is in YYYYMMDD, for example 20201001 (2020, Oct. 1st).
* You can assume that the person is already born, i.e day of birth is earlier than current day.
* 1000 <= YYYY <= 9999, i.e. year will always start with a 9 >= digit >= 0.
* 01 <= MM <= 12 and 01 <= DD <= 31
* Your output to the terminal needs to be exactly the same as the examples below

#### Examples:
```
Input birthday (YYYYMMDD):
20190920
Input current day (YYYYMMDD):
20200920
The number of days this person have lived:
367

Input birthday (YYYYMMDD):
20010427
Input current day (YYYYMMDD):
20010427
The number of days this person have lived:
1

Input birthday (YYYYMMDD):
20010427
Input current day (YYYYMMDD):
20010428
The number of days this person have lived:
2
```

We also provide sample inputs and outputs for this one, refer to `./question3/ExampleInputsOutputs.txt`. Consider using it to automate your debuf process. 

> Note: The first column is birthday, the second column is current day, and the last one is the output number of days. Columns are separated by semi-colons.

#### Your job
Feel free to design whatever program you like, the only constraints are the following:
* Completes the intended feature, the way users input depends on your implementation, it can be through command line or arguments passed to executable or anything else you can think of.
* Written in C++ (of course).
* Your codes are readable.

## Submission

For this assignment, together with the assignment of "Git and Github" module, you need to create your own __PRIVATE__ Github repository and submit your work there, the detailed steps are as follows:
1. Follow [This tutorial](https://kbroman.org/github_tutorial/pages/init.html) to create your repository, note that it should be __PRIVATE__ instead of public.
2. Create a directory/folder with name `CPP` to store your work of this assignment.
3. Push your work to the remote.
4. Set our official account,(Username is `HKUST-Enterprize-RoboMasterTeam`) as collaborator by following [This tutorial](https://help.github.jp/enterprise/2.11/user/articles/inviting-collaborators-to-a-personal-repository/).
5. You can feel free to push as many times as you want when you make changes.

## Grading

1. You can submit your work on Github: 15%
2. Your code compiles: 15%
3. Your code passed k tests out of N test cases: k/N * 50%
4. The dificulty level of your question, given the fact that it compiles: (difficulty_level - 1) * 15%
5. Follow the coding style and convention specified in [Coding convention](#coding-convention): 20%
6. __[BONUS]__ Has a Makefile that can build your code, and has `all` and `clean`(can clean up object files) target in it: 10%

> Note: Your score can be above 100%, Max = 15 + 15 + 50 + (3 - 1) * 15 + 20 + 10 = 140%

### Coding convention:
* lowerCamelCase for variables(Non-const), arguments, and function names.
* UpperCamelCase for Class, Enumeration(Type name), Struct, file names, namespace.
* UPPER_SNAKE_CASE for constants.
* __Do not use__ lower_snake_case.
* Set reasonable and intuitive names.
* Avoid magic numbers, use constants instead.
* __Do not use__ `#define` for constants, use `constexpr` instead.

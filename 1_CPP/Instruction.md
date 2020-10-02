# Instruction for C++ tutorial module assignment

* For this assignment, you need to choose __ONE__ programming task from the tasks listed [Tasks](#tasks). 
* To submit your work, please refer to [Submission](#submission). 
* For grading scheme, please refer to [Grading](#grading).

> Note: If you have the capability, we recommend you to choose the higher difficulty task instead of the lower ones. (If you have programming background, all tasks should be fairly simple to you)

> Note: For each task, we provided example input and output for you to automate your debug process, feel free to use them. (Hint: use file IO to read the corresponding files and store input and output in different variables. Use the inputs as the input to your program and compare your output with the example outputs inside your program and notify you if passed or not).

Suppose your file name is `MyWork.cpp` and you want your executable be named as `MyExecutable.exe`, then to build your code, use the command: `g++ MyWork.cpp -o MyExecutable`. And you can run your program by typing `./MyExecutable` in your terminal.

## Tasks

> Difficulty Levels: 1, 2, and 3 in the increading order of difficulty

### Q1: Find Maximum Consecutive Character
* Difficulty Level: 2
* Input to your program: A string.
* Output of your program: A character.

After user inputting (through `std::cin`) a string, you need to find the __FIRST__ character that appeared the most __CONSECUTIVELY__. 

* Your program should be __CASE SENSITIVE__.  
* White spaces are __ALLOWED__ in inputs.
* Symbols __OTHER THAN__ __{Numbers(0-9), Letters(a-z,A-Z), White Spaces(Tab, Space)}__ __WILL NOT__ appear in inputs.
* Please assume the input size will be __NO MORE THAN__ 16 (White space included).

For example, if I input the string `ababaabbaaa`, your output should be `a`.

#### More Examples:
```c++
INPUT: aabb
OUTPUT: a

INPUT: a
OUTPUT: a

INPUT:  // A space
OUTPUT:  // A space

INPUT: AaaABBB
OUTPUT: B

INPUT: ab bbb aaaa
OUTPUT: a

INPUT: a   b
OUTPUT:  // A space
```

In the directory `./Task1`, we provided example inputs and outputs for you to debug, the file name is `SampleInputAndOutput.txt`, you can read this file in your program. Please feel free to use depending on your needs.

> Note: The first column is inputs and the second column is the corresponding outputs, columns are separated by semi-colon.

### Q2: Column Swapper
* Difficulty Level: 1
* Input to your program: Two numbers indicating columns to swap.
* Output of your program: A new file called `SwapedData.txt` that completes the column swapping.

We will provide you a text file, `Data.txt` that contains the following:  
```
1.23 0.13 1.22
7.0 1.45 10.1
9.8 22.3 0.91
...
...
11.23 34 18
19.18 3.2 5.19
```
Things in the middle are replaced by `...` here. Basically there are three columns of data, each column is separated by a __SPACE__, there are __1000 rows__. 

Your task is to design a program that can swap any two columns specified by users.

* Columns are indexed starting with 1. (Column 1, 2, and 3)
* User input should be two numbers separated by a space (e.g. 2 3)
* User can input two same numbers.
* Request another input if user input is invalid (< 1 or > 3 or any other weired symbol or character).
* The lines printed to the terminal __DON'T HAVE TO BE THE SAME__ as the example below, we only care about the final output file.

#### Examples:
```
Please input the columns to swap, separate your numbers with space (e.g. 1 2):
1 3
Swapping columns 1 and 3...
Done, SwapedData.txt generated.

Please input the columns to swap, separate your numbers with space (e.g. 1 2):
Hi there!
Invalid input, please input again!
Please input the columns to swap, separate your numbers with space (e.g. 1 2):
```

Similarily, we also provided example inputs and outputs in `./Task2`, file `ExampleInput.txt` and `ExampleOutput.txt`. Feel free to use them to automate your debug process.

> Note: In the example provided, __column1__ and __column3__ are swapped.

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
* 1000 <= YYYY <= 9999, i.e. year will always start with a 9 > digit > 0.
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

We also provide sample inputs and outputs for this one, refer to `./Task3/ExampleInputsOutputs.txt`. Consider using it to automate your debuf process. 

> Note: The first column is birthday, the second column is current day, and the last one is the output number of days. Columns are separated by semi-colons.

## Submission

For this assignment, together with the assignment of "Git and Github" module, you need to create your own __PRIVATE__ Github repository and submit your work there, the detailed steps are as follows:
1. Follow [This tutorial](https://kbroman.org/github_tutorial/pages/init.html) to create your repository, note that it should be __PRIVATE__ instead of public.
2. Create a directory/folder with name `CPP` to store your work of this assignment.
3. Push your work to the remote.
4. Set [] as collaborator by following [This tutorial](https://help.github.jp/enterprise/2.11/user/articles/inviting-collaborators-to-a-personal-repository/).
5. You can feel free to push as many times as you want when you make changes.

## Grading

1. You can submit your work on Github: 15%
2. Your code compiles: 35%
3. Your code passed k tests out of N test cases: k/N * 50%
4. The dificulty level of your task, given the fact that it compiles: (difficulty_level - 1) * 15%
5. Has a Makefile that can build your code, and has `all` and `clean` target in it: 10%

> Note: Your score can be above 100%, Max = 15 + 35 + 50 + (3 - 1) * 15 + 10 = 140%

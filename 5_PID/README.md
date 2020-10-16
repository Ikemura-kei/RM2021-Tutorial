# PID Module

In this assignment, you will use what you have learned in PID tutorials and write an actual PID module inside the skeleton simulator we have provided. Note that this assignment will be conducted with __Python__. The reason is Python provides better and easier plot library and it is generally a good language for data visualization. __Note that python skills are almost not required, we only need you to write few equations, and that is basically same as what you would do in C++__. 

> You don't have to study python for this assignment since you are only going to implement a few equations and arithmetic calculations.

* For instruction on the assignment, refer to [Instruction](#instruction)
* For submission, refer to [Assignment Submission](#assignment-submission)
* For grading, refer to [Grading](#grading)

## Instruction
1. Clone the [PID assignment repository](https://github.com/LIANG-SN/PID-Control-Model) to your local.
2. Follow the instructions on the [PID assignment repository](https://github.com/LIANG-SN/PID-Control-Model) to set up the environment (dependencies).
3. Inside that repository, you will see several python scripts. Your goal is: __Modify `PID.py` (ONLY) and impelement INTEGRAL term and DIFFERENTIAL term of a PID controller.
4. More specifically, you only need to modify the function `update(self, setPoint, actualValue, lastActualValue)`, ignore the `self` parameter if you don't know what it means (is analogous to `this` in C++). Note that __PROPORTIONAL term__ is already implemented.
5. After you finished implementing the function, run `python ./plot.py` OR `python3 ./plot.py` to run the simulator, you can play with it to see how your implementation performs.
6. Ask us if you have any confusion that want us to clear out for you.

## Assignment Submission

* As usual, push your code to your private repo. (First create a directory named `5_PID`)
* Please make sure your code can run without any error. (you can have bugs/incorrect implementation, but at least your code should be runnable)

## Grading

* Has implemented __INTEGRATION term and DIFFERENTIAL__ term and your code is runnable. (50%)
* Your implementation is correct. (50%)

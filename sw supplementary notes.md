# Software Supplementary Notes (SAN checker)

> WARNING: If you have difficulty understanding the C++ tutorial, please write some more C/C++ programs until you are comfortable with it, **before** reading this guide. Otherwise you may fail the san check...
>
> The main purpose of this note is to scare small children. This is now incomplete, so it can only scare some but not all.

We assume you have some basic understanding for C and C++, if not please refer to online tutorials. This guide would explain from very low level about the things you should at least have an idea about for embedded programming. It is OK if you don't completely understand the materials at the moment, if you are interested you can ask in the group or PM seniors.

There is often a TLDR section after a long section, which summarize what you should know or do, feel free to skip the explanation and just read the TLDR part. This guide includes some *asides*, which are not very useful but good to know. Feel free to ignore the asides and only read them before sleep, so you would fall asleep quicker. We would also provide a list of keywords for you to Google if you cannot understand the content, which is normal as the author is major in CS but not education nor English literature :).

## History

- 5/10/2020: init
- 9/10/2020: added volatile section
- 12/10/2020: added memory section

## Building

> Keywords: ELF symbols, compiler, linker, name mangling, weak symbol.

### Name Mangling

For C/C++, our source code includes source file and header files, where the header files are included by other header files and source files. Header files should contain the declaration of variables (For example, `extern int a;`), functions (`int foo(int);`), macros, and classes. The compiler would compile each source files *independently* into object files, and they are linked by the linker into an executable or library file.

Note that C and C++ headers are different, C header files often wrap the content with something like this:

```c++
#ifdef __cplusplus
extern "C" {
#endif
// the actual content
#ifdef __cplusplus
}
#endif
```

This is because in C++ we have *name mangling*. In C/C++, we can use the functions or variables that are declared but not defined in that source file, the linker has to help us to find the function/variable definition for each of our usage. Functions and variable names would be exposed as symbol names in the compiled object files for the linker to find them. C symbol names are retained as is, with no modification. However, in C++ we have namespace and things like overloading, so we cannot use the same name for symbols or we would have name collision (linker error: multiple definition of ...). The compiler would *mangle* the symbol name with some rules, that include the namespace, class name, type signature information of the variable/function, to avoid name clashes. The mangling rule is consistent for one platform, so the same declaration in different source files would match to the same symbol (otherwise header files won't work, as including a header file is just replacing the include statement with the content of the header file). However, when we want to link C and C++ code together, we have to tell C++ compiler not to mangle the symbol, or the linker won't be able to find the symbol (either when we want to expose C functions to C++, or C++ function to C).

TLDR: Use the above pattern in C header files. Mark a C++ function as `extern "C"` if you want to expose it to C.

### Common Linker Error

When you work with C/C++, you would often encounter linker error saying missing definition or multiple definitions. For missing definition, most of the time it is due to forgetting to link or compile a certain source file. The C/C++ compiler would not check if the declaration actually have the corresponding definition, as each source file is compiled independently (*compilation unit*). The checking is done when you link the source files, i.e. combine the object files into an executable. Hence, missing source file would result in a link time error. For multiple definitions, that is often due to placing *definition* in header files (such as `int a = 1;`), and the header file is included in multiple source files. In that case, each object files compiled from the source files that included that header file, would generate the symbol corresponding to the definition, and the linker would not know which to include (even if they are actually the same), thus there would be an error.

TLDR: If you get multiple definition error, check your header files first. If you get missing definition error, check your build script or source files.

### Asides

> We should use quote block like this for asides, but unfortunately this section is a bit long...

Can we provide default implementation for a function, but allows the user to override it? Yes, this is done by *weak symbol*. We can declare a function as weak by function attribute. Users can declare the same function normally. If there is only the weak symbol, the linker would use the weak one, otherwise it would use the user provided one. This is usually done by the libraries rather than us.

As the embedded environment is different from our desktop environment, we use a *cross compiler* to build the firmware. A cross compiler is a compiler that compiles for another system, for example we compile to ARM target in our x86_64 computer. Examples of cross compilers that we use include the ARM GCC embedded toolchain, LLVM Clang, etc. When we say compiler, often we are not talking about the compiler only, but talk about the entire compiler toolchain. Compiler toolchain includes the underlying libraries, compiler and also other tools such as objdump for reading the assembly code of the compiled binary.

Also, we often have to specify the memory layout and what sections to keep in the resulting binary. This is done by specifying the linker script. This is often done by the others, such as seniors. Google it or ask seniors if you are interested, explaining it here would scare small children.

## Types

> Keywords: memory, pointer, arrays, numerical stability, floating-point processing unit, memory alignment, padding, memory-mapped IO

### Memory

> This section is not useful on its own, but to provide some background knowledge and terminology for later sections.

Data is represented in bits in the computer. 8 bits is called a byte.

Consider the main memory as a giant tape (with starting position), with different data in different position. The address of data is the position of data in the tape, or distance from the start of the tape to the start of the data segment. The unit is in bytes. For example, consider the following memory: (we are writing hexadecimal here)

```
Addr: | 00 | 01 | 02 | 03 | 04 | ...
Data: | 12 | 34 | 56 | 78 | 9A | ...
```

The address of data `12` is 0, that of `9A` is 4. The address of `1234` is 0, the address of `789A` is 03. 

Our data would be stored in the main memory, and we can take the address to a certain variable by this syntax: `&foo`, where `foo` is the name of the variable. In fact we can do this to a field of an object, like `&foo.a`, where `foo` is an instance of a struct/class and `a` is a field of `foo`. The address to data is called a *pointer*, and has type `T*` meaning it is a *pointer to data of type T*. In the machine level representation, a pointer is often an integer with a large enough size to represent all the addresses. This fact would be used below when we talk about memory-mapped IO, which we cast a certain address (integer) into a pointer.

We can also *dereference* the address by the following syntax: `*foo`, where `foo` is a pointer. The result of the dereferencing is of type `T` if the pointer is of type `T*`. Note that a pointer is often just an integer, it is the programmer's responsibility to make sure that the pointer they dereference is a valid one, i.e. correct address, correct type, correct alignment (see later section). One can first take the address to a certain memory, cast (modify) the type of the pointer, and dereference it. This is called *reinterpretation* (reinterpret the memory as a different type), and we have `reinterpretation_cast` in C++, which is pretty dangerous and you are discouraged to use it. 

> As pointers are also data and can be stored in memory, you can take the pointer of a pointer, and so on.

CPU executes instructions, which are commands telling the CPU what to do. In the machine level, we operate on *registers*. Registers are small and fast storages in the CPU, which are used for CPU operations. We have a limited number of registers (often < 100), with the size of a few bytes (4 bytes for our microcontroller, 8 bytes for modern computers). For example, we may have an addition instruction which adds two registers and store them in another register. We may also have a load instruction which loads the data from an address in the main memory, and a store instruction to store the data into an address in the main memory. Registers are not in the main memory, so there is no address for them, the compiler would allocate address and store the data back into memory when needed (out of register space, or when the user dereference a pointer, etc.).

> Main memory is large but slow, register is fast but small, so we would often run out of register space and have to wait for retrieving/sending data to the main memory. To solve this problem, modern computers and some microcontrollers have cache, which is faster than main memory but slower than registers, but a lot larger than registers. This is to provide something like a buffer, to store recently used or would be used data, so CPU can spend its time doing real work instead of waiting for data. We don't have this for our microcontroller so we would not explain it in details here.

### Integers

Integers are the basics of our program, we work with integers most of the time. Occasionally, when we want to interface with hardware, we may want precise control over the size of the integer we use, how should we do that?  The standards only defined the minimal range for normal integers such as `int`, the actual size is implementation defined. The safest and portable way is to use type aliases defined in `stdint.h`, which defines some fixed size integers, such as `uint8_t`, `int32_t`, etc.

However, does that mean we should use fixed size integers all over the place? For example, if we know a for loop counter is small, should we use `uint8_t` in order to save space? Well, you can but is *not recommended*. Machines have word size, which is the size which the machine can process in one go, normally the size of the register. That is typically 64 bits for desktops/notebooks, or 32 bits for STM32 chips that we use. Smaller size variables may require some additional processing, such as (un)signed extension or unaligned access for loading/storing the variable, which would be slower than word-sized variables. This is the reason for C/C++ having the `int` type but not fixed size integers all over the place. Also, the space you saved may not be that significant, especially if you have not considered type alignment and padding (see below).

### Floating-Point Numbers

In our team, we often do numerical processing, such as doing control loop. The variables we use are in general not integers but real numbers, so we use floating point numbers. Remember that floating point number is the approximation of the actual value with finite bitstring, in a representation similar to scientific notation. This means the operations or values we are dealing with are often approximations only, which have errors.

The first thing you should remember is that, do not check for equality for floating point numbers. A lot of errors can be introduced during the calculation, so even if the computation is deterministic and the IEEE standard guaranteed some basic operations are perfectly rounded, you may still get a value different from the analytical solution. If you really want to check for equality, figure out the acceptable error margin and check for that. *Do not* check against the `FLT_EPSILON`, please refer to this article for details: https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.

The second thing you should note is numerical stability. Operations such as adding small number to large number may cause loss of precision, as we only have finite precision (e.g. `1e30 + 1 = 1e30`). You may want to normalize the data before doing operations to reduce errors due to rounding. Reference: numerical analysis textbook, except that you don't really have to read the textbook: the articles mentioning the fancy algorithms you are going to implement would likely talk about numerical stability problem if that is important.

The last thing we have to consider is whether to use floating point number, and if we should use float or double. For normal applications, the FPU (floating-point processing unit) in our computer is pretty fast and are designed for double precision, so double would not be slower than float but we get higher precision. So, in general we want to use double for normal applications. However, for embedded systems, the FPUs are often single precision, or we may not have a FPU at all. Note that software-emulated floating-point is a lot slower, so you should see what your system supports, and avoid using something that requires software emulation. In general, our development board provides single precision floating-point processing.

There are a lot of things that we did not mention here. Please refer to this document if you want to learn more: https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html

### Arrays

We can store the data consecutively along the main memory in a sequence, the sequence is an array. The address of each element in the array would differ by the size of each element. A 2D array with one of the dimension fixed in size can be represented by a normal array, by considering the array as a normal array of large data where each element is an array of fixed size:

```
1 2 3 4
5 6 7 8  --> [1 2 3 4] [5 6 7 8] [9 A B C] ...
9 A B C
...
```

Arrays are basically just pointers to the first element of the array. Indexing of an array is just normal pointer addition (note that the address of `a + b` where their types are `T* + int` is `a + sizeof(T)*b`).

### Alignment & Padding

Our code would ultimately compile to assembly code, consisting of load/store and other operations. In most of the ISA (instruction set architecture, such as x86 and arm), we have to specify the size of the data we want to load, such as a byte, 2 bytes, 4 bytes, etc.

> Aside: that word size varies between architecture, so we would not count using word size here. And unfortunately, in x86, *word* used in specifying the load/store size is 2 bytes due to historical reason (8086)...

Different access size come with different *alignment*. For example, 2 bytes aligned means `address % 2 == 0`, 4 bytes aligned means `address % 4 == 0`, where address is in bytes. Often the alignment is the same as the access size. If the alignment requirement is not met, in some architecture there would be *performance penalty*, and in some architecture we may even have an *error* (alignment fault in arm).

For example, `uint32_t` are aligned to 4 bytes boundary in arm, and `uint8_t` are aligned to bytes only. Can we cast a `uint8_t*` to `uint32_t*` if we know that the pointer is pointing to 4 bytes? Or in other words, can we *safely reinterpret* a `uint8_t[4]` as `uint32_t`? In general this is not safe, because the pointer may not be aligned to 4 bytes boundary, dereferencing the pointer may cause alignment fault. Further more, the cast is actually undefined behavior as it violates type aliasing rule. The correct way to do this is to read each character and reassemble them into one `uint32_t` via bit operations, or do a `memcpy` to copy it to a destination which is correctly aligned, and read that location as `uint32_t`.

Recall that the addresses of the members of a struct in C/C++ always increase in declaration order. For example, consider the struct in the following code:

```C
#include <stdio.h>
#include <stdint.h>
typedef struct {
    int8_t mem1;
    int32_t mem2;
    int32_t mem3;
} Foo_t;

int main()
{
    Foo_t foo = {0, 0, 0};
    // print the addresses...
    printf("%p %p %p", (void*)&foo.mem1, (void*)&foo.mem2, (void*)&foo.mem3);
    return 0;
}
```

We have `&foo.mem1 < &foo.mem2 < &foo.mem3`. The trivial way would be have a layout that is packed together, i.e. the whole struct occupying 9 bytes. However, that would not be good in the sense of alignment: We cannot guarantee mem2 and mem3 to be aligned to 4 bytes. Running the code on an online compiler may give you output like this:

```
0x7ffe1475baa4 0x7ffe1475baa8 0x7ffe1475baac
```

So, the actual layout looks something like this (the left one is the real one, while the one on the right is the packed one):

```
    Real    vs    Packed
|    mem1   | |    mem1   |
| (padding) | -------------
| (padding) | |    mem2   |
| (padding) | |    mem2   |
------------- |    mem2   |
|    mem2   | |    mem2   |
|    mem2   | |------------
|    mem2   | |    mem3   |
|    mem2   | |    mem3   |
------------- |    mem3   |
|    mem3   | |    mem3   |
|    mem3   |
|    mem3   |
|    mem3   |
```

This way, the fields would be aligned to 4 bytes boundary given that the entire struct is aligned to 4 bytes boundary. It is trivial to see that the right one is not aligned, unless we make the address having `address % 4 = 3` which is weird (and by doing that you are also doing padding...). And in general the algorithm doing the padding is very simple: alignment requirement of the struct is the maximum alignment requirement of its members, and simply add padding until the member alignment requirement is satisfied.

> Reference: https://doc.rust-lang.org/stable/reference/type-layout.html#reprc-structs

Is there a way to pack the fields together? Yes, there is, but often the reason is to communicate with hardware. The hardware may ask the user to give a pointer to a certain struct (*descriptors/tables*), and the exact memory layout is very important. We can do this by using packed attribute, like this:

```C
// random example from the web
typedef struct __attribute__((__packed__)) 
{
    IP_ADDR     MyIPAddr;               // IP address
    IP_ADDR     MyMask;                 // Subnet mask
    IP_ADDR     MyGateway;              // Default Gateway
        // etc...
} APP_CONFIG;
```

And in fact, we can also specify the alignment requirement for our type by the aligned attribute, written like this: `__attribute__ ((aligned (n)))` where `n` is the alignment in bytes. We can combine two attributes like this: `__attribute__((packed, aligned(1)))`.

### Volatile

#### Compiler Optimization

This section explains the common code transforms done by the compiler for optimization, to explain the reason why we need volatile variables.

Consider the following code segment:

```C
int a, b;
a = 1;
b = 2;
```

It is obvious that we can reorder line 2 and 3 without changing the result. However, consider the following case:

```c
int a = 0, b = 0;
a = b + 1;
b = 1;
```

Changing line 2 and 3 would obviously have a different result, we call this a dependency. The compiler can construct the dependency graph of the program and reorder/modify it freely *as long as the behavior is not changed*. What we mean by behavior is the result of the program running in a *single thread*. Hence, the compiler can do a lot of low level optimization for us without affecting the result in most of the cases, until when we deal with embedded or multithreading. We would look into two of the most common cases, hardware interaction and interrupt, and discuss about multithreading in general at last.

### Hardware Register (Memory-Mapped IO)

Memory-mapped IO means that a certain physical address range is mapped to the register of the hardware peripherals, writing/reading from a specific address means writing/reading from a specific hardware peripheral register. Registers are usually used for obtaining status information (status register), configuration or giving commands to the hardware. Obviously, writing could introduce side-effect, and the register can be changed even if our code did not modify it. We would look into two common cases, polling and write-after-write as an example where compiler could mis-optimize, and require setting the variable as volatile to tell the compiler not to optimize in that case.

In the first example, we would consider a GPIO device that is reading the *digital input* of a pin, for example to detect is a switch is pressed or not. To keep it simple, we define the value to be 1 when the switch is pressed, and 0 when the switch is not pressed. We define the address of the register to be `0x1234`.

```c
// this code segment blocks until the switch is pressed
void foo() {
    uint8_t *press = (uint8_t*)0x1234;
    while (*press == 0) {}
}
```

Let us reason about the code from the compiler's perspective: You are reading a value from a pointer, and repeat if the value equals to 0, without doing any other things. For *normal* code, the value the pointer is referring *would not change* because you did not change it in the code, so the compiler would happily optimize the code into something like this:

```c
// correct optimization, incorrect behavior
uint8_t *press = (uint8_t*)0x1234;
uint8_t value = *press;
if (value == 0) {
    // dead loop here...
}
```

Indeed, you can verify this via [compiler explorer](https://godbolt.org/), using gcc with `-O2`:

```asm
foo():
        mov     r3, #4608
        ldrb    r3, [r3, #52]   @ zero_extendqisi2
        cmp     r3, #0
        bxne    lr
.L4:
        b       .L4
```

If you do not understand the assembly, it means loading the value the pointer is pointing to into the CPU register r3, exit the function if it does not equal to 0. Otherwise it would stuck in the loop from line 6 to 7, line 7 means going back to itself (without condition).

For most of the cases, this is indeed the correct optimization, based on the assumption that the behavior would not change in normal circumstances. However, this is clearly not what we want, we want to read the value every time. This is *our bug*, we have to tell the compiler not to optimize our read and writes as they are special. In this case, we should use the `volatile` modifier to tell the compiler that the memory is special.

```c
void foo()
{
    volatile uint8_t *press = (uint8_t*)0x1234;
    while (*press == 0) {}
}
```

And the compiler explorer output:

```asm
foo():
        mov     r2, #4608
.L2:
        ldrb    r3, [r2, #52]   @ zero_extendqisi2
        cmp     r3, #0
        beq     .L2
        bx      lr
```

Clearly, the compiler respects our instruction, even when you enable O3 optimization.

------

In the second example, we would look at a register called *lock register*, which is used for protecting other important registers. One have to write special sequence to the register in order to unlock write access to a group of registers, preventing a buggy program from accidentally writing invalid values to important registers and potentially brick the board. Similarly, we define the address of the register to be `0x1234`, and the sequence is writing `12` and `34` sequentially to unlock the register. Below is the code to unlock the lock register:

```c
void unlock() {
    uint8_t *lock = (uint8_t*)0x1234;
    *lock = 12;
    *lock = 34;
}
```

Except that it won't work, as we are talking about compiler optimization :) . From the compiler's perspective, overwriting a value previously written means that the previous value is useless, so it would happily rewrite into something like this:

```c
void unlock_misoptimized() {
    uint8_t *lock = (uint8_t*)0x1234;
    *lock = 34;
}
```

Assembly output:

```asm
unlock():
        mov     r3, #4608
        mov     r2, #34
        strb    r2, [r3, #52]
        bx      lr
```

This can also be solved by using the `volatile` modifier.

> Aside: Would adding reads in between two writes prevent such optimization? Most likely not, as compilers are able to store the value in register and return the register value for the reads, without writing the register value into the real memory location. Compilers are often smarter than you think.

### Interrupts

Interrupts are procedures (or functions if you like, but they are not common functions) that would be called when CPU receives certain signal, or when specific instruction is executed etc. Please Google for the exact definition if you want to. Long story short, interrupts may run at any time (except when you explicitly blocked them...), and it is run in-between your code. Interrupts can be treated like another thread, running some short functions when certain event happens.

For example, we may setup interrupts when we receive certain messages, increment a counter, and our code would poll for the counter to wait for message arrival. (This is not a good use of interrupt, but good example)

```c
int counter = 0;
// say this is our interrupt
void msg_rcv() {
    counter++;
}
void loop() {
    while (counter == 0) {}
    // processing here
}
```

Does this look familiar? Yes, you can just think of counter as the hardware register, and clearly there would be mis-optimization here as the compiler does not know that the counter value could change in the loop. The rest is just the same as the previous examples, and is left as exercise.

### Aside: General Multithreaded Environment

> If you don't know what is a thread, I would recommend you skipping this aside.

As we can guess from the above examples, using global variables to share states between multiple threads would likely cause bugs. Adding `volatile` modifier could instruct the compiler to respect our reads/writes, but is that enough in general multithreaded environments?

The answer is no, because of atomicity and memory order. First, memory access has to be atomic to prevent observing some partial updates, like the first half is updated while the second half is not updated. Secondly, the memory ordering has to be correct, as in the relaxed memory model there is no guarantee on the order of the updates between operations, variables updating sequentially can be observed updating in another order in another thread/core, memory barriers are required to enforce the memory ordering. Please refer to C++ memory ordering page for details if you want to learn more. Also, atomic is easy to mess up, so mutexes and other synchronization primitives are recommended instead of using atomics when performance is not that important.

Anyway, volatile is enough for single core case, which is our embedded system. 


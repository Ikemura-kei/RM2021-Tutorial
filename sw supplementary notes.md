# Software Supplementary Notes (SAN checker)

> WARNING: If you have difficulty understanding the C++ tutorial, please write some more C/C++ programs until you are comfortable with it, **before** reading this guide. Otherwise you may fail the san check...
>
> The main purpose of this note is to scare small children. This is now incomplete, so it can only scare some but not all.

We assume you have some basic understanding for C and C++, if not please refer to online tutorials. This guide would explain from very low level about the things you should at least have an idea about for embedded programming. It is OK if you don't completely understand the materials at the moment, if you are interested you can ask in the group or PM seniors.

There is often a TLDR section after a long section, which summarize what you should know or do, feel free to skip the explanation and just read the TLDR part. This guide includes some *asides*, which are not very useful but good to know. Feel free to ignore the asides and only read them before sleep, so you would fall asleep quicker. We would also provide a list of keywords for you to Google if you cannot understand the content, which is normal as the author is major in CS but not education nor English literature :).

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

> Keywords: numerical stability, floating-point processing unit, memory alignment, padding

### Integers

Integers are the basics of our program, we work with integers most of the time. Occasionally, when we want to interface with hardware, we may want precise control over the size of the integer we use, how should we do that?  The standards only defined the minimal range for normal integers such as `int`, the actual size is implementation defined. The safest and portable way is to use type aliases defined in `stdint.h`, which defines some fixed size integers, such as `uint8_t`, `int32_t`, etc.

However, does that mean we should use fixed size integers all over the place? For example, if we know a for loop counter is small, should we use `uint8_t` in order to save space? Well, you can but is *not recommended*. Machines have word size, which is the size which the machine can process in one go, normally the size of the register. That is typically 64 bits for desktops/notebooks, or 32 bits for STM32 chips that we use. Smaller size variables may require some additional processing, such as (un)signed extension or unaligned access for loading/storing the variable, which would be slower than word-sized variables. This is the reason for C/C++ having the `int` type but not fixed size integers all over the place. Also, the space you saved may not be that significant, especially if you have not considered type alignment and padding (see below).

### Floating-Point Numbers

In our team, we often do numerical processing, such as doing control loop. The variables we use are in general not integers but real numbers, so we use floating point numbers. Remember that floating point number is the approximation of the actual value with finite bitstring, in a representation similar to scientific notation. This means the operations or values we are dealing with are often approximations only, which have errors.

The first thing you should remember is that, do not check for equality for floating point numbers. A lot of errors can be introduced during the calculation, so even if the computation is deterministic and the IEEE standard guaranteed some basic operations are perfectly rounded, you may still get a value different from the analytical solution. If you really want to check for equality, figure out the acceptable error margin and check for that. *Do not* check against the `FLT_EPSILON`, please refer to this article for details: https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.

The second thing you should note is numerical stability. Operations such as adding small number to large number may cause loss of precision, as we only have finite precision (e.g. `1e30 + 1 = 1e30`). You may want to normalize the data before doing operations to reduce errors due to rounding. Reference: numerical analysis textbook, except that you don't really have to read the textbook: the articles mentioning the fancy algorithms you are going to implement would likely talk about numerical stability problem if that is important.

The last thing we have to consider is whether to use floating point number, and if we should use float or double. For normal applications, the FPU (floating-point processing unit) in our computer is pretty fast and are designed for double precision, so double would not be slower than float but we get higher precision. So, in general we want to use double for normal applications. However, for embedded systems, the FPUs are often single precision, or we may not have a FPU at all. Note that software-emulated floating-point is a lot slower, so you should see what your system supports, and avoid using something that requires software emulation. In general, our development board provides single precision floating-point processing.

There are a lot of things that we did not mention here. Please refer to this document if you want to learn more: https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html

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
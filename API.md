# Introduction #

To unify the environment across different platforms asmloader provides a simple to use and minimalistic API that basically consists of a set of wrappers to standard C functions.

At the beginning of execution of the provided machine code, the **ebx** register points to a table of function pointers. To use a specific function just perform a call to the address taken from the N-th pointer from the table. For example if you want to call the `getchar` function (placed at index 2 in the table) you can use this instruction:

`call [ebx+2*4] ; 2nd 32-bit pointer from the table`

Since the table contains direct pointers to the functions you need to use proper calling convention (on 32-bit x86 that's **cdecl**, so you need to remove the arguments from the stack after the function returns).

# API table #

The full API list follows:

| 0 | exit |
|:--|:-----|
| 1 | putchar |
| 2 | getchar |
| 3 | printf |
| 4 | scanf |

Example of usage (in nasm format):

```
push 'A'       ; \
call [ebx+1*4] ;  > putchar('A')
add esp, 4     ; /
```

In case of strings you can use e.g. this _hackish_ method:

```
push 1234
call after_text
db "printf format with number: %u", 0xa, 0
after_test:
call [ebx+3*4]
add esp, 8
```

Other functions might be added in the future, however I'll try to maintain backward compatibility.
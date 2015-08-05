This little app **executes headerless machine code** (compiled assembly code). It's meant to be an **aid in learning/teaching and playing with assembly** (or assembler as some call it) as well as the **right tool when you just need to execute some machine code**.

What it does:
  1. It allocated memory for Read/Write/Execute
  1. Loads the specified file to it
  1. And jumps to it (i.e. executes the binary code that was in the file)
  1. Plus it provides a minor [API](API.md) set.

Currently only **32-bit x86 Windows and 32-bit x86 GNU/Linux are supported**. The 64-bit assembly support is high on the TODO list.

**Newest version**:
[asmloader-0.0.2.zip](http://code.google.com/p/asmloader/downloads/detail?name=asmloader-0.0.2.zip)
#!/usr/bin/python
# A script that generates .c files that can be included by asmloader.
import subprocess

def generate_c_file(fname):
  print "Processing:", fname
  subprocess.check_call(['nasm', fname + ".nasm"])

  with open(fname, "rb") as f:
    d = f.read()

  out_fname = fname + ".c"
  with open(out_fname, "w") as f:
    f.write("#pragma once\n\n")
    f.write("unsigned char STUB_%s[] = {" % fname)
    for i, ch in enumerate(d):
      if i % 8 == 0:
        f.write("\n    ")

      comma = ", "
      if i == len(d) - 1:
        comma = ""

      f.write("0x%.2x%s" % (ord(ch), comma))

    f.write("\n};\n")


if __name__ == "__main__":
  for fname in [
      "x86_32_stub",
      "x86_64_mswin_stub",
      "x86_64_linux_stub",
  ]:
    generate_c_file(fname)

#!/usr/bin/env python3

import os, os.path
import re
import sys
from sys import platform
import subprocess

def stringtest_split(ff):
    result = ""
    for i, x in enumerate([x for x in ff.read()[:-1].split(" ") if x != " " and x != ""]):
        result += str(i) + ": \"" + x + "\"\n"
    return result

def stringtest_rev(ff):
    return ff.read()[::-1]

def stringtest_blanks(ff):
    result = ""
    str_in = ff.read().rstrip('\n')
    for i in range(len(str_in)):
        x = str_in[:i] + '_' + str_in[i+1:]
        result += str(i) + ": \"" + x + "\"\n"
    return result

string_test = sys.argv[1]
filename = ''
test_method = None
hint = ""

if string_test == 'split':
    filename = 'mystring.c'
    test_method = stringtest_split
    hint = "    Hint: make sure you are accounting for the last word in the phrase, which may not necessarily end in a space."
elif string_test == 'reverse':
    filename = 'string_reverse.c'
    test_method = stringtest_rev
    hint = "    Hint: remember to null-terminate your result string. You can compute the difference of the string length and your current index to find the index in the reversed string."
elif string_test == 'blanks':
    filename = 'string_blanks.c'
    hint = "    Hint: the number of elements in your resulting list should be the number of characters in the input string. What is the difference between a size_t and int type?"
    test_method = stringtest_blanks

if platform != 'win32':
    compile_result = subprocess.run(['clang', '-g', '-fsanitize=address', '-Wno-nullability-completeness', '-o', string_test+'.o', 'testers/'+string_test+'.c', filename])
    binary = string_test
else:
    compile_result = subprocess.run(['cl.exe', '-Zi', '-W3', '-fsanitize=address', '-D_CRT_SECURE_NO_WARNINGS', 'testers/'+string_test+'.c', filename])
    binary = string_test+'exe'

def stringtest(f):
    try:
        with open(f, 'r') as ff:
            return test_method(ff)
    except:
        return "stringtest: " + f + ": No such file or directory\n"

def test(f):
    run_result = subprocess.run(['./' + binary + '.o', f], capture_output=True, text=True)
    result = (stringtest(f).strip(), run_result.stdout.strip(), run_result.stderr.strip())
    if result[0] == result[1]:
        print("PASSED " + f)
    else:
        print("FAILED " + f)
        print("    Correct is '" + result[0] + "'")
        print("    Output  is '" + result[1] + "'")
        print(hint)
        if len(result[2]) > 0:
            print("    Standard Error is:\n")
            print(result[2])

if compile_result.returncode != 0:
    print("Compilation Failed.")
    exit(1)

for f in os.listdir("data"): 
    test(os.path.join("data", f))

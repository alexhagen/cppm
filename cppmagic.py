from IPython.core.magic import (Magics, magics_class, line_magic,
                                cell_magic, line_cell_magic)
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor
from .spinner import Spinner
import os
import subprocess
import select
import threading
import itertools
import time

@magics_class
class cppmagics(Magics):
    def __init__(self, **kwargs):
        self.compiler = 'g++'
        self.opts = '--std=c++11'
        super(cppmagics, self).__init__(**kwargs)

    @line_magic
    def compiler(self, line):
        arr = line.split()
        self.compiler = line[0]
        self.opts = line[1:]

    @cell_magic
    def cpp(self, line, cell):
        self.filename = line
        self.code = cell
        with open(self.filename + '.cpp', 'w') as f:
            f.write(self.code)

    @cell_magic
    def runcpp(self, line, cell):
        arr = line.split()
        self.cpp(line, cell)
        self.compile()
        self.run(arr[0], arr[1:])

    @cell_magic
    def runcppnb(self, line, cell):
        arr = line.split()
        self.cpp(line, cell)
        self.compile()
        self.runnb(arr[0], arr[1:])

    def compile(self):
        cmd = "{cmp} {opts} {inp} -o {out}".format(cmp=self.compiler,
                                                  opts=self.opts,
                                                  inp=self.filename + '.cpp',
                                                  out=self.filename + '.o')
        p = subprocess.Popen([cmd], stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, shell=True)
        (out, err) = p.communicate()
        if len(err) > 0:
            raise CompilationError(err)

    def run(self, binary, args):
        cmd = "./{cmd}.o {args}".format(cmd=binary, args=args)
        p = subprocess.Popen([cmd], stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, shell=True)
        (out, err) = p.communicate()
        if len(err) > 0:
            raise RuntimeError(err)
        print out

    def runnb(self, binary, args):
        cmd = "./{cmd}.o {args}".format(cmd=binary, args=args)
        p = subprocess.Popen([cmd], stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, shell=True)
        out = select.select(p.stdout, [], [], 1)
        err = select.select(p.stderr, [], [], 1)
        print out, err
        #print out

class CompilationError(Exception):
    def __init__(self, message):
        super(CompilationError, self).__init__(message)

class RuntimeError(Exception):
    def __init__(self, message):
        super(RuntimeError, self).__init__(message)

ip = get_ipython()

ip.register_magics(cppmagics)

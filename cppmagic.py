from IPython.core.magic import (Magics, magics_class, line_magic,
                                cell_magic, line_cell_magic)
import os
import subprocess
import select
import threading
import itertools
import time
import multiprocessing
import psutil
import signal

@magics_class
class cppmagics(Magics):
    def __init__(self, **kwargs):
        self.compiler = 'g++'
        self.opts = '--std=c++11'
        self.ps = []
        self.includes = []
        super(cppmagics, self).__init__(**kwargs)

    @line_magic
    def set_compiler(self, line):
        arr = line.split()
        self.compiler = arr[0]
        self.opts = arr[1:]

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

    @line_cell_magic
    def include(self, line, cell=None):
        self.includes.extend([line + '.h'])
        if cell is not None:
            with open(line + '.h', 'w') as f:
                f.write(cell)

    def compile(self):
        includes = ''
        for i in self.includes:
            includes += '-I/{fname} '.format(fname=i)
        opts = ''
        for o in self.opts:
            opts += '{opt} '.format(opt=o)
        cmd = "{cmp} {opts} {inp} -o {out} {inc}".format(cmp=self.compiler,
                                                  opts=opts,
                                                  inp=self.filename + '.cpp',
                                                  out=self.filename + '.o',
                                                  inc=includes)
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
            print "output: " + out
            raise RuntimeError(err)
        print out
        return p

    def _run(self, binary, args):
        cmd = "./{cmd}.o {args}".format(cmd=binary, args=args)
        p = subprocess.Popen([cmd], stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, shell=True)
        self.ps.extend([p])
        (out, err) = p.communicate()
        if len(err) > 0:
            raise RuntimeError(err)
        return p

    def runnb(self, binary, args):
        p = multiprocessing.Process(target=self._run, args=[binary, args])
        self.ps.extend([p])
        p.start()

    @line_magic
    def killall(self, line):
        for p in self.ps:
            pid = p.pid
            parent = psutil.Process(pid)
            for child in parent.children(recursive=True):
                os.kill(child.pid, signal.SIGTERM)
            p.terminate()
        self.ps = []

    @line_magic
    def print_output(self, line):
        for p in self.ps:
            p.poll(1)


class CompilationError(Exception):
    def __init__(self, message):
        super(CompilationError, self).__init__(message)

class RuntimeError(Exception):
    def __init__(self, message):
        super(RuntimeError, self).__init__(message)

ip = get_ipython()

ip.register_magics(cppmagics)

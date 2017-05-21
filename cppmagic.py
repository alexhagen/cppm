from IPython.core.magic import (Magics, magics_class, line_magic,
                                cell_magic, line_cell_magic)
import os
import subprocess

# The class MUST call this class decorator at creation time
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

    def compile(self):
        cmd = "{cmp} {opts} {inp} -o {out}".format(cmp=self.compiler,
                                                  opts=self.opts,
                                                  inp=self.filename + '.cpp',
                                                  out=self.filename)
        p = subprocess.Popen([cmd], stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, shell=True)

    def run(self, binary, args):
        cmd = "./{cmd} {args}".format(cmd=binary, args=args)
        p = subprocess.Popen([cmd], stdout=subprocess.PIPE, shell=True)
        print str(p.communicate()[0])


ip = get_ipython()

ip.register_magics(cppmagics)

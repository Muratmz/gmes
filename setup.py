#!/usr/bin/env python

# System imports
from os import getcwd
from glob import glob
from distutils.core import setup, Extension

# Third-party modules - we depend on numpy for everything
import numpy

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

PACKAGE = 'gmes'
VERSION = open('VERSION').read().strip()
base = getcwd() + '/'

pw_src_lst = glob('src/pw_*.cc')
pw_src_lst.extend(glob('src/pw_*.i'))
pw_dep_lst = glob('src/pw_*.hh')

# _pw_material module
_pw_material = Extension(name = 'gmes._pw_material',
                         sources = pw_src_lst,
                         depends = pw_dep_lst,
                         include_dirs = [numpy_include],
                         swig_opts = ['-c++', '-outdir', 'gmes'],
                         extra_compile_args=['-O'])

# _constants module
_constants = Extension(name = 'gmes._constants',
                       sources = ['src/constants.i',
                                  'src/constants.cc'],
                       depends = ['src/constants.hh'],
                       include_dirs = [numpy_include],
                       swig_opts = ['-c++', '-outdir', 'gmes'],
                       extra_compile_args=[])

setup(name = PACKAGE,
      version = VERSION,
      description = "GIST Maxwell's Equations Solver",
      long_description = """
      GMES is a Python package to solve the Maxwell's equations
      using the explicit Finite-Difference Time-Domain method.""",
      author = 'Kyungwon Chun',
      author_email = 'kwchun@gist.ac.kr',
      maintainer = 'Kyungwon Chun',
      maintainer_email = 'kwchun@gist.ac.kr',
      url = 'http://sourceforge.net/projects/gmes',
      license = 'http://www.gnu.org/licenses/gpl.html',
      packages = [PACKAGE],
      ext_modules = [_pw_material, _constants])

from skbuild import setup


setup(
  packages=[
    'geomstore',
    'procshape'
  ],
  cmake_source_dir='geomstore/src',
  cmake_install_dir='geomstore'
)

from setuptools import setup, Extension

ext = Extension(
    name="7colors.bindings",
    sources=[
        "7color/src/bindings.c",
        "7color/src/functions1.c",
        "7color/src/functions2.c",
    ],
    include_dirs=["7color/head"],
)

setup(
    name="7colors",
    version="0.1.0",
    description="Librairie pour le jeu des 7 couleurs",
    packages=["Py7colors"],
    ext_modules=[ext],
)

#python -m build
#pip install dist/your_library-0.1.0.tar.gz
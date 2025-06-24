from . import bindings  # Load the compiled C module

# Optional: re-export specific functions
hello1 = bindings.hello1

__version__ = "0.1.0"

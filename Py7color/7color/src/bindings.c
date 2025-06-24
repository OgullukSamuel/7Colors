#include "../head/bindings.h"

static PyObject* py_hello1(PyObject* self, PyObject* args) {
    print_hello1();
    Py_RETURN_NONE;
}

static PyMethodDef Methods[] = {
    {"hello1", py_hello1, METH_NOARGS, "Call print_hello1"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "bindings",  
    NULL, -1, Methods
};

PyMODINIT_FUNC PyInit_bindings(void) {
    return PyModule_Create(&module);
}



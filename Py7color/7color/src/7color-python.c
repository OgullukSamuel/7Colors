#define PY_SSIZE_T_CLEAN
#include "../head/7color-python.h"






PyObject* print_hello_world(PyObject* self, PyObject* args){
    printf("hello, python module's world\n");
    return Py_None;
}

PyObject* sum_two_numbers(PyObject* self, PyObject* args){
    double a, b;
    if(!PyArg_ParseTuple(args, "dd", &a, &b)){
        return NULL;
    }
    return PyFloat_FromDouble(a+b);
}

PyObject* sum_lists(PyObject* self, PyObject* args){
    PyObject* array1;
    PyObject* array2;
    if(!PyArg_ParseTuple(args,"OO",&array1,&array2)){
        return NULL;
    }
    Py_ssize_t size = PyList_Size(array1);
    double sum = 0;
    PyObject* sum_list = PyList_New(size);
    for(int i=0;i<size;i++){
        double item1 = PyFloat_AS_DOUBLE(PyList_GetItem(array1,i));
        double item2 = PyFloat_AS_DOUBLE(PyList_GetItem(array2,i));
        PyList_SET_ITEM(sum_list,i,PyFloat_FromDouble(item1+item2));
    }
    return sum_list;
}

PyObject* sub_lists(PyObject* self, PyObject* args){
    PyObject* array1;
    PyObject* array2;
    if(!PyArg_ParseTuple(args,"OO",&array1,&array2)){
        return NULL;
    }
    Py_ssize_t size = PyList_Size(array1);
    double sum = 0;
    PyObject* sum_list = PyList_New(size);
    for(int i=0;i<size;i++){
        double item1 = PyFloat_AS_DOUBLE(PyList_GetItem(array1,i));
        double item2 = PyFloat_AS_DOUBLE(PyList_GetItem(array2,i));
        PyList_SET_ITEM(sum_list,i,PyFloat_FromDouble(item1-item2));
    }
    return sum_list;
}

PyMethodDef verletFunctions[] = {
    {
        .ml_doc  = "this function prints hello world to the screen",
        .ml_flags= METH_VARARGS,
        .ml_name = "hello",
        .ml_meth = print_hello_world
    },
    {
        .ml_doc  = "sum ",
        .ml_flags= METH_VARARGS,
        .ml_name = "sum_two_numbers",
        .ml_meth = sum_two_numbers
    },
    {
        .ml_doc  = "sum on array",
        .ml_flags= METH_VARARGS,
        .ml_name = "sum_lists",
        .ml_meth = sum_lists
    },
    {
        .ml_doc  = "sub on array",
        .ml_flags= METH_VARARGS,
        .ml_name = "sub_lists",
        .ml_meth = sub_lists
    },
    
    {NULL, NULL, 0, NULL}
};



PyModuleDef seven_color_Module = {
    .m_base=PyModuleDef_HEAD_INIT,
    .m_doc = "Module for 7color",
    .m_name="7color",
    .m_methods=verletFunctions,
    .m_size=-1
};

PyMODINIT_FUNC PyInit_seven_color(){
    PyObject* module;
    
    module = PyModule_Create(&seven_color_Module);
    return module;
}

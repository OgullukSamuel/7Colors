#include "../head/Pygamestate.h"

static int GameState_init(PyscObject* self, PyObject *args, PyObject *kwds) {
    int map_size = 0;
    if (!PyArg_ParseTuple(args, "i", &map_size)) {
        return -1; 
    }
    srand(time(NULL) ^ clock());
	create_empty_game_state(self,map_size);
	fill_map(self);
	set_map_value(self, 0, map_size-1, 1);
	set_map_value(self, map_size-1, 0, 2);
    return 0;
}

static void GameState_free(PyscObject *self) {
    if (self->map) {
		free(self->map);
		self->map = NULL;
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyTypeObject PyscObjectType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "7colors.PyscObject",
    .tp_basicsize = sizeof(PyscObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)GameState_init,
    .tp_dealloc = (destructor)GameState_free,
};

static PyModuleDef sevencolor = {
    PyModuleDef_HEAD_INIT,
    .m_name = "7colors",
    .m_doc = "module pour les 7 couleurs",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_sevencolor(void) {
    PyObject *m;
    if (PyType_Ready(&PyscObjectType) < 0)
        return NULL;

    m = PyModule_Create(&sevencolor);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyscObjectType);
    if (PyModule_AddObject(m, "PyscObject", (PyObject *) &PyscObjectType) < 0) {
        Py_DECREF(&PyscObjectType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
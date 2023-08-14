#define PY_SSIZE_T_CLEAN
#include <python3.11/Python.h>

#include <iostream>
#include <string>

using std::string, std::cerr, std::endl;

static PyObject *SpamError;

static PyObject *spam_system(PyObject *self, PyObject *args) {
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command)) return NULL;
    sts = std::system(command);

    if (sts < 0) {
        PyErr_SetString(SpamError, "System command failed");
        return NULL;
    }

    return PyLong_FromLong(sts);
}

static PyMethodDef SpamMethods[] = {{"system", spam_system, METH_VARARGS, "Execute a shell command"},
                                    {NULL, NULL, 0, NULL}};

static struct PyModuleDef spammodule = {PyModuleDef_HEAD_INIT, "spam", NULL, -1, SpamMethods};

PyMODINIT_FUNC PyInit_spam(void) { return PyModule_Create(&spammodule); }

int main(int argc, char *argv[]) {
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        cerr << "Fatal error: cannot decode argv[0]" << endl;
    }

    if (PyImport_AppendInittab("spam", PyInit_spam) == -1) {
        cerr << "Error: could not extend in-built modules table" << endl;
    }

    Py_SetProgramName(program);

    Py_Initialize();

    PyObject *pmodule = PyImport_ImportModule("spam");
    if (!pmodule) {
        PyErr_Print();
        cerr << "Error: could not import module 'spam'" << endl;
    }

    PyMem_RawFree(program);
    return 0;
}

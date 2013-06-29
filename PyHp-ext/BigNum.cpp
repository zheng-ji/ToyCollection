#include "BigNum.h"
#include <python2.7/Python.h>
#include <iostream>

string add(int a,int b) {
    Bint num1(a);
    Bint num2(b);
    Bint ans = num1 + num2;
    return Bint::write(ans);
}

string multiple(int a,int b) {
    Bint num1(a);
    Bint num2(b);
    Bint ans = num1 * num2;
    return Bint::write(ans);
}

PyObject* wrap_add(PyObject* self,PyObject* args) {
    int a,b;
    if (!PyArg_ParseTuple(args,"i|i",&a,&b))
        return NULL;
    return Py_BuildValue("s",add(a,b).c_str());
}

PyObject* wrap_mul(PyObject* self,PyObject* args) {
    int a,b;
    if (!PyArg_ParseTuple(args,"i|i",&a,&b))
        return NULL;
    return Py_BuildValue("s",multiple(a,b).c_str());
}

static PyMethodDef bintMethods[] =
{
    {"add", wrap_add, METH_VARARGS, "Caculate N!"},
    {"mul", wrap_mul, METH_VARARGS, "Caculate N!"},
    {NULL, NULL,0,NULL}
};

extern "C"              //不加会导致找不到initexample
void initBint() {
    PyObject* m;
    m = Py_InitModule("Bint", bintMethods);
}

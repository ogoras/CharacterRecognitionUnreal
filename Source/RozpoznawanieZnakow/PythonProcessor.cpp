#include "PythonProcessor.h"
#include <Python.h>
#include "pyhelper.hpp"


PythonProcessor::PythonProcessor() {
	;
}

int PythonProcessor::calculate_int() {
	CPyInstance hInstance2;

	CPyObject pName = PyUnicode_FromString("pyemb3");
	CPyObject pModule = PyImport_Import(pName);

	if (pModule)
	{
		CPyObject pFunc = PyObject_GetAttrString(pModule, "getInteger");
		if (pFunc && PyCallable_Check(pFunc))
		{
			CPyObject pValue = PyObject_CallObject(pFunc, NULL);

			return PyLong_AsLong(pValue);
		}
		else
		{
			return -2;
		}

	}
	else
	{
		return -1;
	}
}

FString PythonProcessor::syspath() {
	char filename[] = "A:\\pyemb7.py";
	FILE* fp;

	Py_Initialize();

	fp = _Py_fopen(filename, "r");
	PyRun_SimpleFile(fp, filename);

	Py_Finalize();
	return TEXT("??");
}
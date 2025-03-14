#include "PythonApplicationModule.h"
#include "Log.h"

/*test*/
static PyObject* app_multiplyFloated(PyObject* self, PyObject* args)
{
	float a;
	float b;

	if (!PyArg_ParseTuple(args, "ff", &a, &b))
	{
		PyErr_SetString(PyExc_TypeError, "Expected an float and a float");
		return nullptr;
	}

	float result = a * b;
	return Py_BuildValue("f", result);
}

static PyObject* app_sendLog(PyObject* self, PyObject* args)
{
	int logLevel;
	const char* message;

	if (!PyArg_ParseTuple(args, "is", &logLevel, &message))
	{
		PyErr_SetString(PyExc_TypeError, "Expected an integer (log level) and a string (message)");
		return nullptr;
	}

	if (logLevel < LOG_LEVEL_NONE || logLevel > LOG_LEVEL_ERR)
	{
		PyErr_SetString(PyExc_ValueError, "Invalid log level. Must be between LOG_LEVEL_NONE and LOG_LEVEL_ERR.");
		return nullptr;
	}

	eLogLevel level = static_cast<eLogLevel>(logLevel);

	TRACE_LOG(level, "[PY_TRACE]: %s", message);
	Py_RETURN_NONE;
}

static PyMethodDef ApplicationMethods[] =
{
	/*test*/
	{ "f_multiply", app_multiplyFloated, METH_VARARGS, "Multiply two floats" },
	{ "PY_TRACE", app_sendLog, METH_VARARGS, "Log Python messages to C++ using TRACE_LOG." },
	{ nullptr, nullptr, 0, nullptr }
};

static PyModuleDef ApplicationModule =
{
	PyModuleDef_HEAD_INIT, "app", nullptr, -1, ApplicationMethods,
	nullptr, nullptr, nullptr, nullptr
};

PyMODINIT_FUNC PyInit_app(void)
{
	return PyModule_Create(&ApplicationModule);
}
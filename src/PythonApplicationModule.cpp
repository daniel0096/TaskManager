#include "PythonApplicationModule.h"
#include "Log.h"
#include "Config.h"

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
	const char* psz_message;

	memset(&psz_message, 0x0, sizeof(psz_message));

	if (!PyArg_ParseTuple(args, "is", &logLevel, &psz_message))
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

	std::string logMessage = "[PY_TRACE]: " + std::string(psz_message);
	TRACE_LOG(level, logMessage.c_str());

	Py_RETURN_NONE;
}

static PyObject* app_getApplicationStatus(PyObject* self, PyObject* args)
{
	/*test*/
	bool running = true; 
	return PyBool_FromLong(running);
}

static PyObject* app_getApplicationSize(PyObject* self, PyObject* args)
{
	int width = 1920;
	int height = 1080;

	PyObject* pyList = PyList_New(2);
	PyList_SetItem(pyList, 0, PyLong_FromLong(width));
	PyList_SetItem(pyList, 1, PyLong_FromLong(height));

	return pyList;
}

static PyObject* app_getApplicationName(PyObject* self, PyObject* args)
{
	return PyUnicode_FromString("Task manager");
}

static PyMethodDef ApplicationMethods[] =
{
	/*test*/
	{ "f_multiply", app_multiplyFloated, METH_VARARGS, "Multiply two floats" },
	/*test*/

	{ "PY_TRACE", app_sendLog, METH_VARARGS, "Log Python messages to C++ using TRACE_LOG." },
	{ "get_application_status", app_getApplicationStatus, METH_VARARGS, "Get application sttatus bool--running"},
	{ "get_application_size", app_getApplicationSize, METH_VARARGS, "Get application size"},
	{ "get_application_name", app_getApplicationName, METH_VARARGS, "Get application name"},

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
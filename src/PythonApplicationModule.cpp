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

static PyObject* app_getApplicationStatus(PyObject* self, PyObject* args)
{

}

static PyObject* app_getApplicationSize(PyObject* self, PyObject* args)
{
	/*
		optimal heap?
	*/
	CConfig cfg;
	std::map<std::string, std::string> applicationSize = cfg.GetConfigSettings();

	PyObject* pyDict = PyDict_New();
	if (!pyDict)
		return nullptr;

	for (const auto& pair : applicationSize)
	{
		PyObject* key = PyUnicode_FromString(pair.first.c_str());
		PyObject* value = PyUnicode_FromString(pair.second.c_str());

		if (!key || !value)
		{
			Py_XDECREF(key);
			Py_XDECREF(value);
			Py_DECREF(pyDict);
			return nullptr;
		}

		PyDict_SetItem(pyDict, key, value);

		Py_DECREF(key);
		Py_DECREF(value);
	}

	return pyDict;
}

static PyMethodDef ApplicationMethods[] =
{
	/*test*/
	{ "f_multiply", app_multiplyFloated, METH_VARARGS, "Multiply two floats" },
	/*test*/

	{ "PY_TRACE", app_sendLog, METH_VARARGS, "Log Python messages to C++ using TRACE_LOG." },
	{ "get_application_status", app_getApplicationStatus, METH_VARARGS, "Get application sttatus bool--running"},
	{ "get_width", app_getApplicationSize, METH_VARARGS, "Get application size"},

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
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
	const char* psz_message = {};

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

	auto level = static_cast<eLogLevel>(logLevel);

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
	CConfig config;
	const std::map<std::string, std::string>& cfgMap = config.GetConfigSettings();

#ifdef _DEBUG
	TRACE_LOG(LOG_LEVEL_LOG, "Config settings:");
#endif

	for (const auto& [key, value] : cfgMap)
		std::cout << key << " = " << value << std::endl;

	int width = 0, height = 0;

	auto widthIter = cfgMap.find("res_x");
	auto heightIter = cfgMap.find("res_y");

	if (widthIter != cfgMap.end())
		width = std::stoi(widthIter->second);

	if (heightIter != cfgMap.end())
		height = std::stoi(heightIter->second);

#ifdef _DEBUG
	TRACE_LOG(LOG_LEVEL_LOG, "Width %d, Height %d:", width, height);
#endif

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
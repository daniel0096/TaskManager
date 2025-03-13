#pragma once

#include <Python.h>
#include <iostream>

class PythonApplication
{
public:
	PythonApplication();
	~PythonApplication();

	void RunPythonScript(const std::string& script);
};
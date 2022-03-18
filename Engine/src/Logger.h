#pragma once
#include <iostream>

/* Useful logger function for displaying self defined warnings and errors */
class Logger
{
	static std::ostream& getStream();
	static bool flag;
	static std::string file_path;
public:
	Logger() = delete;

	enum class SEVERITY
	{
		LOW = 0,
		MEDIUM,
		HIGH
	};

	static void toFile(const std::string& filePath = "debug.err"); // call before any message functions to switch program to 
	static void message(const std::string& message);
	static void warning(const std::string& message, const SEVERITY severity);
	static void error(const std::string& message, const SEVERITY severity);

private:
	static std::string getSeverity(const SEVERITY severity);
};

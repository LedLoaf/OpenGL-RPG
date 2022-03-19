#include "Logger.h"
#include "Game.h"

#include <fstream>

bool Logger::flag = true;
std::string Logger::file_path = "debug.err";

std::ostream& Logger::getStream()
{
	static std::ofstream file(Logger::file_path);
	return Logger::flag ? std::cerr : file;
}

void Logger::toFile(const std::string& filePath)
{
	Logger::flag = false;
	Logger::file_path = filePath;
}

void Logger::message(const std::string& message)
{
	Logger::getStream() << message << "\n";
}

void Logger::warning(const std::string& message, const SEVERITY severity)
{
	Logger::getStream() << "WARNING: " << message << std::endl << "SEVERITY: " << Logger::getSeverity(severity) << "\n";
}

void Logger::error(const std::string& message, const SEVERITY severity)
{
	Logger::getStream() << "ERROR: " << message << std::endl << "SEVERITY: " << Logger::getSeverity(severity) << "\n";

	if (static_cast<int>(severity) > 0)
		Game::Exit = true;
	if (static_cast<int>(severity) > 1)
		throw;
}

std::string Logger::getSeverity(const SEVERITY severity)
{
	std::string sev;
	switch (static_cast<int>(severity)) {
		case 0:
			sev = "Low";
			break;
		case 1:
			sev = "Medium";
			break;
		default:
			sev = "High";
	}
	return sev;
}
#include "DelimiterSplit.h"

std::vector<std::string> delimiterSplit(const GLchar* str, const GLchar delim)
{
	std::vector<std::string> result;
	std::string temp;

	while (*str) {
		// if delimiter is found push-back temp string into result list and start on the next string
		if (*str == delim) {
			result.emplace_back(temp);
			temp.clear();
			str++;
		}
		temp.push_back(*str++);
	}
	result.emplace_back(temp);

	return result;
}

std::vector<std::string> delimiterSplit(const std::string& str, const GLchar delim)
{
	return delimiterSplit(str.c_str(), delim);
}
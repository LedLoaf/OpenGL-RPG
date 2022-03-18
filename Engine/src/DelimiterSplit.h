#pragma once
#include <string>
#include <vector>

#include <glad/glad.h>

/* Splits a string into x substrings dividing at all delimiter positions, returned as a vector eg. delimiter_split("jarvis.png", '.') -> {"jarvis", "png"} */

std::vector<std::string> delimiterSplit(const GLchar* str, GLchar delim);

std::vector<std::string> delimiterSplit(const std::string& str, GLchar delim);
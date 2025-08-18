#pragma once

#include <iostream>

std::wostream& operator<<(std::wostream& os, const char16_t* str);

std::wostream& operator<<(std::wostream& os, const std::string& str);
#pragma once

#include <iostream>

std::wostream& operator<<(std::wostream& os, const std::string& str);

std::wostream& operator<<(std::wostream& os, const std::u16string& str);

std::ostream& operator<<(std::ostream& os, const std::wstring& str);

std::ostream& operator<<(std::ostream& os, const std::u16string& str);

std::ostream& operator<<(std::ostream& os, const std::u32string& str);
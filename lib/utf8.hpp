#pragma once
#include <codecvt>
#include <locale>
using namespace std;

size_t utf8size(const char* str) {
  size_t len = 0;
  while (*str) len += (*str++ & 0xc0) != 0x80;
  return len;
}

u32string to_utf32(const string &s) {
  wstring_convert<codecvt_utf8<char32_t>, char32_t> conv;
  return conv.from_bytes(s);
}

std::string to_utf8(const std::u32string &s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(s);
}
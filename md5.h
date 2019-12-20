#pragma once
#include <cstdint>

uint8_t* md5(const char* message, uint64_t size, uint8_t digest[32]);

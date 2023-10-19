#pragma once

// For functions without a class
#define BOOM_BIND_FREE_FN(fn) std::bind(&fn, std::placeholders::_1, std::placeholders::_2)

// For functions with a class
#define BOOM_BIND_CLASS_FN(fn) std::bind(&fn, this, std::placeholders::_1, std::placeholders::_2)

#define BUFFER_SIZE 4096
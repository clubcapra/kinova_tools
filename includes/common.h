#pragma once

#define TOKENIZE(token) #token
#define _CONCAT(x, y) x##y
#define CONCAT(x, y) _CONCAT(x, y)
#pragma once

#define _TOKENIZE(x) #x 
#define TOKENIZE(token) _TOKENIZE(token)
#define _CONCAT(x, y) x##y
#define CONCAT(x, y) _CONCAT(x, y)
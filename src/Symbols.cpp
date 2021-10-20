#pragma once
#include "Symbols.h"

using namespace symbols;

bool symbols::isKey(char a) {
	if (a > 90 || a < 65) return false;
	if (a == 'I' || a == 'O' || a == 'P' || a == 'K' || a == 'L') return false;
	return true;
}
bool symbols::isSymbol(char a) {
	if (a == '0' || a == '2' || a == '4' || a == '8' || a == 'p' || a == '3' || a == '5' || a == '7' || a == '.' || a == '*' || a == '~' || a == '^' || a == '>' || a == '<')
		return true;
	else return false;
}
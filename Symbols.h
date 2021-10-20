#pragma once
enum CtrlSymbol {
	whole = '0',
	half = '2',
	quad = '4',
	oct = '8',
	hex = 'p',
	triplet = '3',
	quintuplet = '5',
	septuplet = '7',

	dot = '.',
	appoggiatura = '*',
	arpgInTime = '~',
	arpgOnTime,
	trill = '^',

	rit = '>',
	accel = '<',
};
enum JmpSymbol {
	repeat = ':',
	room = '|',
	DC = ']',
	DS = '$',
	fin = '!'
};
enum Key {
	C3 = 'Q', D3 = 'W', E3 = 'E', F3 = 'R', G3 = 'T', A3 = 'Y', B3 = 'U',
	C2 = 'A', D2 = 'S', E2 = 'D', F2 = 'F', G2 = 'G', A2 = 'H', B2 = 'J',
	C1 = 'Z', D1 = 'X', E1 = 'C', F1 = 'V', G1 = 'B', A1 = 'N', B1 = 'M'
};

bool isKey(char a) {
	if (a > 90 || a < 65) return false;
	if (a == 'I' || a == 'O' || a == 'P' || a == 'K' || a == 'L') return false;
	return true;
}
bool isSymbol(char a) {
	if (a == '0' || a == '2' || a == '4' || a == '8' || a == 'p' || a == '3' || a == '5' || a == '7' || a == '.' || a == '*' || a == '~' || a == '^' || a == '>' || a == '<')
		return true;
	else return false;
}

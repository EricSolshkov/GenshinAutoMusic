#pragma once
namespace symbols {
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

	bool isKey(char a);
	bool isSymbol(char a);
}

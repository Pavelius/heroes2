#include "stringbuilder.h"
#include "view.h"

using namespace draw;

enum tavern_s : unsigned char {
	MostGold, MostArmy, MostMapPiece,
};

static const char* tavern_say[] = {"Ѕартендер налил пива и произнес",
"Ѕартендер наклонилс€ и произнес",
"Ѕартендер расказал свежие слухи",
};
static const char* tavern_rumor[] = {"%1 собрал больше всего золота.",
"%1 имеет самую большую армию.",
"%1 нашел больше всего обелисков.",
};

void playeri::tavern() {
	draw::screenshoot surface;
	auto res = isevil(SYSTEME, SYSTEM);
	auto height = 300;
	char temp[512];
	stringbuilder sb(temp);
	sb.add("Ѕартендер сообщил:\n");
	sb.add("\"");
	sb.add(tavern_rumor[1], getname());
	sb.add("\"");
	while(ismodal()) {
		surface.restore();
		int ye;
		auto y = dialog(height, &ye);
		auto x = (draw::width - getwidth(TAVWIN, 0)) / 2;
		image(x + 3, y + 3, TAVWIN, 1);
		image(x + 3, y + 3, TAVWIN, 2 + counter % 20);
		image(x, y, TAVWIN, 0);
		auto y1 = y + getheight(TAVWIN, 0) + 8;
		textf(x, y1, 200, temp);
		button((width - getwidth(res, 1)) / 2, ye - getheight(res, 1), res, buttonok, {1, 1, 2}, KeyEnter);
		cursor(ADVMCO, 0);
		domodal();
	}
}
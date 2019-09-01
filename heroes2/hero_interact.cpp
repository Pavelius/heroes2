#include "main.h"

static bool isonetime(object_s id) {
	switch(id) {
	case CampFire:
	case TreasureChest:
	case ResourceObject:
	case WaterChest:
		return true;
	default:
		return false;
	}
}

bool heroi::interact(moveablei& object) {
	return true;
}

bool heroi::interact(short unsigned index, const pvar& object) {
	costi cost;
	auto player = getplayer();
	switch(object.type) {
	case Hero:
		break;
	case CastleVar:
		break;
	case Moveable:
		switch(object.moveable->element.type) {
		case Resource:
			cost.clear();
			cost.add(object.moveable->element.resource, object.moveable->element.value);
			if(player) {
				player->getresources() += cost;
				player->quickmessage(cost, "Вы нашли ресурс\n(%-1)",
					getstr(object.moveable->element.resource));
			}
			return true;
		case Artifact:
			return true;
		case Object:
			return isonetime(object.moveable->element.object);
		}
		break;
	}
	return false;
}
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

bool heroi::interact(short unsigned index, const pvar& object) {
	resource_s resource;
	switch(object.type) {
	case Hero:
		break;
	case CastleVar:
		break;
	case Moveable:
		switch(object.moveable->element.type) {
		case Resource:
			resource = object.moveable->element.resource;
			return true;
		case Object:
			return isonetime(object.moveable->element.object);
		}
		break;
	}
	return false;
}
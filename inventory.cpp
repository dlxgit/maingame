#include "inventory.h"

int GetMaxQuantity(NameItem & name)
{
	switch (name)
	{
	case DRINK:
		return 1;
	case PISTOL:
		return 12;
	case RIFLE:
		return 30;
	case AMMO:
		return 1;
	case KEY:
		return 1;
	case MIXTURE:
		return 1;
	case GRENADE:
		return 1;
	}
}

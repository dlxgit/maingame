#include "explosion.h"

void DeleteExplosionList(vector<Explosion> & explosions)
{
	for (vector<Explosion>::iterator it = explosions.begin(); it != explosions.end();)
	{
		it = explosions.erase(it);
	}
}
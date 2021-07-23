#include "Ball.h"

Ball::Ball(const std::string& name, int ID, int probability)
	:
	Item(name, ID),
	probability(probability)
{
}

void Ball::UseItem(Pokemon& pkmn)
{
	printf("Ball go!\n");
}

int Ball::GetProbability() const
{
	return probability;
}

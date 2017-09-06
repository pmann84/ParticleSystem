#include "particle_utils.h"


double ps::generate_random_number_in_range(double begin, double end)
{
	double rand_num = (double)rand() / RAND_MAX;
	double random_range = begin + rand_num * (end - begin);
	return random_range;
}

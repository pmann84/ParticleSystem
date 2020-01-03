#ifndef __PARTICLE_UTILS_H__
#define __PARTICLE_UTILS_H__

#include <cstdlib>
#include <vector>
#include <memory>
#include <random>
#include <iostream>

namespace ps
{
	inline double generate_random_number_in_range(double begin, double end)
   {
      std::uniform_real_distribution<double> uniform_dist(begin, end);
      std::random_device random_device;
      std::default_random_engine rand(random_device());
      return uniform_dist(rand);
   }

	template<class TYPE>
	unsigned generate_normal_random_variate(TYPE mean, TYPE variance)
	{
		std::default_random_engine generator;
		auto dist = std::normal_distribution<double>(mean, variance);
		const double random_step = dist(generator);
		return (TYPE)random_step;
	}

	//intersect_type ps::line_line_intersection(ps::vector3d start1, ps::vector3d end1, ps::vector3d start2, ps::vector3d end2);
}

#endif // __PARTICLE_UTILS_H__

#ifndef __PARTICLE_GROUP_H__
#define __PARTICLE_GROUP_H__

#include <vector>
#include <memory>
#include <random>
#include <iostream>

#include "vector.h"
#include "particle.h"
#include "domain.h"

namespace ps
{
	using domain_ptr = std::shared_ptr<idomain>;

	class particle_group
	{
	public:
		particle_group();
		explicit particle_group(unsigned int max_particles, unsigned int particle_emission_rate);

		void max_particles(int max) { m_max_particles = max; }
		unsigned int max_particles() const { return m_max_particles; }

		void time_interval(double time_interval) { m_dt = time_interval; }
		double time_interval() const { return m_dt; }

		particle_group& update();
		particle_group& source(domain_ptr source_domain);
		particle_group& sink(domain_ptr sink_domain);
		particle_group& velocity(domain_ptr vel_domain, double speed, bool randomised = false, int variance = 1);
		particle_group& bounce(domain_ptr bounce_domain, double dampening_factor = 1.0);
		particle_group& stick(domain_ptr stick_domain);
		particle_group& constant_force(vector3d direction);

		void particle_life(int steps, bool randomised, int variance = 1);
		std::vector<particle> particles() const { return m_particles; };
		void draw_domains(idomain_draw& drawer) const;

	private:
		std::vector<particle> m_particles;
		domain_ptr m_source_domain;
		domain_ptr m_sink_domain;

		domain_ptr m_velocity_domain;
		double m_velocity_speed;
		bool m_randomised_velocity_speed;
		unsigned int m_velocity_variance;

		domain_ptr m_bounce_domain;
		double m_bounce_dampening;

		domain_ptr m_stick_domain;
		vector3d m_const_force_vector;
		unsigned int m_max_particles;
		unsigned int m_particle_emission_rate;
		double m_dt;
		unsigned int m_sim_steps;

		unsigned int m_particle_life;
		bool m_randomised_particle_life;
		unsigned int m_particle_life_variance;

		vector3d calculate_force(vector3d position, vector3d velocity);
		void advance_particle(particle& particle);
		unsigned int generate_particle_life() const;
		unsigned int num_particles_alive();

		double generate_particle_speed() const;
	};
}

#endif // __PARTICLE_GROUP_H__

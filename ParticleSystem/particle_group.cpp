#include "particle_group.h"

ps::particle_group::particle_group() : particle_group(100, 10)
{
}

ps::particle_group::particle_group(unsigned max_particles, unsigned particle_emission_rate) : m_const_force_vector(0.0,0.0,0.0),
																							  m_particles(max_particles),
																							  m_max_particles(max_particles), 
																							  m_particle_emission_rate(particle_emission_rate), 
																							  m_dt(0.01), 
																							  m_sim_steps(0),
																							  m_particle_life(100),
																							  m_randomised_particle_life(false),
																							  m_particle_life_variance(1)
{
	for (unsigned int i = 0; i < m_particles.size(); ++i)
	{
		// Create particle initialised to nothing
		particle new_particle;
		new_particle.age(0);
		m_particles[i] = new_particle;
	}
}

ps::particle_group& ps::particle_group::update()
{
	for (auto& particle : m_particles)
	{
		// Particle destruction
		bool particle_entered_sink = false;
		if (m_sink_domain)
		{
			particle_entered_sink = m_sink_domain->has_entered_domain(particle);
		}
		if (!particle.is_alive() || particle_entered_sink)
		{
			// Kill any particles that are past their life or hit the sink
			particle.reset();
		}
	}

	// Figure out number of particles to create - will now always be > 0
	unsigned int num_p_alive = num_particles_alive();
	int num_particles_available_to_create = m_max_particles - num_p_alive;
	int num_particles_to_create = num_particles_available_to_create > m_particle_emission_rate ? m_particle_emission_rate : num_particles_available_to_create;

	for (auto& particle : m_particles)
	{
		// Particle Simulation
		if (particle.is_alive())
		{
			// Advance particle position using *PHYSICS*
			advance_particle(particle);
		}
		else
		{
			if (num_particles_to_create > 0)
			{
				// Particle Creation
				// Generate particles from source (if any)
				// Get position from source domain 
				vector3d pos = m_source_domain->generate_position();
				// Get velocity using velocity domain
				vector3d vel = m_velocity_domain->generate_velocity(pos);
				// Generate particle life
				int life = generate_particle_life();
				// Update a particle that is dead
				particle.position(pos);
				particle.velocity(vel);
				particle.life(life);
				particle.birthday();
				// Decrease counter
				num_particles_to_create--;
			}
		}
	}
	m_sim_steps++;

	std::cout << "Particles Alive: " << num_particles_alive() << std::endl;
	return *this;
}

ps::particle_group& ps::particle_group::source(domain_ptr source_domain)
{
	m_source_domain = source_domain;
	return *this;
}

ps::particle_group& ps::particle_group::sink(domain_ptr sink_domain)
{
	m_sink_domain = sink_domain;
	return *this;
}

ps::particle_group& ps::particle_group::velocity(domain_ptr vel_domain)
{
	m_velocity_domain = vel_domain;
	return *this;
}

ps::particle_group& ps::particle_group::constant_force(vector3d direction)
{
	m_const_force_vector = direction;
	return *this;
}

void ps::particle_group::particle_life(int steps, bool randomised = false, int variance)
{
	m_particle_life = steps;
	m_randomised_particle_life = randomised;
	m_particle_life_variance = variance;
}

void ps::particle_group::draw_domains(idomain_draw& drawer)
{
	m_source_domain->draw(drawer);
	m_velocity_domain->draw(drawer);
	if (m_sink_domain)
	{
		m_sink_domain->draw(drawer);
	}
}

ps::vector3d ps::particle_group::calculate_force(vector3d position, vector3d velocity)
{
	return m_const_force_vector;
}

void ps::particle_group::advance_particle(particle& particle)
{
	// Advance particle position using *PHYSICS* - here most actions 
	// can be calculated in terms of forces that can be used in newtons 
	// 3rd law solution, algorithm is thus (midpoint/euler-richardson):
	// First update old position
	particle.previous_position() = particle.position();
	// Get accn at current pos, vel and time
	vector3d acc_n = calculate_force(particle.position(), particle.velocity()) / particle.mass();
	// Calculate velocity at mid point of time step
	vector3d vel_mid = particle.velocity() + 0.5*acc_n*m_dt;
	// Calculate position at mid point of time step
	vector3d pos_mid = particle.position() + 0.5*particle.velocity()*m_dt;
	// Calculate acc at mid point of time step, using vel_mid, pos_mid and t + 0.5dt
	vector3d acc_mid = calculate_force(pos_mid, vel_mid) / particle.mass();
	// Now calculate the final pos and vel from mid points!
	vector3d vel_np1 = particle.velocity() + 0.5*acc_mid*m_dt;
	vector3d pos_np1 = particle.position() + 0.5*vel_mid*m_dt;
	// Update the particle position
	particle.position(pos_np1);
	particle.velocity(vel_np1);
	// Increment the age of the particle
	particle.birthday();
}

unsigned ps::particle_group::generate_particle_life()
{
	if (m_randomised_particle_life)
	{
		std::default_random_engine generator;
		auto dist = std::normal_distribution<double>(m_particle_life, m_particle_life_variance);
		double random_step = dist(generator);
		return (int)random_step;
	}
	return m_particle_life;
}

unsigned ps::particle_group::num_particles_alive()
{
	unsigned int num_particles_alive = 0;
	for (auto& particle : m_particles)
	{
		if (particle.is_alive())
		{
			num_particles_alive++;
		}
	}
	return num_particles_alive;
}

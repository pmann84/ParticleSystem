#include "particle_group.h"

ps::particle_group::particle_group() : particle_group(100, 10)
{
}

ps::particle_group::particle_group(unsigned max_particles, unsigned particle_emission_rate) : m_particles(max_particles),
																							  m_const_force_vector(0.0,0.0,0.0),
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
		particle new_particle(i);
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
	const unsigned int num_p_alive = num_particles_alive();
	const int num_particles_available_to_create = m_max_particles - num_p_alive;
	int num_particles_to_create = num_particles_available_to_create > m_particle_emission_rate 
										? m_particle_emission_rate 
										: num_particles_available_to_create;

	for (auto& particle : m_particles)
	{
		// Particle Simulation
		if (particle.is_alive())
		{
			if (m_stick_domain && m_stick_domain->has_entered_domain(particle))
			{
				// Update position to be intersection on the domain
				vector3d new_pos = m_stick_domain->particle_domain_intersection(particle);
				particle.position(new_pos);
				// Set velocity and accn to 0 so it doesnt move
				particle.velocity(vector3d());
			}
			else
			{
				if (m_bounce_domain && m_bounce_domain->has_entered_domain(particle))
				{
					// Update position to be intersection on the domain
					vector3d new_pos = m_bounce_domain->particle_domain_intersection(particle);
					particle.position(new_pos);
					// Update velocity to be appropriate for a bounce off the domain
					vector3d domain_norm = m_bounce_domain->normal();
					vector3d bounce_velocity = -m_bounce_dampening*particle.velocity();
					double ct = cos(M_PI);
					double st = sin(M_PI);
					double omct = (1.0 - ct);
					double R11 = ct + domain_norm[0] * domain_norm[0] * omct;
					double R12 = domain_norm[0] * domain_norm[1] * omct - domain_norm[2] * st;
					double R13 = domain_norm[0] * domain_norm[2] * omct + domain_norm[1] * st;
					double R21 = domain_norm[1] * domain_norm[0] * omct + domain_norm[2] * st;
					double R22 = ct + domain_norm[1] * domain_norm[1] * omct;
					double R23 = domain_norm[1] * domain_norm[2] * omct - domain_norm[0] * st;
					double R31 = domain_norm[2] * domain_norm[0] * omct - domain_norm[1] * st;
					double R32 = domain_norm[2] * domain_norm[1] * omct + domain_norm[0] * st;
					double R33 = ct + domain_norm[2] * domain_norm[2] * omct;
					vector3d new_vel = vector3d(bounce_velocity[0]*R11 + bounce_velocity[1]*R12 + bounce_velocity[2]*R13, 
												bounce_velocity[0]*R21 + bounce_velocity[1]*R22 + bounce_velocity[2]*R23, 
												bounce_velocity[0]*R31 + bounce_velocity[1]*R32 + bounce_velocity[2]*R33);
					particle.velocity(new_vel);
				}
				// Advance particle position using *PHYSICS*
				advance_particle(particle);
			}
			// Increment the age of the particle
			particle.birthday();
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
				vector3d vel = m_velocity_domain->generate_velocity(pos, generate_particle_speed());
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

ps::particle_group& ps::particle_group::velocity(domain_ptr vel_domain, double speed, bool randomised, int variance)
{
	m_velocity_domain = vel_domain;
	m_velocity_speed = speed;
	m_randomised_velocity_speed = randomised;
	m_velocity_variance = variance;
	return *this;
}

ps::particle_group& ps::particle_group::bounce(domain_ptr bounce_domain, double dampening_factor)
{
	m_bounce_domain = bounce_domain;
	m_bounce_dampening = dampening_factor;
	return *this;
}

ps::particle_group& ps::particle_group::stick(domain_ptr stick_domain)
{
	m_stick_domain = stick_domain;
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

void ps::particle_group::draw_domains(idomain_draw& drawer) const
{
	if (m_source_domain)
	{
		m_source_domain->draw(drawer);
	}
	if (m_velocity_domain)
	{
		m_velocity_domain->draw(drawer);
	}
	if (m_sink_domain)
	{
		m_sink_domain->draw(drawer);
	}
	if (m_stick_domain)
	{
		m_stick_domain->draw(drawer);
	}
	if (m_bounce_domain)
	{
		m_bounce_domain->draw(drawer);
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
	particle.previous_position(particle.position());
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
}

unsigned ps::particle_group::generate_particle_life() const
{
	if (m_randomised_particle_life)
	{
		std::default_random_engine generator;
		auto dist = std::normal_distribution<double>(m_particle_life, m_particle_life_variance);
		const double random_step = dist(generator);
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

double ps::particle_group::generate_particle_speed() const
{
	if (m_randomised_velocity_speed)
	{
		std::default_random_engine generator;
		auto dist = std::normal_distribution<double>(m_velocity_speed, m_velocity_variance);
		const double random_step = dist(generator);
		return random_step;
	}
	return m_velocity_speed;
}
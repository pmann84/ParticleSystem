#include "particle.h"

ps::particle::particle() : m_position(0.0, 0.0, 0.0),
                           m_velocity(0.0, 0.0, 0.0),
                           m_colour(255, 255, 255),
                           m_alpha(0.0),
						   m_radius(1.0),
                           m_mass(1.0),
                           m_life(-1),
						   m_age(0),
						   m_visible(true)
{
}

ps::particle::particle(vector3d position, vector3d velocity) : m_position(position),
															   m_velocity(velocity),
															   m_colour(255, 255, 255),
															   m_alpha(0.0),
															   m_radius(1.0),
															   m_mass(1.0),
															   m_life(-1),
															   m_age(0),
															   m_visible(true)
{
}

bool ps::particle::is_alive() const
{
	return m_age > 0 && m_age < m_life;
}

void ps::particle::reset()
{
	m_age = 0;
}

void ps::particle::birthday()
{
	m_age += 1;
}

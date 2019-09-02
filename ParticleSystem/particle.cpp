#include "particle.h"

ps::particle::particle(unsigned int id) : particle(id, vector3d(), vector3d())
{
}

ps::particle::particle(unsigned int id, vector3d position, vector3d velocity) : m_id(id),
																	   m_position(position),
																	   m_velocity(velocity),
																	   m_colour(255, 255, 255),
																	   m_alpha(0.0),
																	   m_radius(1.0),
																	   m_mass(1.0),
																	   m_life(0),
																	   m_age(0),
																	   m_visible(true),
                                                      m_static(false)
{
}

bool ps::particle::is_dead() const
{
   return m_age >= m_life;
}

void ps::particle::reset()
{
	m_age = 0;
   m_life = 0;
}

void ps::particle::birthday()
{
	m_age += 1;
}

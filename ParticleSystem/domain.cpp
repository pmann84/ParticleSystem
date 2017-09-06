#include "domain.h"
#include "particle_utils.h"

ps::domain::~domain()
{
}

// POINT DOMAIN
ps::point_domain::point_domain(vector3d point, double speed) : domain(), m_point(point), m_speed(speed)
{
}

ps::vector3d ps::point_domain::generate_position()
{
	return m_point;
}

ps::vector3d ps::point_domain::generate_velocity(vector3d start_position)
{
	vector3d velocity_vector(m_point - start_position);
	velocity_vector.normalise();
	velocity_vector *= m_speed;
	return velocity_vector;
}

// LINE DOMAIN
ps::line_domain::line_domain(vector3d begin, vector3d end, double speed) : m_begin(begin), m_end(end), m_speed(speed)
{
}

ps::vector3d ps::line_domain::generate_position()
{
	vector3d line_dir = m_end - m_begin;
	double random_max = line_dir.norm();
	line_dir.normalise();
	double random_min = 0.0;
	double random_length = generate_random_number_in_range(random_min, random_max);
	vector3d new_pt = m_begin + random_length * line_dir;
	return new_pt;
}

ps::vector3d ps::line_domain::generate_velocity(vector3d start_position)
{
	vector3d pos_for_vel = generate_position();
	vector3d velocity_vector(pos_for_vel - start_position);
	velocity_vector.normalise();
	velocity_vector *= m_speed;
	return velocity_vector;
}

// DISK DOMAIN
ps::disk_domain::disk_domain(vector3d centre, double inner_radius, double outer_radius, double zenith, double azimuth, double speed) : m_disc_centre(centre), m_inner_radius(inner_radius), m_outer_radius(outer_radius), m_theta(zenith), m_phi(azimuth), m_speed(speed)
{
}

ps::vector3d ps::disk_domain::generate_position()
{
	// P(t) = rcos(t) u + rsin(t) nxu + C
	// here r = radius, C = (x, y, z) = centre
	// n = (nx, ny, nz) = normal vector
	// u = (ux, uy, uz) = vector perpendicular to n
	// Can define angles from horizontal and vertical, theta (0, pi) and phi (-pi, pi) (repsectively) to give
	// n = (cos(phi)sin(theta), sin(theta)sin(phi), cos(theta))
	// u = (-sin(phi), cos(phi), 0)
	// nxu = (cos(theta)cos(phi), cs(theta)sin(phi), -sin(theta))
	// For OpenGL to get a flat circle with normal facing upwards (y direction in OGL)
	// we must choose

	// Check first to see if its a circle or an annulus
	double random_radius = 0.0;
	if (m_inner_radius == 0.0f)
	{
		random_radius = std::sqrt(generate_random_number_in_range(0.0f, m_outer_radius));
	}
	else
	{
		// First generate random radius in annulus
		// Calculate radius ratios
		double inner_R = m_inner_radius;
		double outer_R = m_outer_radius / m_inner_radius;
		// To generate a suitable r is to take random variables u and v 
		// with u chosen uniformly in[R2, R1] 
		// and v chosen uniformly in[0, R2 + R1],
		double u_rand = generate_random_number_in_range(outer_R, inner_R);
		double v_rand = generate_random_number_in_range(0, outer_R + inner_R);
		//and then take r = u if v < u and r = R2 + R1 - u if v >= u.
		if (v_rand < u_rand)
		{
			random_radius = u_rand;
		}
		else
		{
			random_radius = outer_R + inner_R - u_rand;
		}
	}

	// Generate random angle
	double random_angle = generate_random_number_in_range(0.0, M_PI);
	// Generate cartesian coordinates - x = sqrt(r) * cos(th), y = sqrt(r) * sin(th)
	vector3d new_pos(random_radius * cos(random_angle), random_radius * sin(random_angle), 0.0);

	// Calculate the normal and perp vectors
	//vector3d normal(cos(m_phi)*sin(m_theta), sin(m_theta)*sin(m_phi), cos(m_theta));
	vector3d perp(-sin(m_phi), cos(m_phi), 0);
	vector3d ncrossp(cos(m_theta)*cos(m_phi), cos(m_theta)*sin(m_phi), -sin(m_theta));

	// Calculate the final position
	vector3d newpos(random_radius*cos(random_angle)*perp + random_radius*sin(random_angle)*ncrossp + m_disc_centre);
	return newpos;
}

ps::vector3d ps::disk_domain::generate_velocity(vector3d start_position)
{
	vector3d pos_for_vel = generate_position();
	vector3d velocity_vector(pos_for_vel - start_position);
	velocity_vector.normalise();
	velocity_vector *= m_speed;
	return velocity_vector;
}

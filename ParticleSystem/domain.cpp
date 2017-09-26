#include "domain.h"
#include "particle_utils.h"

// POINT DOMAIN
ps::point_domain::point_domain(vector3d point, double speed) : idomain(), m_point(point), m_speed(speed)
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

bool ps::point_domain::has_entered_domain(particle particle)
{
	// c is point from domain a and b are start and end of line
	// need to return distance(A, C) + distance(B, C) == distance(A, B)
	vector3d start_point = particle.previous_position();
	vector3d end_point = particle.position();
	return start_point.distance(m_point) + end_point.distance(m_point) == start_point.distance(end_point);
}

void ps::point_domain::draw(idomain_draw& draw_interface)
{
	draw_interface.draw(this);
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

bool ps::line_domain::has_entered_domain(particle particle)
{
	// Intersection point 
	// Px = ((x1*y2 - y1*x2)(x3 - x4) - (x1 - x2)(x3*y4 - y3*x4))/((x1 - x2)(y3 - y4) - (y1 - y2)(x3 - x4))
	// Py = ((x1*y2 - y1*x2)(y3 - y4) - (y1 - y2)(x3*y4 - y3*x4))/((x1 - x2)(y3 - y4) - (y1 - y2)(x3 - x4))
	//double intersect_x = 
	return false;
}

void ps::line_domain::draw(idomain_draw& draw_interface)
{
	draw_interface.draw(this);
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
		random_radius = generate_random_number_in_range(0.0f, m_outer_radius);
	}
	else
	{
		random_radius = generate_random_number_in_range(m_inner_radius, m_outer_radius);
		//// First generate random radius in annulus
		//// Calculate radius ratios
		//double inner_R = m_inner_radius;
		//double outer_R = m_outer_radius / m_inner_radius;
		//// To generate a suitable r is to take random variables u and v 
		//// with u chosen uniformly in[R2, R1] 
		//// and v chosen uniformly in[0, R2 + R1],
		//double u_rand = generate_random_number_in_range(outer_R, inner_R);
		//double v_rand = generate_random_number_in_range(0, outer_R + inner_R);
		////and then take r = u if v < u and r = R2 + R1 - u if v >= u.
		//if (v_rand < u_rand)
		//{
		//	random_radius = u_rand;
		//}
		//else
		//{
		//	random_radius = outer_R + inner_R - u_rand;
		//}
	}

	// Generate random angle
	double random_angle = generate_random_number_in_range(0.0, 2*M_PI);
	return generate_position(random_radius, random_angle);

}

ps::vector3d ps::disk_domain::generate_velocity(vector3d start_position)
{
	vector3d pos_for_vel = generate_position();
	vector3d velocity_vector(pos_for_vel - start_position);
	velocity_vector.normalise();
	velocity_vector *= m_speed;
	return velocity_vector;
}

bool ps::disk_domain::has_entered_domain(particle particle)
{
	// 0 = disjoint(no intersection)
	// 1 =  intersection in the unique point *I0
	// 2 = the  segment lies in the plane
	vector3d start = particle.previous_position();
	vector3d end = particle.position();
	vector3d normal(cos(m_phi)*sin(m_theta), sin(m_theta)*sin(m_phi), cos(m_theta));

	// plane point = m_disc_centre
	vector3d u = end - start;
	vector3d w = start - m_disc_centre;

	double D = inner_product(normal, u);
	double N = -inner_product(normal, w);
	
	if (std::abs(D) < 0.00000001) // segment is parallel to plane
	{
		if (N == 0) 
		{
			return true; // segment lies in plane (2)
		}
		else
		{
			return false; // no intersection (0)
		}			 
	}
	// they are not parallel
	// compute intersect param
	const double sI = N / D;
	if (sI < 0 || sI > 1)
	{
		return false; // no intersection (0)
	}
	vector3d intersect_point = start + sI * u; // compute segment intersect point
	double radius_from_centre = (intersect_point - m_disc_centre).norm();
	return radius_from_centre < m_outer_radius && radius_from_centre > m_inner_radius; // intersection in the unique point (1)
}

void ps::disk_domain::draw(idomain_draw& draw_interface)
{
	draw_interface.draw(this);
}

ps::vector3d ps::disk_domain::generate_position(float radius, float angle)
{
	// Generate cartesian coordinates - x = sqrt(r) * cos(th), y = sqrt(r) * sin(th)
	vector3d new_pos(radius * cos(angle), radius * sin(angle), 0.0);

	// Calculate the normal and perp vectors
	//vector3d normal(cos(m_phi)*sin(m_theta), sin(m_theta)*sin(m_phi), cos(m_theta));
	vector3d perp(-sin(m_phi), cos(m_phi), 0);
	vector3d ncrossp(cos(m_theta)*cos(m_phi), cos(m_theta)*sin(m_phi), -sin(m_theta));

	// Calculate the final position
	vector3d newpos(radius*cos(angle)*perp + radius*sin(angle)*ncrossp + m_disc_centre);
	return newpos;
}
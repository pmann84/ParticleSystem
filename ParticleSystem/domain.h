#ifndef __DOMAIN_H__
#define __DOMAIN_H__

#define _USE_MATH_DEFINES

#include "vector.h"
#include <math.h>

namespace ps
{
	class domain
	{
	public:
		virtual ~domain() = 0;
		virtual vector3d generate_position() = 0;
		virtual vector3d generate_velocity(vector3d start_position) = 0;
	};

	class point_domain : public domain
	{
	public:
		point_domain(vector3d point, double speed = 0.0);

		vector3d generate_position() override;
		vector3d generate_velocity(vector3d start_position) override;

	private:
		vector3d m_point;
		double m_speed;
	};

	class line_domain : public domain
	{
	public:
		line_domain(vector3d begin, vector3d end, double speed = 0.0);

		vector3d generate_position() override;
		vector3d generate_velocity(vector3d start_position) override;

	private:
		vector3d m_begin;
		vector3d m_end;
		double m_speed;
	};

	class disk_domain : public domain
	{
	public:
		disk_domain(vector3d centre, double inner_radius, double outer_radius, double zenith, double azimuth, double speed = 0.0);

		vector3d generate_position() override;
		vector3d generate_velocity(vector3d start_position) override;

	private:
		vector3d m_disc_centre;
		double m_inner_radius, m_outer_radius;
		double m_theta, m_phi;
		double m_speed;
	};
}

#endif __DOMAIN_H__

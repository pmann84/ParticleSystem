#ifndef __DOMAIN_H__
#define __DOMAIN_H__

#define _USE_MATH_DEFINES

#include "vector.h"
#include <math.h>
#include <memory>

namespace ps
{
	class domain_draw_interface;

	class domain
	{
	public:
		virtual ~domain() = 0;
		virtual vector3d generate_position() = 0;
		virtual vector3d generate_velocity(vector3d start_position) = 0;
		virtual void draw(domain_draw_interface& draw_interface) = 0;
	};

	class point_domain : public domain
	{
	public:
		point_domain(vector3d point, double speed = 0.0);

		vector3d generate_position() override;
		vector3d generate_velocity(vector3d start_position) override;
		void draw(domain_draw_interface& draw_interface) override;

		vector3d point() const { return m_point; }

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
		void draw(domain_draw_interface& draw_interface) override;

		vector3d start_point() const { return m_begin; }
		vector3d end_point() const { return m_end; }

	private:
		vector3d m_begin;
		vector3d m_end;
		double m_speed;
	};

	// Disk Domain - zenith = M_PI/2 and azimuth = M_PI/2, give a disk that is oriented in the x, z plane in openGL.
	class disk_domain : public domain
	{
	public:
		disk_domain(vector3d centre, double inner_radius, double outer_radius, double zenith, double azimuth, double speed = 0.0);

		vector3d generate_position() override;
		vector3d generate_velocity(vector3d start_position) override;
		void draw(domain_draw_interface& draw_interface) override;

		vector3d generate_position(float radius, float angle);
		vector3d centre() const { return m_disc_centre; }
		double inner_radius() const { return m_inner_radius; }
		double outer_radius() const { return m_outer_radius; }

	private:
		vector3d m_disc_centre;
		double m_inner_radius, m_outer_radius;
		double m_theta, m_phi;
		double m_speed;
	};

	class domain_draw_interface
	{
	public:
		virtual ~domain_draw_interface() = 0;
		virtual void draw(point_domain* domain) = 0;
		virtual void draw(line_domain* domain) = 0;
		virtual void draw(disk_domain* domain) = 0;
	};
}

#endif __DOMAIN_H__

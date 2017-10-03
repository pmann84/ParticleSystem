#ifndef __DOMAIN_H__
#define __DOMAIN_H__

#define _USE_MATH_DEFINES

#include "vector.h"
#include <math.h>
#include <memory>
#include "particle.h"
#include "idomain.h"
#include "idomain_draw.h"

namespace ps
{
	class no_domain_intersection_exception : std::exception
	{
		virtual const char* what() const throw() override
		{
			return "Point does not intersect with domain.";
		}
	};

	class point_domain : public idomain
	{
	public:
		point_domain(vector3d point);

		vector3d generate_position() override;
		vector3d generate_velocity(vector3d start_position, double speed) override;
		bool has_entered_domain(particle particle) override;
		vector3d particle_domain_intersection(particle particle) override;
		vector3d normal() override;
		void draw(idomain_draw& draw_interface) override;

		vector3d point() const { return m_point; }

	private:
		vector3d m_point;
	};

	class line_domain : public idomain
	{
	public:
		line_domain(vector3d begin, vector3d end);

		vector3d generate_position() override;
		vector3d generate_velocity(vector3d start_position, double speed) override;
		bool has_entered_domain(particle particle) override;
		vector3d particle_domain_intersection(particle particle) override;
		vector3d normal() override;
		void draw(idomain_draw& draw_interface) override;

		vector3d start_point() const { return m_begin; }
		vector3d end_point() const { return m_end; }

	private:
		vector3d m_begin;
		vector3d m_end;
	};

	// Disk Domain - zenith = M_PI/2 and azimuth = M_PI/2, give a disk that is oriented in the x, z plane in openGL.
	class disk_domain : public idomain
	{
	public:
		disk_domain(vector3d centre, double inner_radius, double outer_radius, double zenith, double azimuth);

		vector3d generate_position() override;
		vector3d generate_velocity(vector3d start_position, double speed) override;
		bool has_entered_domain(particle particle) override;
		vector3d particle_domain_intersection(particle particle) override;
		vector3d normal() override;
		void draw(idomain_draw& draw_interface) override;

		vector3d generate_position(float radius, float angle) const;
		vector3d centre() const { return m_disc_centre; }
		double inner_radius() const { return m_inner_radius; }
		double outer_radius() const { return m_outer_radius; }

	private:
		vector3d m_disc_centre;
		double m_inner_radius, m_outer_radius;
		double m_theta, m_phi;
	};
}

#endif __DOMAIN_H__

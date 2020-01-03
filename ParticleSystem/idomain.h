#ifndef __IDOMAIN_H__
#define __IDOMAIN_H__

namespace ps
{
	class idomain_draw;

	class idomain
	{
	public:
		virtual ~idomain() {}
		virtual vector3d generate_position() = 0;
		virtual vector3d generate_velocity(vector3d start_position, double speed) = 0;
		virtual vector3d particle_domain_intersection(const particle& particle) const = 0;
		virtual bool has_entered_domain(const particle& particle) const = 0;
		virtual vector3d normal() = 0;
		virtual void draw(idomain_draw& draw_interface) = 0;
	};
}

#endif // __IDOMAIN_H__
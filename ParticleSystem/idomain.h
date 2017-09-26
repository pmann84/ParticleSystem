#ifndef __IDOMAIN_H__
#define __IDOMAIN_H__

namespace ps
{
	class idomain_draw;

	class idomain
	{
	public:
		virtual ~idomain();
		virtual vector3d generate_position() = 0;
		virtual vector3d generate_velocity(vector3d start_position) = 0;
		virtual bool has_entered_domain(particle particle) = 0;
		virtual void draw(idomain_draw& draw_interface) = 0;
	};

	inline idomain::~idomain()
	{
	}
}

#endif // __IDOMAIN_H__
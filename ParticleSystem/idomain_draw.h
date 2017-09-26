#ifndef __DOMAIN_DRAW_INTERFACE_H__
#define __DOMAIN_DRAW_INTERFACE_H__

namespace ps 
{
	class point_domain;
	class line_domain;
	class disk_domain;

	class idomain_draw
	{
	public:
		virtual ~idomain_draw() = 0;
		virtual void draw(point_domain* domain) = 0;
		virtual void draw(line_domain* domain) = 0;
		virtual void draw(disk_domain* domain) = 0;
	};

	inline idomain_draw::~idomain_draw()
	{
	}

}

#endif // __DOMAIN_DRAW_INTERFACE_H__

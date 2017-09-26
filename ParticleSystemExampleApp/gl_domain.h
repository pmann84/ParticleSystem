#ifndef __GL_DOMAIN_H__
#define __GL_DOMAIN_H__

#include <GL/freeglut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut_std.h>

#include "../ParticleSystem/domain.h"

class opengl_domain_drawer : public ps::idomain_draw
{
public:
	void draw(ps::point_domain* domain) override;
	void draw(ps::line_domain* domain) override;
	void draw(ps::disk_domain* domain) override;
};

#endif // __GL_DOMAIN_H__
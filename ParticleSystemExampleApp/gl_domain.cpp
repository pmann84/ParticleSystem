#include "gl_domain.h"

void opengl_domain_drawer::draw(ps::point_domain* domain)
{
	ps::vector3d pt = domain->point();
	glBegin(GL_POINTS);
	glVertex3f(pt[0], pt[1], pt[2]);
	glEnd();
}

void opengl_domain_drawer::draw(ps::line_domain* domain)
{
	ps::vector3d start_pt = domain->start_point();
	ps::vector3d end_pt = domain->end_point();
	glBegin(GL_LINES);
	glVertex3f(start_pt[0], start_pt[1], start_pt[2]);
	glVertex3f(end_pt[0], end_pt[1], end_pt[2]);
	glEnd();
}

void opengl_domain_drawer::draw(ps::disk_domain* domain)
{
	int num_segments = 50;
	float angle_delta = 2.0*M_PI / (num_segments-1);
	// Do outer radius
	glBegin(GL_LINES);
	for (int i=0; i<num_segments; ++i)
	{
		// Get point on outer radius
		ps::vector3d start_pos = domain->generate_position(domain->outer_radius(), angle_delta*i);
		ps::vector3d end_pos = domain->generate_position(domain->outer_radius(), angle_delta*(i+1));
		glVertex3f(start_pos[0], start_pos[1], start_pos[2]);
		glVertex3f(end_pos[0], end_pos[1], end_pos[2]);
	}
	// Do outer radius
	for (int i = 0; i<num_segments; ++i)
	{
		// Get point on outer radius
		ps::vector3d start_pos = domain->generate_position(domain->inner_radius(), angle_delta*i);
		ps::vector3d end_pos = domain->generate_position(domain->inner_radius(), angle_delta*(i + 1));
		glVertex3f(start_pos[0], start_pos[1], start_pos[2]);
		glVertex3f(end_pos[0], end_pos[1], end_pos[2]);
	}
	glEnd();
}

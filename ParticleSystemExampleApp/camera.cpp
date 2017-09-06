#include "camera.h"
#include <GL/freeglut_std.h>
#include <cmath>

void camera::update_camera() const
{
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(m_x, 1.0f, m_z,
		m_x + m_lx, 1.0f, m_z + m_lz,
		0.0f, 1.0f, 0.0f);
}

camera::camera()
{
}

void camera::process_special_keys(int key, int xx, int yy)
{
	float fraction = 0.1f;
	switch (key) {
		case GLUT_KEY_LEFT:
			m_angle -= 0.01f;
			m_lx = sin(m_angle);
			m_lz = -cos(m_angle);
			break;
		case GLUT_KEY_RIGHT:
			m_angle += 0.01f;
			m_lx = sin(m_angle);
			m_lz = -cos(m_angle);
			break;
		case GLUT_KEY_UP:
			m_x += m_lx * fraction;
			m_z += m_lz * fraction;
			break;
		case GLUT_KEY_DOWN:
			m_x -= m_lx * fraction;
			m_z -= m_lz * fraction;
			break;
	}
}
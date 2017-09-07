#include "camera.h"
#include <GL/freeglut_std.h>
#include <cmath>

void camera::update_camera()
{
	if (m_delta_move)
		compute_pos(m_delta_move);
	//if (m_delta_angle)
	//	compute_dir(m_delta_angle);

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

void camera::compute_pos(float deltaMove)
{

	m_x += deltaMove * m_lx * 0.1f;
	m_z += deltaMove * m_lz * 0.1f;
}

void camera::compute_dir(float deltaAngle)
{

	m_angle += deltaAngle;
	m_lx = sin(m_angle);
	m_lz = -cos(m_angle);
}

void camera::process_key_press(int key, int xx, int yy)
{

	switch (key) 
	{
		//case GLUT_KEY_LEFT: m_delta_angle = -m_rotate_speed; break;
		//case GLUT_KEY_RIGHT: m_delta_angle = m_rotate_speed; break;
		case GLUT_KEY_UP: m_delta_move = m_move_speed; break;
		case GLUT_KEY_DOWN: m_delta_move = -m_move_speed; break;
	}
}

void camera::process_key_release(int key, int x, int y)
{

	switch (key)
	{
		//case GLUT_KEY_LEFT:
		//case GLUT_KEY_RIGHT: m_delta_angle = 0.0f; break;
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN: m_delta_move = 0; break;
	}
}

void camera::process_mouse_move(int x, int y)
{
	// this will only be true when the left button is down
	if (m_x_origin >= 0) 
	{
		// update deltaAngle
		m_delta_angle = (x - m_x_origin) * 0.001f;

		// update camera's direction
		m_lx = sin(m_angle + m_delta_angle);
		m_lz = -cos(m_angle + m_delta_angle);
	}
}

void camera::process_mouse_button(int button, int state, int x, int y)
{
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) 
	{
		// when the button is released
		if (state == GLUT_UP) 
		{
			m_angle += m_delta_angle;
			m_x_origin = -1;
		}
		else // state = GLUT_DOWN
		{
			m_x_origin = x;
		}
	}
}

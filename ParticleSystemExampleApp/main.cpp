#include <iostream>

#include <GL/freeglut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut_std.h>

#include "../ParticleSystem/particle_group.h"
#include "camera.h"

// Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

// GLUT Forward decl methods
void Initialise(int, char*[]);
void Resize(int, int);
void Render();
void Render(int);
void ProcessSpecialKeys(int, int, int);

// App forward decl
class particle_demo_app;
particle_demo_app *p_app;

// Application class
class particle_demo_app
{
public:
	particle_demo_app();

	bool initialise(int argc, char* args[]);
	void update();
	void resize(int width, int height);
	void render();

	void process_special_keys(int, int, int);

private:
	ps::particle_group p_group;
	camera m_camera;

	void init_window(int argc, char* args[]);
};

bool particle_demo_app::initialise(int argc, char* args[])
{
	init_window(argc, args);
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	return true;
}

void particle_demo_app::init_window(int argc, char* args[])
{
	glutInit(&argc, args);

	//glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (glutCreateWindow("Particle Demo") < 1) 
	{
		std::cout << "ERROR: Could not create a new rendering window." << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(Resize);
	glutDisplayFunc(Render);
	glutSpecialFunc(ProcessSpecialKeys);
}

particle_demo_app::particle_demo_app() : p_group(500, 5)
{
	p_group.time_interval(0.05);
	p_group.particle_life(100, true);
}

void particle_demo_app::update()
{	

	std::shared_ptr<ps::domain> source_domain = std::make_shared<ps::point_domain>(ps::vector3d(0.0f, 0.0f, -5.0f));
	//std::shared_ptr<ps::domain> source_domain = std::make_shared<ps::line_domain>(ps::vector3d(0.0f, 0.0f, -5.0f), ps::vector3d(-0.1f, 0.0f, 0.0f), 1.5);
	//std::shared_ptr<ps::domain> source_domain = std::make_shared<ps::disk_domain>(ps::vector3d(0.0f, 0.0f, -5.0f), 0.0, 0.25, 0.0f, 0.0f);

	std::shared_ptr<ps::domain> velocity_domain = std::make_shared<ps::disk_domain>(ps::vector3d(0.0f, 5.0f, 0.0f), 4.0, 5.0, 0.0f, 0.0f, 1.5);
	//std::shared_ptr<ps::domain> velocity_domain = std::make_shared<ps::line_domain>(ps::vector3d(3.0f, 3.0f, 0.0f), ps::vector3d(5.0f, 3.0f, 0.0f), 1.5);
	//std::shared_ptr<ps::domain> velocity_domain = std::make_shared<ps::point_domain>(ps::vector3d(3.0f, 3.0f, 0.0f));
	ps::vector3d gravity_vector(0.0, -1.0, 0.0);
	ps::vector3d wind_vector(2.0, 0.0, 0.0);
	
	p_group.source(source_domain)
		   .velocity(velocity_domain)
		   .constant_force(gravity_vector + wind_vector)
		   .update();
}

void particle_demo_app::resize(int width, int height)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
				   (double)width / (double)height, //The width-to-height ratio
				   1.0,                //The near z clipping coordinate
				   200.0);              //The far z clipping coordinate
}

void particle_demo_app::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	m_camera.update_camera();
	glBegin(GL_POINTS);
	for(auto& p: p_group.particles())
	{
		if (p.is_alive())
		{
			glVertex3f(p.position()[0], p.position()[1], p.position()[2]);
		}
	}
	glEnd();
	glutSwapBuffers();
}

void particle_demo_app::process_special_keys(int key, int xx, int yy)
{
	m_camera.process_special_keys(key, xx, yy);
}

void Initialise(int argc, char* args[])
{
	p_app->initialise(argc, args);
}

void Resize(int width, int height)
{
	p_app->resize(width, height);
}

void Render()
{
	p_app->update();
	p_app->render();
}

void Render(int val)
{
	Render();
	glutTimerFunc(1000 / SCREEN_FPS, Render, 1);
}

void ProcessSpecialKeys(int key, int xx, int yy) 
{
	p_app->process_special_keys(key, xx, yy);
}

// Application Entry Point
int main(int argc, char* args[])
{
	// Create app class
	p_app = new particle_demo_app();
	// Initialise gl
	Initialise(argc, args);
	// Begin main loop
	glutTimerFunc(1000 / SCREEN_FPS, Render, 0);
	glutMainLoop();
	return 0;
}
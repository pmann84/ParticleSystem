#ifndef __CAMERA_H__
#define __CAMERA_H__

// Camera class
class camera
{
public:
	camera();

	void update_camera();
	void process_key_press(int, int, int);
	void process_key_release(int, int, int);
	void process_mouse_move(int, int);
	void process_mouse_button(int, int, int, int);
	void compute_pos(float delta_move);
	void compute_dir(float delta_angle);

private:
	// angle of rotation for the camera direction
	float m_angle = 0.0;
	// actual vector representing the camera's direction
	float m_lx = 0.0f, m_ly = 0.0f, m_lz = -1.0f;
	// XZ position of the camera
	float m_x = 0.0f, m_y = 0.0f, m_z = 5.0f;
	// the key states. These variables will be zero
	//when no key is being presses
	float m_delta_angle = 0.0f;
	float m_delta_move = 0;
	// camera control speeds
	float m_rotate_speed = 0.1;
	float m_move_speed = 1.0;
	// Origin of camera for mouse movement
	int m_x_origin = -1;
};

#endif // __CAMERA_H__


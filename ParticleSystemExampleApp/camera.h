#ifndef __CAMERA_H__
#define __CAMERA_H__

// Camera class
class camera
{
public:
	camera();

	void update_camera() const;
	void process_special_keys(int, int, int);

private:
	// angle of rotation for the camera direction
	float m_angle = 0.0;
	// actual vector representing the camera's direction
	float m_lx = 0.0f, m_lz = -1.0f;
	// XZ position of the camera
	float m_x = 0.0f, m_z = 5.0f;
};

#endif // __CAMERA_H__


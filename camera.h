#pragma once

#include <qvector3d.h>
#include <qmatrix4x4.h>

class FocusCamera
{
	static constexpr float YAW = -90.0f;
	static constexpr float PITCH = 0.0f;
	static constexpr float SPEED = 2.5f;
	static constexpr float SENSITIVITY = 0.1f;
	static constexpr float ZOOM = 45.0f;

	// Camera Attributes
	QVector3D m_position;
	QVector3D m_front;
	QVector3D m_up;
	QVector3D m_right;
	QVector3D m_worldUp;
	QVector3D m_center;

	// Camera options
	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

public:

	// Constructor with vectors
	FocusCamera(const QVector3D& position = {0.0f, 0.0f, 0.0f}, QVector3D up = {0.0f, 1.0f, 0.0f},
	            const QVector3D& center = {0, 0, 0});

	QVector3D front()const { return m_front; }
	QVector3D right()const { return m_right; }
	QVector3D up()const { return m_up; }

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	QMatrix4x4 view()const
	{
		QMatrix4x4 vi;
		vi.lookAt(m_position, m_position + m_front, m_up);
		return vi;
	}

	QVector3D position()const { return m_position; }

	QVector3D center()const { return m_center; }

	void setPosition(const QVector3D & pos);

	void setUpDirection(const QVector3D & up);

	void setCenter(const QVector3D& center);

	void movement(const QVector3D& direction, float deltaTime);

	void rotation(float xoffset, float yoffset);

	void processMouseScroll(float yoffset);

private:

	void updateCameraVectors(const QVector3D& axis, double theta);
};
#include "camera.h"

FocusCamera::FocusCamera(const QVector3D& position, QVector3D up, const QVector3D& center):
	m_position(position),
	m_front(center - position),
	m_worldUp(up),
	m_movementSpeed(SPEED),
	m_mouseSensitivity(SENSITIVITY),
	m_center(center),
	m_zoom(ZOOM)
{
	m_right = QVector3D::crossProduct(m_front, m_worldUp);
	m_up = QVector3D::crossProduct(m_right, m_front);
	//updateCameraVectors(QVector3D(0,1,0),QVector3D(0,0,0),0);
}

void FocusCamera::setPosition(const QVector3D & pos)
{
	m_position = pos;
	m_front = (m_center - m_position).normalized();
	m_right = QVector3D::crossProduct(m_front, m_worldUp).normalized();
	m_up = QVector3D::crossProduct(m_right, m_front).normalized();
}

void FocusCamera::setUpDirection(const QVector3D & up)
{
	m_worldUp = up;
	m_front = (m_center - m_position).normalized();
	m_right = QVector3D::crossProduct(m_front, m_worldUp).normalized();
	m_up = QVector3D::crossProduct(m_right, m_front).normalized();
}

void FocusCamera::setCenter(const QVector3D& center)
{
	m_center = center;
	m_front = (m_center - m_position).normalized();
	m_right = QVector3D::crossProduct(m_front, m_worldUp).normalized();
	m_up = QVector3D::crossProduct(m_right, m_front).normalized();
}

void FocusCamera::movement(const QVector3D& direction, float deltaTime)
{
	const auto velocity = m_movementSpeed * direction * deltaTime;
	m_position += velocity;
}

void FocusCamera::rotation(float xoffset, float yoffset)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;
	const auto theta = 4.0 * (std::fabs(xoffset) + std::fabs(yoffset));
	const auto v = ((m_right * xoffset) + (m_up * yoffset));
	const auto axis = QVector3D::crossProduct(v, -m_front).normalized();
	updateCameraVectors(axis, theta);
}

void FocusCamera::processMouseScroll(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

void FocusCamera::updateCameraVectors(const QVector3D& axis, double theta)
{
	QMatrix4x4 rotation;
	rotation.rotate(theta, axis);
	QMatrix4x4 translation;
	translation.translate(-m_center.x(), -m_center.y(), -m_center.z());
	m_position = translation.inverted() * (rotation * (translation * m_position));
	m_front = (rotation * m_front.normalized());
	m_up = (rotation * m_up.normalized());
	m_right = QVector3D::crossProduct(m_front, m_up);
	m_up = QVector3D::crossProduct(m_right, m_front);
	m_front.normalize(); // 不返回新的
	m_right.normalize();
	m_up.normalize();
}

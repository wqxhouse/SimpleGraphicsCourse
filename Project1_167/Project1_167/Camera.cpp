#include "Camera.h"


Camera::Camera(Vector3 &e, Vector3 &d, Vector3 &up) :
		m_e ( e['x'],  e['y'],  e['z']),
		m_d ( d['x'],  d['y'],  d['z']),
		m_up(up['x'], up['y'], up['z'])
		{
			Vector3 z = e.subtract(d);
			z.normalize();

			Vector3 x = up.cross(z);
			x.normalize();

			Vector3 y = z .cross(x);

			// before inversion
			/*m_cameraMat.set(0 , x['x']);
			m_cameraMat.set(1 , x['y']);
			m_cameraMat.set(2 , x['z']);
			m_cameraMat.set(3 , 0     );

			m_cameraMat.set(4 , y['x']);
			m_cameraMat.set(5 , y['y']);
			m_cameraMat.set(6 , y['z']);
			m_cameraMat.set(7 , 0     );

			m_cameraMat.set(8 , z['x']);
			m_cameraMat.set(9 , z['y']);
			m_cameraMat.set(10, z['z']);
			m_cameraMat.set(11, 0     );

			m_cameraMat.set(12, e['x']);
			m_cameraMat.set(13, e['y']);
			m_cameraMat.set(14, e['z']);
			m_cameraMat.set(15, 1     );*/

			// Viewport matrix -- Camera matrix inverted 
			m_cameraMat.set(0 , x['x'] );
			m_cameraMat.set(1 , y['x'] );
			m_cameraMat.set(2 , z['x'] );
			//m_cameraMat.set(3 , 0      );

			m_cameraMat.set(4 , x['y'] );
			m_cameraMat.set(5 , y['y'] );
			m_cameraMat.set(6 , z['y'] );
			//m_cameraMat.set(7 , 0      );

			m_cameraMat.set(8 , x['z'] );
			m_cameraMat.set(9 , y['z'] );
			m_cameraMat.set(10, z['z'] );
			//m_cameraMat.set(11, 0      );

			/*m_cameraMat.set(12, -e['x']);
			m_cameraMat.set(13, -e['y']);
			m_cameraMat.set(14, -e['z']);*/
			//m_cameraMat.set(15, 1      );

			Matrix4 transMat;
			transMat.set(12, -e['x']);
			transMat.set(13, -e['y']);
			transMat.set(14, -e['z']);

			m_cameraMat = m_cameraMat.multiplyMat(transMat);

		}


Camera::~Camera()
{
}

void Camera::getCameraMatrix(float cameraMat[])
{
	std::copy((float *)&m_cameraMat, (float *)&m_cameraMat + 16, cameraMat);
}





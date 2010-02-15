#ifndef _NEHE_LESSON_3
#define _NEHE_LESSON_3

#include "basecode/lesson.h"

#include <vector>

class Labirynt : public NeHe::Lesson {
	private:
		struct Vertex {
			float x, y, z;
			Vertex(float x, float y, float z) {
				this->x = x;
				this->y = y;
				this->z = z;
			}
		};

		struct Point {
      double x, y;
      Point(double x = 0, double y = 0) {
        this->x = x;
        this->y = y;
      }
    };

		std::vector<Vertex> m_Vertices;
		std::vector<Vertex> m_Colors;

		float m_RotationAngle;
        float x, z, y;
        int h, m;

		virtual void draw();

		bool helper(Vertex A, Vertex B, Vertex C);
		bool intersect(Vertex A, Vertex B, Vertex C, Vertex D);
		bool intersects(Point p, Point q, Point c, double r);
		bool out(Vertex u, Vertex v);

		GLuint texture[4]; /* Storage For One Texture ( NEW ) */
		int LoadGLTextures();

	public:
		Labirynt();
		virtual bool init();
};

#endif

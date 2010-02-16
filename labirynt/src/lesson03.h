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

		std::vector<Vertex> m_Vertices;
		float m_RotationAngle;
    float x, z, y;
    int h, m;
    GLuint texture[4];
    char T[21][10];

		virtual void draw();

		bool helper(Vertex A, Vertex B, Vertex C);
		bool intersect(Vertex A, Vertex B, Vertex C, Vertex D);
		bool intersects(Vertex p, Vertex q, Vertex c, double r);
		bool out(Vertex u, Vertex v);

		int loadtextures();
		void genmaze();

	public:
		Labirynt();
		virtual bool init();
};

#endif

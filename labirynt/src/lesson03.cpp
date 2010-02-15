#include "lesson03.h"
#include <cmath>
#define PI 3.14159265
#include <fstream>
using namespace std;

Labirynt::Labirynt() {}

int Labirynt::LoadGLTextures() {
    int status = false;
    SDL_Surface *TextureImage[4];

    if ((TextureImage[0] = SDL_LoadBMP("mot096.bmp")) && (TextureImage[1] = SDL_LoadBMP( "mot069.bmp")) &&
        (TextureImage[2] = SDL_LoadBMP("mot099.bmp")) && (TextureImage[3] = SDL_LoadBMP( "mot066.bmp"))) {
	    status = true;

	    glGenTextures(4, &texture[0]);

	    for (int i = 0; i < 4; i++) {
            glBindTexture(GL_TEXTURE_2D, texture[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->w, TextureImage[i]->h, 0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[i]->pixels);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }

    for (int i = 0; i < 4; i++) {
        if (TextureImage[i]) SDL_FreeSurface(TextureImage[i]);
    }

    return status;
}

bool Labirynt::init() {
	if (!m_Window.createWindow(800, 600, 32, false, "Labirynt")) return false;

    if (!LoadGLTextures()) return false;

    glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    m_RotationAngle = 90;
	x = 8;
	y = 0;
	z = -6;
	h = 6;
	m = 6;

	fstream file("labirynt.txt");
	float xx = 0, zz = 0, xxx, zzz;
	float r1 = 0.25, r2 = 0.17, r = 0.0;
    int i = 0;
    char c;
	while (file >> c) {
	    if (c == '#') {
	        if (i % 2 == 0) {
	            m_Vertices.push_back(Vertex(xx, -1, zz));
                m_Colors.push_back(Vertex(r1, 0, 0));
                m_Vertices.push_back(Vertex(xx + h, -1, zz));
                m_Colors.push_back(Vertex(r1, 0, 0));
                m_Vertices.push_back(Vertex(xx + h, 1, zz));
                m_Colors.push_back(Vertex(r1, 0, 0));
                m_Vertices.push_back(Vertex(xx, 1, zz));
                m_Colors.push_back(Vertex(r1, 0, 0));

//                m_Vertices.push_back(Vertex(xx, -1, zz + r));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + h, -1, zz + r));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + h, 1, zz + r));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx, 1, zz + r));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//
//                m_Vertices.push_back(Vertex(xx, -1, zz));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx, -1, zz + r));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx, 1, zz + r));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx, 1, zz));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//
//                m_Vertices.push_back(Vertex(xx + h, -1, zz));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + h, -1, zz + r));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + h, 1, zz + r));
//                m_Colors.push_back(Vertex(r1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + h, 1, zz));
//                m_Colors.push_back(Vertex(r1, 0, 0));
	        }
	        else {
	            m_Vertices.push_back(Vertex(xx, -1, zz));
                m_Colors.push_back(Vertex(r2, 0, 0));
                m_Vertices.push_back(Vertex(xx, -1, zz + h + r));
                m_Colors.push_back(Vertex(r2 + 0.1, 0, 0));
                m_Vertices.push_back(Vertex(xx, 1, zz + h + r));
                m_Colors.push_back(Vertex(r2 + 0.1, 0, 0));
                m_Vertices.push_back(Vertex(xx, 1, zz));
                m_Colors.push_back(Vertex(r2, 0, 0));

//                m_Vertices.push_back(Vertex(xx + r, -1, zz));
//                m_Colors.push_back(Vertex(r2, 0, 0));
//                m_Vertices.push_back(Vertex(xx + r, -1, zz + h + r));
//                m_Colors.push_back(Vertex(r2 + 0.1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + r, 1, zz + h + r));
//                m_Colors.push_back(Vertex(r2 + 0.1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + r, 1, zz));
//                m_Colors.push_back(Vertex(r2, 0, 0));
//
//                m_Vertices.push_back(Vertex(xx, -1, zz));
//                m_Colors.push_back(Vertex(r2, 0, 0));
//                m_Vertices.push_back(Vertex(xx + r, -1, zz));
//                m_Colors.push_back(Vertex(r2, 0, 0));
//                m_Vertices.push_back(Vertex(xx + r, 1, zz));
//                m_Colors.push_back(Vertex(r2, 0, 0));
//                m_Vertices.push_back(Vertex(xx, 1, zz));
//                m_Colors.push_back(Vertex(r2, 0, 0));
//
//                m_Vertices.push_back(Vertex(xx, -1, zz + h + r));
//                m_Colors.push_back(Vertex(r2 + 0.1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + r, -1, zz + h + r));
//                m_Colors.push_back(Vertex(r2 + 0.1, 0, 0));
//                m_Vertices.push_back(Vertex(xx + r, 1, zz + h + r));
//                m_Colors.push_back(Vertex(r2 + 0.1, 0, 0));
//                m_Vertices.push_back(Vertex(xx, 1, zz + h + r));
//                m_Colors.push_back(Vertex(r2 + 0.1, 0, 0));
	        }
	        xx += h;
	    }
	    else if (c == '.') {
	        xxx = xx;
	        xx = 0;
	        if (i % 2 == 1) zz += h;
	        zzz = zz;
	        i++;
	        r1 += 0.05;
	        r2 += 0.05;
	    }
	    else xx += h;
	}
	file.close();

//	m_Vertices.push_back(Vertex(0, -1, 0));
//    m_Colors.push_back(Vertex(0.1, 0, 0));
//    m_Vertices.push_back(Vertex(xxx + r, -1, 0));
//    m_Colors.push_back(Vertex(0.1, 0, 0));
//    m_Vertices.push_back(Vertex(xxx + r, -1, zzz));
//    m_Colors.push_back(Vertex(0.1, 0, 0));
//    m_Vertices.push_back(Vertex(0, -1, zzz));
//    m_Colors.push_back(Vertex(0.1, 0, 0));
//
//    m_Vertices.push_back(Vertex(0, 1, 0));
//    m_Colors.push_back(Vertex(0.2, 0, 0));
//    m_Vertices.push_back(Vertex(xxx + r, 1, 0));
//    m_Colors.push_back(Vertex(0.2, 0, 0));
//    m_Vertices.push_back(Vertex(xxx + r, 1, zzz));
//    m_Colors.push_back(Vertex(0.2, 0, 0));
//    m_Vertices.push_back(Vertex(0, 1, zzz));
//    m_Colors.push_back(Vertex(0.2, 0, 0));

	return true;
}

bool Labirynt::helper(Vertex A, Vertex B, Vertex C) {
    return (C.z-A.z)*(B.x-A.x) > (B.z-A.z)*(C.x-A.x);
}

bool Labirynt::intersect(Vertex A, Vertex B, Vertex C, Vertex D) {
    return helper(A,C,D) != helper(B,C,D) && helper(A,B,C) != helper(A,B,D);
}

bool Labirynt::intersects(Point p, Point q, Point c, double r) {
	double dx, dy, t, rt;

	dx = q.x - p.x;
	dy = q.y - p.y;
	t = -((p.x - c.x)*dx + (p.y - c.y)*dy) / ((dx*dx) + (dy*dy));

	if(t < 0.0) {
		t = 0.0;
	} else if(t > 1.0) {
		t = 1.0;
	}

	dx = (p.x + t*(q.x - p.x)) - c.x;
	dy = (p.y + t*(q.y - p.y)) - c.y;
	rt = (dx*dx) + (dy*dy);

	if(rt < (r*r))
		return true;
	return false;
}

bool Labirynt::out(Vertex u, Vertex v) {
    int n = (m_Vertices.size() + 1) / 4;
    for (int i = 0; i < n; i++) {
        Vertex v1 = m_Vertices[4*i], v2 = m_Vertices[4*i+1];
//        if (intersect(u, v, v1, v2)) return true;
        if (intersects(Point(v1.x, v1.z), Point(v2.x, v2.z), Point(v.x, v.z), 0.6)) return true;
    }
    return false;
}

void Labirynt::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(m_RotationAngle, 0.0, 1.0, 0.0);

	glTranslatef(x, y, z);

    int n = (m_Vertices.size() - 1) / 4;
    for (int i = 0; i < n; i++) {
        Vertex v1 = m_Vertices[4*i],
            v2 = m_Vertices[4*i+1],
            v3 = m_Vertices[4*i+2],
            v4 = m_Vertices[4*i+3];
        bool p = v1.z == v2.z && v2.z == v3.z && v3.z == v4.z;

        if (p) {
            for (int j = 0; j < h; j += 2) {
                if (j == 2) glBindTexture( GL_TEXTURE_2D, texture[0] );
                else glBindTexture( GL_TEXTURE_2D, texture[1] );

                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(v1.x + j, v1.y, v1.z);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(v1.x + (j + 2), v2.y, v2.z);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(v1.x + (j + 2), v3.y, v3.z);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(v1.x + j, v4.y, v4.z);
                glEnd( );
            }
        } else {
            for (int j = 0; j < h; j += 2) {
                if (j == 2) glBindTexture( GL_TEXTURE_2D, texture[0] );
                else glBindTexture( GL_TEXTURE_2D, texture[1] );

                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(v1.x, v1.y, v1.z + j);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(v2.x, v2.y, v1.z + (j + 2));
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(v3.x, v3.y, v1.z + (j + 2));
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(v4.x, v4.y, v1.z + j);
                glEnd();
            }
        }
    }

    glBindTexture( GL_TEXTURE_2D, texture[2] );
    for (int i = 0; i < m * h; i += 2) {
        for (int j = 0; j < m * h; j += 2) {
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(i, -1, j);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(i + 2, -1, j);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(i + 2, -1, j + 2);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(i, -1, j + 2);
            glEnd();
        }
    }

    glBindTexture( GL_TEXTURE_2D, texture[3] );
    for (int i = 0; i < m * h; i += 2) {
        for (int j = 0; j < m * h; j += 2) {
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(i, 1, j);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(i + 2, 1, j);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(i + 2, 1, j + 2);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(i, 1, j + 2);
            glEnd();
        }
    }


    if (m_Keys[SDLK_DOWN]) {
        double xx = x + sin(m_RotationAngle /180*PI)*2.0;
        double zz = z - cos(m_RotationAngle /180*PI)*2.0;
        if (!out(Vertex(-x, 0, -z), Vertex(-xx, 0, -zz))) {
            x += sin(m_RotationAngle /180*PI)*0.2;
            z -= cos(m_RotationAngle /180*PI)*0.2;
        } else
        {
            if (!out(Vertex(-x, 0, -z), Vertex(-xx, 0, -z))) x += sin(m_RotationAngle /180*PI)*0.2;
            else if (!out(Vertex(-x, 0, -z), Vertex(-x, 0, -zz))) z -= cos(m_RotationAngle /180*PI)*0.2;
        }
    }
    else if (m_Keys[SDLK_UP]) {
        double xx = x - sin(m_RotationAngle /180*PI)*2.0;
        double zz = z + cos(m_RotationAngle /180*PI)*2.0;
        if (!out(Vertex(-x, 0, -z), Vertex(-xx, 0, -zz))) {
            x -= sin(m_RotationAngle /180*PI)*0.2;
            z += cos(m_RotationAngle /180*PI)*0.2;
        }
        else {
            if (!out(Vertex(-x, 0, -z), Vertex(-xx, 0, -z))) x -= sin(m_RotationAngle /180*PI)*0.2;
            else if (!out(Vertex(-x, 0, -z), Vertex(-x, 0, -zz))) z += cos(m_RotationAngle /180*PI)*0.2;
        }
    }

	if (m_Keys[SDLK_a]) y -= 0.16;
	if (y < 0) y += 0.06;
	if (m_Keys[SDLK_z]) y += 0.16;
	if (y > 0) y -= 0.06;

	if (m_Keys[SDLK_LEFT]) m_RotationAngle -= 1.8;
	if (m_Keys[SDLK_RIGHT]) m_RotationAngle += 1.8;

	if(m_RotationAngle > 360.0) m_RotationAngle -= 360.0;

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	//glVertexPointer(3, GL_FLOAT, 0, &m_Vertices[0]);
	//glColorPointer(3, GL_FLOAT, 0, &m_Colors[0]);

	//glDrawArrays(GL_QUADS, 0, m_Vertices.size());

	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);
}

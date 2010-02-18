#include "lesson03.h"

#include <cmath>
#include <fstream>

#define PI 3.14159265

using namespace std;


Labirynt::Labirynt() {}


int Labirynt::loadtextures() {
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

  for (int i = 0; i < 4; i++)
    if (TextureImage[i]) SDL_FreeSurface(TextureImage[i]);

  return status;
}


void Labirynt::genmaze() {
  int H, C,	E;
  int L[10],R[10];
  int i, j;

  for (E = 10, i = 0; --E; L[E] = R[E] = E, i ++) { /* close top of maze */
    T[0][i] = '_';
  }

  H = 10;
  L[0] = 1;

	j = 1;
  T[j][0] = '|';

  while (--H) {
    for (C = 10, i = 0; --C; i++)	{
      if (C != (E=L[C-1]) && 6<<27 < rand()) { /* make right-connection ? */
        R[E] = R[C]; /* link E */
        L[R[C]] = E; /* to R[C] */
        R[C] = C-1;	/* link C */
        L[C-1] = C;	/* to C-1 */

        T[j][i + 1] = '.';
      }
      else { /* no wall to the right */
        T[j][i + 1] = '|';
      }

      if (C != (E=L[C]) && 6<<27 < rand()) {	/* omit down-connection ? */
        R[E] = R[C]; /* link E */
        L[R[C]] = E; /* to R[C] */
        L[C] = C;	/* link C */
        R[C] = C;	/* to C */

        T[j + 1][i] = '_';
      }
      else { /* no wall downward */
        T[j + 1][i] = ' ';
      }
    }

    j += 2;
    T[j][0] = '|';
  }

  for (C = 10, i = 0; --C; i++) { /* close bottom of maze */
    if (C != (E=L[C-1]) && (C == R[C] || 6<<27 < rand())) {
      L[R[E]=R[C]]=E;
      L[R[C]=C-1]=C;

      T[j][i + 1] = '.';
    }
    else {
      T[j][i + 1] = '|';
    }

    E = L[C];
    R[E] = R[C];
    L[R[C]] = E;
    L[C] = C;
    R[C] = C;

    T[j + 1][i] = '_';
  }

  for (i = 0; i < 9; i++) printf("._");
  printf("\n");
  for (j = 1; j < 21; j += 2) {
    for (i = 0; i < 10; i++) {
      printf("%c", T[j][i]);
      printf("%c", T[j + 1][i]);
    }
    printf("\n");
  }
}


bool Labirynt::init() {
  srand(time(0));

	if (!m_Window.createWindow(800, 600, 32, false, "Labirynt")) return false;

  if (!loadtextures()) return false;
  genmaze();

  glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  m_RotationAngle = 135;
  x = -3;
  y = 0;
  z = -3;
  h = 6;
  m = 10;

  float xx = 0, zz = 0;
  for (int j = 0; j < 20; j++) {
    if (j % 2 == 0) {
      for (int i = 0; i < 9; i++) {
        if (T[j][i] == '_') {
          m_Vertices.push_back(Vertex(xx, -1, zz));
          m_Vertices.push_back(Vertex(xx + h, -1, zz));
          m_Vertices.push_back(Vertex(xx + h, 1, zz));
          m_Vertices.push_back(Vertex(xx, 1, zz));
        }
        xx += h;
      }
      xx = 0;
    }
    else {
      for (int i = 0; i < 10; i++) {
        if (T[j][i] == '|') {
          m_Vertices.push_back(Vertex(xx, -1, zz));
          m_Vertices.push_back(Vertex(xx, -1, zz + h));
          m_Vertices.push_back(Vertex(xx, 1, zz + h));
          m_Vertices.push_back(Vertex(xx, 1, zz));
        }
        xx += h;
      }
      xx = 0;
      zz += h;
    }
  }

//  m_RotationAngle = 90;
//	x = 9;
//	y = 0;
//	z = -9;
//	h = 6;
//	m = 10;
//
//	string name;
//
//  switch (rand() % 3) {
//    case 0 : name = "maze1.txt";
//      break;
//    case 1 : name = "maze2.txt";
//      break;
//    case 2 : name = "maze3.txt";
//      break;
//  }
//
//	fstream file(name.c_str());
//
//	float xx = 0, zz = 0;
//	int i = 0;
//	char c, d, e, f;
//	while (file >> c) {
//	  if (i % 2 == 0) {
//	    if (c == '-') {
//	      file >> d >> e;
//	      m_Vertices.push_back(Vertex(xx, -1, zz));
//        m_Vertices.push_back(Vertex(xx + h, -1, zz));
//        m_Vertices.push_back(Vertex(xx + h, 1, zz));
//        m_Vertices.push_back(Vertex(xx, 1, zz));
//        xx += h;
//	    } else if (c == '.') {
//	      file >> d >> e;
//	      xx += h;
//	    } else if (c == ';') {
//	      i++;
//	      xx = 0;
//	    }
//	  } else {
//	    if (c == '|') {
//	      m_Vertices.push_back(Vertex(xx, -1, zz));
//        m_Vertices.push_back(Vertex(xx, -1, zz + h));
//        m_Vertices.push_back(Vertex(xx, 1, zz + h));
//        m_Vertices.push_back(Vertex(xx, 1, zz));
//	      file >> d;
//	      if (d == ';') {
//	        i++;
//          xx = 0;
//          zz += h;
//	      } else {
//          xx += h;
//          file >> e >> f;
//	      }
//	    } else if (c == '.') {
//	      file >> d;
//	      if (d == ';') {
//	        i++;
//          xx = 0;
//          zz += h;
//	      } else {
//          xx += h;
//          file >> e >> f;
//	      }
//	    }
//	  }
//	}
//
//	file.close();

	return true;
}


bool Labirynt::helper(Vertex A, Vertex B, Vertex C) {
    return (C.z-A.z)*(B.x-A.x) > (B.z-A.z)*(C.x-A.x);
}


bool Labirynt::intersect(Vertex A, Vertex B, Vertex C, Vertex D) {
    return helper(A,C,D) != helper(B,C,D) && helper(A,B,C) != helper(A,B,D);
}


bool Labirynt::intersects(Vertex p, Vertex q, Vertex c, double r) {
	double dx, dy, t, rt;

	dx = q.x - p.x;
	dy = q.z - p.z;
	t = -((p.x - c.x)*dx + (p.z - c.z)*dy) / ((dx*dx) + (dy*dy));

	if(t < 0.0) {
		t = 0.0;
	} else if(t > 1.0) {
		t = 1.0;
	}

	dx = (p.x + t*(q.x - p.x)) - c.x;
	dy = (p.z + t*(q.z - p.z)) - c.z;
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
        if (intersects(Vertex(v1.x, 0, v1.z), Vertex(v2.x, 0, v2.z), Vertex(v.x, 0, v.z), 0.6)) return true;
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

	if (m_Keys[SDLK_LEFT]) m_RotationAngle -= 2.0;
	if (m_Keys[SDLK_RIGHT]) m_RotationAngle += 2.0;

	if(m_RotationAngle > 360.0) m_RotationAngle -= 360.0;
}

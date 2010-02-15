#include "lesson.h"

namespace NeHe{
	Lesson::Lesson()
	{
	}

	Lesson::~Lesson()
	{
	}

	void Lesson::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
	}

	void Lesson::resize(int x, int y)
	{
		std::cout << "Resizing Window to " << x << "x" << y << std::endl;

		if (y <= 0)
		{
			y = 1;
		}

		glViewport(0,0,x,y);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f,(GLfloat)x/(GLfloat)y,1.0f,100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	bool Lesson::processEvents()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))//get all events
		{
			switch (event.type)
			{
				// Quit event
				case SDL_QUIT:
				{
					// Return false because we are quitting.
					return false;
				}

				case SDL_KEYDOWN:
				{
					SDLKey sym = event.key.keysym.sym;

					if(sym == SDLK_ESCAPE) //Quit if escape was pressed
					{
						return false;
					}

					m_Keys[sym] = 1;
					break;
				}

				case SDL_KEYUP:
				{
					SDLKey sym = event.key.keysym.sym;
					m_Keys[sym] = 0;
					break;
				}

				case SDL_VIDEORESIZE:
				{
					//the window has been resized so we need to set up our viewport and projection according to the new size
					resize(event.resize.w, event.resize.h);
					break;
				}
				// Default case
				default:
				{
					break;
				}
			}
		}

		return true;
	}

	bool Lesson::init()
	{
		if(!m_Window.createWindow(800, 600, 32, false, "Ne(w)He Lesson"))
		{
			return false;
		}

		glClearColor(0.0, 0.0, 0.0, 1.0);

		return true;
	}

	void Lesson::run()
	{
		while(processEvents())
		{
			draw();
			SDL_GL_SwapBuffers();
		}
	}
};

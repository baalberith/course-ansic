#ifndef _NEHE_LESSON
#define _NEHE_LESSON

//we need to include window first because GLee needs to be included before GL.h
#include "window.h"

#include <SDL/SDL.h>

namespace NeHe 
{
	class Lesson 
	{
		protected:
			Window m_Window; ///< The window for this lesson
			unsigned int m_Keys[SDLK_LAST]; ///< Stores keys that are pressed
		
			virtual void draw();
			virtual void resize(int x, int y);
			virtual bool processEvents();

		public:
			Lesson();
			virtual ~Lesson();
						
			virtual bool init();
			virtual void run();
	};
};

#endif


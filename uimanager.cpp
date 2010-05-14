#include "uimanager.h"
#include "util.h"

namespace ufo
{
	bool UIManager::processEvent(SDL_Event& e)
	{
		for (Uint32 i = 0; i < m_elements.size(); ++i)
		{
			if (e.type == SDL_MOUSEMOTION)
			{
				if (SDL_PtInRect(m_elements[i].get(), e.motion.x, e.motion.y) && m_elements[i]->processEvent(e))
					break;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				if (SDL_PtInRect(m_elements[i].get(), e.button.x, e.button.y) && m_elements[i]->processEvent(e))
					break;
			}
			else if (m_elements[i]->processEvent(e))
				break;
		}

		return true;
	}

	void UIManager::draw(SDL_Surface* surface)
	{
		for (Uint32 i = 0; i < m_elements.size(); ++i)
			m_elements[i]->draw(surface);
	}
}
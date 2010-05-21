#include "uimanager.h"
#include "util.h"

namespace ufo
{
	UIManager::UIManager()
		: m_focus(0)
	{
	}

	UIManager::~UIManager()
	{
		for (size_t i = 0; i < size(); ++i)
			delete at(i);
	}

	void UIManager::add(UIElement* e)
	{
		push_back(e);
		e->setUIManager(this);
		e->onCreate();
	}

	bool UIManager::requestFocus(UIElement* e)
	{
		if (m_focus && m_focus->onBlur())
			return false;

		m_focus = e;
		m_focus->onFocus();

		return true;
	}

	bool UIManager::processEvent(SDL_Event& e)
	{
		for (size_t i = 0; i < size(); ++i)
		{
			size_t j = size() - i - 1;
			if (e.type == SDL_MOUSEMOTION)
			{
				if (at(j)->contains(e.motion.x, e.motion.y) && at(j)->onMouseHover(e.motion.x, e.motion.y))
					break;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
			{
				if (at(j)->contains(e.button.x, e.button.y) && at(j)->onMouseLeftClick(e.button.x, e.button.y))
					break;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT)
			{
				if (at(j)->contains(e.button.x, e.button.y) && at(j)->onMouseRightClick(e.button.x, e.button.y))
					break;
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
			{
				if (at(j)->contains(e.button.x, e.button.y) && at(j)->onMouseLeftUnclick(e.button.x, e.button.y))
					break;
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
			{
				if (at(j)->contains(e.button.x, e.button.y) && at(j)->onMouseRightUnclick(e.button.x, e.button.y))
					break;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (m_focus && m_focus->onKeyDown(e.key.keysym))
					break;
				else if (at(j)->onKeyDown(e.key.keysym))
					break;
			}
			else if (e.type == SDL_KEYUP)
			{
				if (m_focus && m_focus->onKeyUp(e.key.keysym))
					break;
				else if (at(j)->onKeyUp(e.key.keysym))
					break;
			}
		}

		return true;
	}

	void UIManager::draw(Surface& surface)
	{
		for (size_t i = 0; i < size(); ++i)
			at(i)->draw(surface);
	}
}
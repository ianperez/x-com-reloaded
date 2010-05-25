#include "uimanager.h"
#include "util.h"
#include <algorithm>

using namespace std;

namespace ufo
{
	UIManager::UIManager(Surface& s)
		: m_focus(0), surface(s)
	{
	}

	UIManager::~UIManager()
	{
		for (list<UIElement*>::iterator i = begin(); i != end(); ++i)
			delete *i;
	}

	void UIManager::create(UIElement* e)
	{
		push_front(e);
		e->m_ui = this;
		e->onCreate();
	}

	void UIManager::destroy(UIElement* e)
	{
		if (find(m_toDestroy.begin(), m_toDestroy.end(), e) != m_toDestroy.end())
			return;

		list<UIElement*>::iterator i = find(begin(), end(), e);
		if (i != end())
		{
			// destroy child elements first
			for (size_t i = 0; i < e->m_elements.size(); ++i)
				destroy(e->m_elements[i]);

			e->onDestroy();
			m_toDestroy.push_back(e);
		}
	}

	void UIManager::cleanup()
	{
		// remove any destroyed elements
		for (size_t i = 0; i < m_toDestroy.size(); ++i)
		{
			list<UIElement*>::iterator j = find(begin(), end(), m_toDestroy[i]);
			if (j != end())
			{
				delete *j;
				erase(j);
			}
		}

		m_toDestroy.clear();
	}

	bool UIManager::requestFocus(UIElement* e)
	{
		if (m_focus && m_focus->onBlur())
			return false;

		m_focus = e;
		m_focus->onFocus();

		return true;
	}

	bool UIManager::releaseFocus(UIElement* e)
	{
		if (m_focus == e)
		{
			m_focus = NULL;
			return true;
		}

		return false;
	}

	bool UIManager::processEvent(SDL_Event& e, UIElement* i)
	{
		if (!i)
			return false;

		if (e.type == SDL_MOUSEMOTION)
		{
			if ((i == m_focus || i->contains(e.motion.x, e.motion.y)) && i->onMouseMove(e.motion.x, e.motion.y))
				return true;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
		{
			if ((i == m_focus || i->contains(e.button.x, e.button.y)) && i->onMouseLeftClick(e.button.x, e.button.y))
				return true;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT)
		{
			if ((i == m_focus || i->contains(e.button.x, e.button.y)) && i->onMouseRightClick(e.button.x, e.button.y))
				return true;
		}
		else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
		{
			if ((i == m_focus || i->contains(e.button.x, e.button.y)) && i->onMouseLeftUnclick(e.button.x, e.button.y))
				return true;
		}
		else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
		{
			if ((i == m_focus || i->contains(e.button.x, e.button.y)) && i->onMouseRightUnclick(e.button.x, e.button.y))
				return true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (i->onKeyDown(e.key.keysym))
				return true;
		}
		else if (e.type == SDL_KEYUP)
		{
			if (i->onKeyUp(e.key.keysym))
				return true;
		}

		return i->m_exclusive;
	}

	bool UIManager::processEvent(SDL_Event& e)
	{
		// first give the focused element this event
		if (!processEvent(e, m_focus))
		{
			// if not processed, give all elements a chance to process it
			for (list<UIElement*>::iterator i = begin(); i != end(); ++i)
			{
				if (processEvent(e, *i))
					break;
			}
		}

		cleanup();

		return true;
	}

	void UIManager::updateTime(UIElement* e)
	{
		e->m_timeElapsed = SDL_GetTicks() - e->m_lastUpdate;
		e->m_lastUpdate = SDL_GetTicks();
	}

	void UIManager::draw()
	{
		for (list<UIElement*>::reverse_iterator i = rbegin(); i != rend(); ++i)
		{
			updateTime(*i);
			(*i)->draw(surface);
		}

		cleanup();
	}
}
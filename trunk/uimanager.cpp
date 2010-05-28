#include "uimanager.h"
#include "util.h"
#include <algorithm>

namespace ufo
{
	UIManager::UIManager(Surface& _surface, StringTable& _strings)
		: m_focus(0), surface(_surface), text(_strings), strings(_strings), m_debug(false)
	{
	}

	UIManager::~UIManager()
	{
		for (list<UIElement*>::iterator i = begin(); i != end(); ++i)
			delete *i;
	}

	void UIManager::create(UIElement* e)
	{
		if (e->m_alwaysOnTop)
			push_front(e);
		else
		{
			bool inserted = false;
			for (list<UIElement*>::iterator i = begin(); i != end(); ++i)
			{
				if (!(*i)->m_alwaysOnTop)
				{
					insert(i, e);
					inserted = true;
					break;
				}
			}

			if (!inserted)
				insert(end(), e);
		}

		e->m_ui = this;
		e->onCreate();
	}

	void UIManager::destroy(UIElement* e, bool cascade)
	{
		if (find(m_toDestroy.begin(), m_toDestroy.end(), e) != m_toDestroy.end())
			return;

		list<UIElement*>::iterator i = find(begin(), end(), e);
		for (; i != end(); ++i)
		{
			// destroy child elements first
			for (size_t j = 0; j < (*i)->m_elements.size(); ++j)
				destroy((*i)->m_elements[j]);

			(*i)->onDestroy();
			m_toDestroy.push_back(*i);

			if (!cascade)
				break;
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

	bool UIManager::processEvent(SDL_Event& evt, UIElement* e)
	{
		if (!e)
			return false;

		if (evt.type == SDL_MOUSEMOTION)
		{
			if ((e == m_focus || e->contains(evt.motion.x, evt.motion.y)) && e->onMouseMove(evt.motion.x, evt.motion.y))
				return true;
		}
		else if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT)
		{
			if ((e == m_focus || e->contains(evt.button.x, evt.button.y)) && e->onMouseLeftClick(evt.button.x, evt.button.y))
				return true;
		}
		else if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_RIGHT)
		{
			if ((e == m_focus || e->contains(evt.button.x, evt.button.y)) && e->onMouseRightClick(evt.button.x, evt.button.y))
				return true;
		}
		else if (evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_LEFT)
		{
			if ((e == m_focus || e->contains(evt.button.x, evt.button.y)) && e->onMouseLeftUnclick(evt.button.x, evt.button.y))
				return true;
		}
		else if (evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_RIGHT)
		{
			if ((e == m_focus || e->contains(evt.button.x, evt.button.y)) && e->onMouseRightUnclick(evt.button.x, evt.button.y))
				return true;
		}
		else if (evt.type == SDL_KEYDOWN)
		{
			if (e->onKeyDown(evt.key.keysym))
				return true;
		}
		else if (evt.type == SDL_KEYUP)
		{
			if (e->onKeyUp(evt.key.keysym))
				return true;
		}

		return e->m_exclusive;
	}

	bool UIManager::processEvent(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11)
			m_debug = !m_debug;

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

	void UIManager::logic()
	{
		for (list<UIElement*>::iterator i = begin(); i != end(); ++i)
		{
			updateTime(*i);
			(*i)->logic();

			if ((*i)->m_exclusive)
				break;
		}

		cleanup();
	}

	void UIManager::draw()
	{
		for (list<UIElement*>::reverse_iterator i = rbegin(); i != rend(); ++i)
			(*i)->draw(surface);

		cleanup();

		if (m_debug)
		{
			size_t j = 0;
			for (list<UIElement*>::iterator i = begin(); i != end(); ++i, ++j)
				m_font.print(surface, 10, 10 + j * 10, format("element %d (exclusive = %s, alwaysOnTop = %s)", j, (*i)->m_exclusive ? "true" : "false", (*i)->m_alwaysOnTop ? "true" : "false"));
		}
	}
}
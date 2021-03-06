#ifndef __UI_WIDGET_H__
#define __UI_WIDGET_H__

#include "../../Source/Interface.h"
#include "../../Source/E3DDelegate.h"

namespace E3DEngine
{
	namespace UI
	{
		class Widget : public Ref, public IActor
		{
		public:
			Widget(float width, float height);

		public:
			Event::EventDelegates Click;
			Event::EventDelegates Touch;
			Event::EventDelegates Drag;

		public:
			virtual void setWidth(float width);
			virtual void setHeight(float height);
			virtual void setPosition(float x, float y);

		private: 
			float m_fWidth;
			float m_fHeight;
			vec3f m_vecPosition;
		};
	}
}

#endif

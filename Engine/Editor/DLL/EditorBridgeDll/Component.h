#pragma once
#include <Component/E3DComponent.hpp>

#pragma managed

namespace E3DEngine
{
	public ref class ComponentFieldType
	{
	public:
		static int FT_INT		= 1 << 0;
		static int FT_FLOAT		= 1 << 1;
		static int FT_STRING	= 1 << 2;
		static int FT_BOOLEAN	= 1 << 3;
		static int FT_OBJECT	= 1 << 4;
		static int FT_COLOR		= 1 << 5;
		static int FT_VECTOR3	= 1 << 6;
		static int 	FT_VECTOR2	= 1 << 7;

		static int FT_READONLY = 1 << 31;
	};

	ref class ComponentRef;

	public ref class ComponentField
	{
	public:
		System::String ^ Name;
		System::String ^ Value;
		int				 Type;
		ComponentRef   ^ Component;
		void SetValue(System::String ^value);
	};

	public ref class ComponentRef
	{
	public:
		ComponentRef(Component *component);
		void SetValue(Component *component);
		System::Collections::Generic::List<ComponentField^>^ GetFields();
		System::String^ GetName();
		System::String^ GetFullName();
		Component *GetComponentPtr();
		void SetFielValue(System::String ^name, System::String ^value);
		bool GetIsActive();
		void SetActive(bool active);

	private:
		Component * mComponent;
		System::Collections::Generic::List<ComponentField^>^ mComponentFieldMap;
		System::String^ mName;
		System::String^ mFullName;
	};
}
#pragma unmanaged
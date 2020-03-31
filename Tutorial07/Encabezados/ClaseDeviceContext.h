#pragma once
#include "Defines.h"

struct DeviceContextDescriptor {

};

///
/// Class with the Device Context abstraction
///
class ClaseDeviceContext {

	public:

		///
		/// Main members of the class
		///
		DeviceContextDescriptor m_DescDCont;

#ifdef D3D11
		ID3D11DeviceContext* g_pImmediateContextD3D11;
#endif // D3D11

		///
		/// Builder and destroyer
		///
		ClaseDeviceContext() {};
		~ClaseDeviceContext() {};

		///
		/// Main methods of the class
		///
#ifdef D3D11
		void
		Init(ID3D11DeviceContext* _context);
#endif
		//
		void
		Update();
		//
		void
		Render();
		//
		void
		Delete();

		void*
		GetDevCont();
		

};
#pragma once
#include <vector>
#include "Mesh.h"
#include "ClaseDeviceContext.h"
#include "ClaseBuffer.h"

///
/// Scene Controller Class
///
class SCENEMANAGER
{
	public:

		///
		/// Builder and destroyer
		///
		SCENEMANAGER();
		~SCENEMANAGER();

		///
		/// Main methods for the class
		///
		void 
		AddMesh(MESH* newMesh);
		
		MESH*& 
		GetMesh(int meshID);
		
		void 
		Update();
		
		void 
		Render(ClaseDeviceContext *_deviceCon, ClaseBuffer* _buff, ClaseDevice* _dev);
		
		void 
		Shutdown();

		void
		Init();
		
		///
		/// Variables
		///
		ClaseBuffer* meshData;

#ifdef OPENGL
		GLuint m_VertexArrayID;
#endif // OPENGL

		bool m_initialize = false;
		std::vector<MESH*> m_MeshInScene;

	private:
		
		///
		/// Variables
		///
		int m_ID = 0;
};
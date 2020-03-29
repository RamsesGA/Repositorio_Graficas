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

		SCENEMANAGER();
		~SCENEMANAGER();

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
		GLuint m_VertexArrayID;
		bool m_initialize = false;
		std::vector<MESH*> m_MeshInScene;

	private:
		
		///
		/// Variables
		///
		int m_ID = 0;
};
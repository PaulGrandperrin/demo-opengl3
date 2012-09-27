/*
TODO
-implémenter les VAO
-implémenter les FBO
-implémenter le chargement des textures depuis les fichiers .obj
-implémenter le support du rendu de plusieurs meshes
-implémenter une caméra freefly piloté par la souris
-implémenter le normal mapping

HDR, shadow volume, depth of field, chargement des spéculaire depuis fichier .obj,
créer format de fichier binaire, animated mesh by key frames, transparence,
blinn phong (half vector), point light, directional light, spot light, bump mapping, parallax mapping,relief mapping, displacement mapping ( hight map et geometry shader)
fur par shell texturing, cuve environnement mapping ( refraction et reflection) , bloom, chromatic aberation



*/


#include <GL/glew.h>

typedef int GEint;

class GraphicEngine
{
	public:
		GraphicEngine();
		GraphicEngine(unsigned int winWidth,unsigned int winHeight,bool fullscreen);
		~GraphicEngine();
		
		void setCameraFovy(float fovy);
		void setCameraPosition(float x,float y, float z, float ax,float ay, float az);
		
		void setBackgroundColor(unsigned char R,unsigned char G, unsigned char B,unsigned char A);
		
		void setWindowTitle(const char* title);
		void setShowCursor(bool show);
		void setWindowResolution(unsigned int winWidth,unsigned int winHeight,bool fullscreen);
		
		bool loadMesh(GEint* meshId,const char* pathName);
		void unLoadMesh(GEint);
		
		void setMeshAttribute(unsigned int program, float x,float y, float z, float ax,float ay, float az,float sx, float sy, float sz);
		
		void setLightDir(float x,float y,float z);
		
		unsigned int loadProgram(const char *vsname,const char* gsname, const char *psname);
		
		void render(float time);
		
		// Benchmark
		
		float getSceneRenderTime(); //millisecondes
		
		float getMeshRenderTime(GEint);
		
		float getPostFXRenderTime();
		
		
	protected:
		
		
		
		
		
		
	private:
		
		struct
		{
			unsigned int width,height;
			bool fullscreen;
		} m_window;
		
		struct
		{
			float x,y,z,ax,ay,az,fovy;
		} m_camera;
		
		struct
		{
			float R,G,B,A;
		} m_backgroundColor;
		
		struct
		{
			GLuint ibo;
			GLuint vboVertex;
			unsigned int sizeofVertex,sizeofIbo;
			float x,y,z,ax,ay,az,sx,sy,sz;
			unsigned int program;
			GLuint textureMap;
			GLuint normalMap;
		} m_mesh;
		
		float m_lightDir[3];
		
		float projectionMatrix[4][4];
		float modelMatrix[4][4];
		float viewMatrix[4][4];
		
		float modelViewMatrix[4][4];
		float modelViewProjectionMatrix[4][4];
		
		float normalMatrix[3][3];
		
		GLuint modelMatrixId,viewMatrixId,modelViewMatrixId,projectionMatrixId,modelViewProjectionMatrixId,normalMatrixId,lightDirId,cameraDirId;
		
};


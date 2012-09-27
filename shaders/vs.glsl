#version 130
//invariant vec4 gl_Position;
// On recoie un vecteur
in vec3 vertexPosition,vertexNormal;
in vec2 vertexTexture;

// On renvoie une normale
smooth out vec3 normal;
smooth out vec2 texPos;


// Avec pour parametre deux matrices
uniform mat4 modelMatrix,viewMatrix,modelViewMatrix, projectionMatrix,modelViewProjectionMatrix;
uniform mat3 normalMatrix;


void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertexPosition,1.0);

	normal=normalMatrix*vertexNormal;

	texPos=vertexTexture;
}
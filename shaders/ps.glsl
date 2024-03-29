#version 130
precision highp float;
smooth in vec3 normal;
smooth in vec2 texPos;
uniform vec3 lightDir,cameraDir;
uniform sampler2D textureMap;

out vec4 fragColor;

void main()
{
	vec4 color=texture(textureMap,texPos);
	vec3 normal=normalize(normal);

	float al=0.2;
	float dl= max(dot(lightDir,normal),0.0)*1.5;
	float sl= pow(max(dot(-normalize(cameraDir),reflect(-lightDir,normal)),0.0),16)*32;


	fragColor=color*(al+dl+sl);

}

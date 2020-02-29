#version 330 core

in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 FragColor;

uniform sampler2D sprite;

void main()
{
	//vec4 ParticleColor = vec4(1.0f, 0.5f, 0.0f,1.0f);
	vec4 temp = texture(sprite, TexCoords)*ParticleColor;
	//vec3 fColor = vec3(1.0f,0.0f,0.0f);
	if(temp.a < 0.05)
		discard;
    FragColor = temp; //vec4(fColor, 1.0);
}
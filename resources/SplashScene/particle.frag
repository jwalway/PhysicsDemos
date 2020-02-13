#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D sprite;

void main()
{
    // color = (texture(sprite, TexCoords) * ParticleColor);
	vec4 temp = (texture(sprite, TexCoords) * ParticleColor);
	
	//temp = ParticleColor;
	
	if(temp.a < 0.1)
		discard;
	//else{
	//	temp = ParticleColor;
	//}
	
	color = temp;
    //color = ParticleColor; 
	//*ParticleColor;
	//color = texture(sprite, TexCoords);
}  
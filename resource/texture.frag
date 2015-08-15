#version 330 core
 
// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Normal;

// Ouput data
out vec3 color;
 
// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
 
void main(){
	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	//float cosTheta = clamp( dot( n,l ), 0,1 );

    // Output color = color of the texture at the specified UV
    color = texture( myTextureSampler, UV ).rgb;// * cosTheta;

}
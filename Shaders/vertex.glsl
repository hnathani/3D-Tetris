#version 150

in vec4 vPosition;
in vec4 vColor;
in vec4 vNormal;
smooth out vec4 oColor;
uniform mat4 modelView;
uniform mat4 projection;
uniform mat4 normalMatrix;
uniform vec4 Ambient, Diffuse, Specular;
uniform vec4 LightDirection;
uniform float useLighting;
void main()
{
	if(useLighting == 1) {
		vec3 V = (modelView * vPosition).xyz;
		vec3 E = normalize( -V );
		vec3 H = normalize( LightDirection.xyz + E );
		
		vec3 N = normalize( modelView*vec4(vNormal.xyz, 0.0) ).xyz;
		
		//Ambient
		vec4 A = Ambient;
		
		//Calculate Specular
		float sAmount = max(dot(N, H), 0.0);
		vec4 S = sAmount * Specular;
		
		if( dot(LightDirection.xyz, N) < 0.0 ) {
			S = vec4(0.0, 0.0, 0.0, 1.0);
		}
		
		//Calculate Diffuse
		float dAmount = max( dot(LightDirection.xyz, N), 0.0 );
		vec4 D = dAmount*Diffuse;
		
		//Set color to calculated values
		oColor = .5 * vColor + .5 * (A + D + S);
		oColor.a = 1.0;
    
    } else {
    	 oColor = vColor;
    }
    
	gl_Position = projection * modelView * vPosition;
}
	

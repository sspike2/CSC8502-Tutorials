#version 330 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseLight;
uniform sampler2D emissionTex;
uniform sampler2D specularLight;

// uniform vec4 emissionColor;
in Vertex { vec2 texCoord; }
IN;

out vec4 fragColour;

void main(void) {
  vec3 diffuse = texture(diffuseTex, IN.texCoord).xyz;
  vec3 light = texture(diffuseLight, IN.texCoord).xyz;
  vec3 specular = texture(specularLight, IN.texCoord).xyz;
  vec4 emission = texture(emissionTex, IN.texCoord);

  fragColour.xyz = diffuse * 0.1;    // ambient
  fragColour.xyz += diffuse * light; // lambert
  fragColour.xyz += specular;        // Specular

//   vec3 emiscolor = vec3(1, 0, 0);
//   emission.xyz *= emiscolor;

  fragColour += mix(fragColour, emission, emission.a);

  fragColour.a = 1.0;
}
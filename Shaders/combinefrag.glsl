#version 330 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseLight;
uniform sampler2D emissionTex;
uniform sampler2D specularLight;
uniform sampler2D depthTex;
uniform vec4 fogcolor;

float near = .1f;
float far = 1000f;

// uniform vec4 emissionColor;
in Vertex { vec2 texCoord; }
IN;

out vec4 fragColour;

void main(void) {
  vec3 diffuse = texture(diffuseTex, IN.texCoord).xyz;
  vec3 light = texture(diffuseLight, IN.texCoord).xyz;
  vec3 specular = texture(specularLight, IN.texCoord).xyz;
  vec4 emission = texture(emissionTex, IN.texCoord);
  float depth = texture(depthTex, IN.texCoord).r;

  fragColour.xyz = diffuse * 0.1;    // ambient
  fragColour.xyz += diffuse * light; // lambert
  fragColour.xyz += specular;        // Specular

  //   vec3 emiscolor = vec3(1, 0, 0);
  //   emission.xyz *= emiscolor;

  fragColour += mix(fragColour, emission, emission.a);

  float z = depth * 2.0 - 1.0; // back to NDC
  float linearDepth = (2.0 * near * far) / (far + near - z * (far - near));

  // depth = 1.0f - depth;

  fragColour = mix(fragColour, fogcolor, depth);

  // fragColour = vec4(depth, 0, 0, 1);

  // fragColour = vec4(diffuse, 1);
  // fragColour = vec4(depth, depth, depth, 1);
  // fragColour.a = 1.0;
}
#version 330 core

uniform sampler2D diffuseTex; // Diffuse texture map
uniform sampler2D bumpTex;    // Bump map
uniform sampler2D emiisionTex;

uniform vec4 emissionColor;


in Vertex {
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 tangent;
  vec3 binormal;
  vec3 worldPos;
}
IN;

out vec4 fragColour[3]; // Our final outputted colours !

void main(void) {
  mat3 TBN =
      mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));

  vec3 normal = texture2D(bumpTex, IN.texCoord).rgb * 2.0 - 1.0;
  normal = normalize(TBN * normalize(normal));

  vec4 diffuse = texture2D(diffuseTex, IN.texCoord);
  vec4 emiss = texture2D(emiisionTex, IN.texCoord);

  //  diffuse += emiss;

  //   fragColour[0] += mix(diffuse, emiss, emiss.a);
  fragColour[0] = diffuse;
  fragColour[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
  fragColour[2] = emiss * emissionColor;
}
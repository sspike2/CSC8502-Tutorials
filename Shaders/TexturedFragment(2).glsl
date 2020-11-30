#version 330 core
uniform sampler2D diffuseTex;
uniform sampler2D alphaTex;
uniform sampler2D bumpTex;

uniform vec4 EmmisColour;

in Vertex { vec2 texCoord; }
IN;

out vec4 fragColour;
void main(void) {

  vec4 diff = texture(diffuseTex, IN.texCoord);

  vec4 alpha = texture(alphaTex, IN.texCoord);

  vec4 bump = texture(bumpTex, IN.texCoord);

  //   alpha.normalize();

  vec4 col = vec4(0, 0, 0, 0);
  col += mix(diff, EmmisColour, alpha.a);

  fragColour += mix(col, bump, alpha.a);
}
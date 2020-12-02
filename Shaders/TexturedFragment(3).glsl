#version 440 core
uniform sampler2D diffuseTex;

uniform vec4 fogcolor;
uniform float linearStart;
uniform float linearEnd;
uniform float density;
uniform float u_time;

float val;

in Vertex {
  vec2 texCoord;
  vec4 ioEyeSpacePosition;
}
IN;

out vec4 fragColour;
void main(void) {

  vec4 diffusecolor = texture(diffuseTex, IN.texCoord);

  // fragColour =

  float fogCoordinate = abs(IN.ioEyeSpacePosition.z / IN.ioEyeSpacePosition.w);

  float result = 0;
  //   float foglength = linearEnd - linearStart;
  //   result = (linearEnd - fogCoordinate) / foglength;

  //   result *= density;

  //   result = exp(-density * fogCoordinate);

  result = exp(-pow(density * fogCoordinate, 2.0));

  result = 1.0 - clamp(result, 0.0, 1.0);

  fragColour = mix(diffusecolor, fogcolor, result);

   val += (abs(sin(u_time)));

  fragColour = vec4(val,val,val,1);

  // fragColour = fogcolor;
}
#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec4 nodeColour;
uniform mat4 textureMatrix;

in vec3 position;
in vec2 texCoord;

out Vertex {
  vec2 texCoord;
  vec4 colour;
  vec4 gl_Position;
}
OUT;

void main(void) {
  gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
  OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
  OUT.colour = nodeColour;
}
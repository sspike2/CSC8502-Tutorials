#version 330 core
// precision highp float;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec2 texCoord;
in vec3 normal; 

out Vertex {
  vec3 normal;
  vec3 Position;
  vec2 Uv;
}
OUT;

void main() {

  mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
  OUT.normal = normalize(normalMatrix * normalize(normal));


  OUT.Uv = texCoord;

  mat4 mvp = projMatrix * viewMatrix * modelMatrix;
  vec4 pos =  mvp * vec4(position, 1.0);
  OUT.Position = pos.xyz;
  gl_Position = pos;

}

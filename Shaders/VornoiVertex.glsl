#version 330 core
/**
 * Example Vertex Shader
 * Sets the position of the vertex by setting gl_Position
 */

// Set the precision for data types used in this shader
precision highp float;
precision highp int;

// Default THREE.js uniforms available to both fragment and vertex shader
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

// Default uniforms provided by ShaderFrog.
uniform vec3 cameraPosition;
uniform float time;

// Default attributes provided by THREE.js. Attributes are only available in the
// vertex shader. You can pass them to the fragment shader using varyings
in vec3 position;
in vec3 normal;
in vec2 uv;
in vec2 uv2;

// Examples of variables passed from vertex to fragment shader
out Vertex {
  vec3 Position;
  vec3 Normal;
  vec2 Uv;
  vec2 Uv2;
}
OUT;

void main() {

  // To pass variables to the fragment shader, you assign them here in the
  // main function. Traditionally you name the varying with vAttributeName
  OUT.Normal = normal;
  OUT.Uv = uv;
  OUT.Uv2 = uv2;
  OUT.Position = position;

  // This sets the position of the vertex in 3d space. The correct math is
  // provided below to take into account camera and object data.
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
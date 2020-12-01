#version 330 core
//precision highp float;

uniform vec3 color;
uniform float start;
uniform float end;
uniform float alpha;

in Vertex {
  vec3 normal;  
  vec3 Position;
  vec2 Uv;
}
IN;

out vec4 fragColour;

void main() {
  vec3 normal = IN.normal;
  vec3 eye = normalize(-IN.Position.xyz);
  float rim = smoothstep(0, 1, 1.0 - dot(normal, eye));

  fragColour = vec4(clamp(rim, 0.0, 1.0) * 1 * vec3(1,0,0), 1.0);
}
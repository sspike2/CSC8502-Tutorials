#version 330 core

uniform sampler2D diffuseTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;
uniform vec3 lightDir;
uniform float outerCutoff;
uniform float cutoff;
uniform float angle;

in Vertex {
  vec3 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 worldPos;
}
IN;

out vec4 fragColour;

void main(void) {

  // calculate vectors
  vec3 incident = normalize(lightPos - IN.worldPos);
  vec3 viewDir = normalize(cameraPos - IN.worldPos);
  vec3 halfDir = normalize(incident + viewDir);
  float distance = length(lightPos - IN.worldPos);

  // diffuse
  vec4 diffuse = texture(diffuseTex, IN.texCoord);
  float lambert = max(dot(incident, IN.normal), 0.0f);
  vec3 surface = diffuse.rgb * lightColour.rgb * lambert;

  // ambient
  vec3 ambient = diffuse.rgb * lightColour.rgb * .1f;

  // spec
  // vec3 reflectDIr = reflect(-incident,IN.normal);
  float specFactor = clamp(dot(halfDir, IN.normal), 0.0, 1.0);
  specFactor = pow(specFactor, 60.0);
  vec3 spec = lightColour.rgb * specFactor * 0.33;

  // spotlight

  float theta = dot(incident, -lightDir);
  float epsilon = (cutoff - outerCutoff);
  float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

  surface *= intensity;
  spec *= intensity;

  // attenuation
  // float attenuation = 1.0 - clamp(distance / lightRadius, 0.0, 1.0);
  float attenuation = 1.0 / (1.0 + lightRadius * pow(distance, 2));

  float lightToSurfaceAngle = degrees(acos(dot(-incident, lightDir)));
  if (lightToSurfaceAngle > angle) {
    attenuation = 0.0;
  }

  // spec

  ambient *= attenuation;
  surface *= attenuation;
  spec    *= attenuation;

  fragColour = vec4((ambient + surface + spec), 1.0f);

  // vec3 ambient = surface * .1f * attenuation;

  // fragColour = vec4((ambient + surface + spec),1.0f);

  // fragColour.rgb = surface * lambert * attenuation;
  // fragColour.rgb += (lightColour.rgb * specFactor) * attenuation * 0.33;
  // fragColour.rgb += spec;
  // fragColour.rgb += surface * 0.1f; // ambient !
  // fragColour.a = diffuse.a;
}

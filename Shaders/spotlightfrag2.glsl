#version 330 core

uniform sampler2D diffuseTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
// uniform float attenuation;
uniform vec3 lightDir;
uniform float angle;

// uniform float materialShininess;
// uniform vec3 materialSpecularColor;

// #define MAX_LIGHTS 10
// uniform int numLights;
// uniform struct Light {
//   float ambientCoefficient;
// } allLights[MAX_LIGHTS];

in Vertex {
  vec3 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 worldPos;
}
IN;
out vec4 fragColour;

void main() {
  vec4 surfaceColor = texture(diffuseTex, IN.texCoord);
  vec3 surfaceToCamera = normalize(cameraPos - IN.worldPos);

  vec3 surfaceToLight;
  float attenuation = 1.0;

  // point light
  surfaceToLight = normalize(lightPos - IN.worldPos);
  float distanceToLight = length(lightPos - IN.worldPos);
  attenuation = 1.0 / (1.0 + 0.1 * pow(distanceToLight, 2));

  // cone restrictions (affects attenuation)
  float lightToSurfaceAngle =
      degrees(acos(dot(-surfaceToLight, normalize(lightDir))));
  if (lightToSurfaceAngle > angle) {
    attenuation = 0.0;
  }

  // ambient
  vec3 ambient = .1f * surfaceColor.rgb * lightColour.rbg;

  // diffuse
  float diffuseCoefficient = max(0.0, dot(IN.normal, surfaceToLight));
  vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * lightColour.rgb;

  // specular
  float specularCoefficient = 0.0;
  if (diffuseCoefficient > 0.0)
    specularCoefficient =
        pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, IN.normal))),
            1.0f);
  vec3 specular = specularCoefficient * lightColour.rgb;

  fragColour.rgb = ambient + attenuation * (diffuse + specular);
  fragColour.a = 1.0f;

  // vec3 gamma = vec3(1.0 / 2.2);
  // fragColour = vec4(pow(linearColor, gamma), surfaceColor.a);
}
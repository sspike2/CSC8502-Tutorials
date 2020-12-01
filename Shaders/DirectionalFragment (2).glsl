#version 150

uniform sampler2D diffuseTex ;
uniform sampler2D bumpTex ; // New !


uniform mat4 model;

uniform vec3 cameraPos ;
uniform vec4 lightColour ;
uniform vec3 lightPos ;
uniform float lightRadius ;



in Vertex {

 vec3 fragVert;
 vec2 texCoord;
 vec3 normal;
 vec4 colour;
 vec3 tangent; 
 vec3 binormal; // New !
 vec3 worldPos;

}IN;


out vec4 finalColor;

void main() {

    
    //calculate the location of this fragment (pixel) in world coordinates
    // vec3 fragPosition = vec3(model * vec4(fragVert, 1));    
    //calculate the vector from this pixels surface to the light source
   // vec3 surfaceToLight = lightPos - IN.worldPos;

    //calculate the cosine of the angle of incidence
  //  float brightness = dot(IN.normal, surfaceToLight) / (length(surfaceToLight) * length(IN.normal));

  //  brightness = clamp(brightness, 0, 1);

    //calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
    // 3. The texture and texture coord: texture(tex, fragTexCoord)
   // vec4 surfaceColor = texture(diffuseTex, IN.texCoord);
//finalColor = vec4(brightness * lightColour.xyz * surfaceColor.rgb, surfaceColor.a);






    vec3 incident = normalize ( lightPos - IN. worldPos );
    vec3 viewDir = normalize ( cameraPos - IN. worldPos );
    vec3 halfDir = normalize ( incident + viewDir );
    
    mat3 TBN = mat3 ( normalize (IN. tangent ),
    normalize (IN. binormal ), normalize (IN. normal ));
    vec4 diffuse = texture ( diffuseTex , IN. texCoord );
    vec3 bumpNormal = texture ( bumpTex , IN. texCoord ).rgb;
    bumpNormal = normalize (TBN * IN.normal * 2.0 - 1.0);

    // float reflec
    
   // float lambert = max (dot ( incident , bumpNormal ),1.0f);
    
    float specFactor = clamp (dot ( halfDir , bumpNormal ) ,0.0 ,1.0);
    specFactor = pow ( specFactor , 60.0 );
    vec3 surface = ( diffuse .rgb * lightColour .rgb );
    finalColor.rgb = surface;// * lambert;
    //finalColor.rgb += ( lightColour .rgb * specFactor )* 0.33;
   // finalColor.rgb += surface * 0.1f;
    finalColor.a = diffuse .a;










}
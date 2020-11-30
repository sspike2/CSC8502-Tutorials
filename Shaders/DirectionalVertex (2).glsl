#version 150

uniform mat4 camera;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix ;
uniform mat4 projMatrix ;


in vec3 vert;
in vec2 texCoord;
in vec3 normal;
in vec4 tangent ; // New ! Note , Vec4 !
in vec4 colour ;
in vec3 position ;


out Vertex {

 vec3 fragVert;
 vec2 texCoord;
 vec3 normal;
 vec4 colour;
 vec3 tangent; 
 vec3 binormal; // New !
 vec3 worldPos;

}OUT;




void main() {

     OUT.colour = colour ;
    // Pass some variables to the fragment shader
    OUT.texCoord = texCoord;


    mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));
    vec3 wNormal = normalize ( normalMatrix * normalize ( normal ));
    vec3 wTangent = normalize ( normalMatrix * normalize ( tangent .xyz ));

    OUT.normal = wNormal ;  
    OUT.tangent = wTangent ;

     OUT . binormal = cross ( wNormal , wTangent ) * tangent .w;

        vec4 worldPos = ( modelMatrix * vec4 ( position ,1));

 OUT . worldPos = worldPos .xyz ;
    
    // Apply all matrix transformations to vert 
    gl_Position = ( projMatrix * viewMatrix ) * worldPos ;
}
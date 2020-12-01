#version 330 core
/**
* Example Fragment Shader
* Sets the color and alpha of the pixel by setting gl_FragColor
* take from The Book of Shaders by Patricio Gonzalez Vivo & Jen Lowe
*/

// Set the precision for data types used in this shader
precision highp float;
precision highp int;

// Default THREE.js uniforms available to both fragment and vertex shader
uniform mat4 modelMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

// Default uniforms provided by ShaderFrog.
uniform vec3 cameraPosition;
uniform float time;

// A uniform unique to this shader. You can modify it to the using the form
// below the shader preview. Any uniform you add is automatically given a form
uniform vec3 diffuseColor;
uniform vec2 pattern_scale;

// Example varyings passed from the vertex shader
 in vertex
 {

 vec3 vPosition;
 vec3 vNormal;
 vec2 vUv;
 vec2 vUv2;
 }IN;



out vec4 fragColour;

//#define SHOW_DOTS
//#define SHOW_GRID
//#define SHOW_ISOLINES
//#define PATTERN_CIRCLE
//#define PATTERN_RAMP
#define PATTERN_VORONOI

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

vec3 voronoi(vec2 st){
        // Tile the space
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float m_dist = 1.;  // minimum distance

    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x),float(y));

            // Random position from current + neighbor place in the grid
            vec2 point = random2(i_st + neighbor);

			// Animate the point
            point = 0.5 + 0.5*sin(time + 6.2831*point);

			// Vector between the pixel and the point
            vec2 diff = neighbor + point - f_st;

            // Distance to the point
            float dist = length(diff);

            // Keep the closer distance
            m_dist = min(m_dist, dist);
        }
    }

    // Draw the min distance (distance field)
    vec3 color =  vec3(0.0);
    color += m_dist;

    #ifdef SHOW_DOTS
    // Draw cell center
    color += 1.-step(.02, m_dist);
    #endif
    
    #ifdef SHOW_GRID
    // Draw grid
    color.r += step(.98, f_st.x) + step(.98, f_st.y);
    #endif
    
    #ifdef SHOW_ISOLINES
    // Show isolines
    color -= step(.7,abs(sin(27.0*m_dist)))*.5;
    #endif
    return color;
}

float circle(in vec2 _st, in float _radius){
    vec2 l = _st-vec2(0.5);
    return 1.-smoothstep(_radius-(_radius*0.01), _radius+(_radius*0.01), dot(l,l)*4.0);
}

void main() {

    //vec2 st = gl_FragCoord.xy/pattern_scale;
    vec2 st = IN.vUv.xy/pattern_scale;
    vec3 color = vec3(0.0);

    st *= 3.0;      // Scale up the space by 3
    st = fract(st); // Wrap around 1.0

    // Now we have 9 spaces that go from 0-1
    #ifdef PATTERN_RAMP
    color = vec3(st,0.0);
    #endif
    #ifdef PATTERN_CIRCLE
    color = vec3(circle(st,0.8));
    #endif
    #ifdef PATTERN_VORONOI
    color = voronoi(IN.vUv.xy/pattern_scale * 3.0);
    #endif

	fragColour = vec4(color*diffuseColor,1.0);
}
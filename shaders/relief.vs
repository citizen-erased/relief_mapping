// object space eye position
uniform vec3 eye_pos;

// object space light position
uniform vec3 light_pos;


attribute vec4 tangent;


// tangent space vector to the light
varying vec3 to_light;

//tangent space vector to the eye
varying vec3 to_eye;

// fragment position in tangent space
varying vec3 position_tan;

// eye space vector to fragment. used for depth correct.
varying vec3 eye_to_pos;


void main()
{
    vec3 bitangent = cross(gl_Normal, tangent.xyz) * tangent.w;
    mat3 TBN = mat3(tangent.xyz, bitangent, gl_Normal);

    // TBN transforms from tangent to object space so post multiply
    // to transform by its "inverse" since we need to go from object
    // to tangent space.
    to_light = (light_pos - vec3(gl_Vertex)) * TBN;
    to_eye = (eye_pos - vec3(gl_Vertex)) * TBN;

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    position_tan = vec3(gl_Vertex) * TBN;

    eye_to_pos = vec3(gl_ModelViewMatrix * gl_Vertex);
}


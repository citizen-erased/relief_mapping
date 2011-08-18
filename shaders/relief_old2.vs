// object space eye position
uniform vec3 eye_pos;

// object space light position
uniform vec3 light_pos;

// uniform mat4 obj_to_world;


attribute vec4 tangent;


varying vec3 to_light;
varying vec3 to_eye;

varying vec3 position_tan;
varying vec3 light_position_tan;

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
    light_position_tan = (light_pos - gl_Vertex.xyz) * TBN;
}


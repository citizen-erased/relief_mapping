#define SHADOWS         1
#define DEPTH_CORRECT   1
#define SHOW_DEPTH      0

#define LINEAR_STEPS    60
#define BINARY_STEPS    16

uniform sampler2D diffuse_map;
uniform sampler2D relief_map;

// scale of the depth map so the 0-1 range looks good in whatever unit scale
// the application is using.
uniform float scale;


// tangent space stuff:

varying vec3 to_light;
varying vec3 to_eye;
varying vec3 position_tan;


//eye space stuff:

varying vec3 eye_to_pos;


// The two searches are separated so only one can be used to get less accurate,
// but faster, shadows. But it's not used like that since the quality isn't
// great.

float linearSearch(vec2 A, vec2 B)
{
    float t = 0.0;

    for(int i = 0; i < LINEAR_STEPS; i++)
    {
        t += 1.0 / LINEAR_STEPS;
        float d = texture2D(relief_map, mix(A, B, t)).a;
        if(t > d) break;
    }

    return t;
}

float binarySearch(vec2 A, vec2 B, float a, float b)
{
    float depth;

    for(int i = 0; i < BINARY_STEPS; i++)
    {
        depth = mix(a, b, 0.5);
        float d = texture2D(relief_map, mix(A, B, depth)).a;

        if(d > depth)
            a = depth;
        else
            b = depth;
    }

    return depth;
}

float fullSearch(vec2 A, vec2 B)
{
    float depth = linearSearch(A, B);
    return binarySearch(A, B, depth-(1.0 / LINEAR_STEPS), depth);
}


void main()
{
    vec2 A = gl_TexCoord[0].st; //entry point of view ray in texture coordinates
    vec3 V = (to_eye / -to_eye.z) * scale; //vector from A to the exit point (B)
    vec2 B = A + V.xy;

    float depth = fullSearch(A, B);

    // the intersection point in texture space
    vec3 P = vec3(mix(A, B, depth), depth);


    // correct light vector should go through from the point in the depth map,
    // P, to the light
    vec3 P_tan = position_tan + (to_eye / -to_eye.z) * scale * depth;
    vec3 p_to_light = (position_tan + to_light) - P_tan;


    vec4 diffuse_col = texture2D(diffuse_map, P.xy);
    vec4 col = diffuse_col * vec4(0.1, 0.1, 0.1, 1.0);

    // normal mapping normal
    vec3 norm = texture2D(relief_map, P.xy).rgb;
    norm = normalize((norm - 0.5) * 2.0);

    float n_dot_l = max(dot(norm, normalize(p_to_light)), 0.0);
    if(n_dot_l > 0.0)
    {
#if SHADOWS
        vec3 l_entry = P + (p_to_light / p_to_light.z) * scale * depth;
        vec3 l_exit  = l_entry + (p_to_light / -p_to_light.z) * scale;
        float l_depth = fullSearch(l_entry.xy, l_exit.xy);

        if(l_depth < depth-0.05) //in shadow
        {
            col += diffuse_col * 0.2 * n_dot_l;
        }
        else
        {
            col += diffuse_col * n_dot_l;
            vec3 H = normalize(p_to_light + to_eye);
            col += vec4(0.5, 0.5, 0.5, 1.0) * pow(max(dot(norm,H),0.0), 64.0);
        }
#else
        col += diffuse_col * n_dot_l;

        vec3 H = normalize(p_to_light + to_eye);
        col += vec4(0.5, 0.5, 0.5, 1.0) * pow(max(dot(norm,H),0.0), 64.0);
#endif
    }


#if DEPTH_CORRECT
    // depth correct formula as described in the paper.
    // the near and far plane constants could be uniforms if they
    // need to vary.
    const float near = 0.1;
    const float far  = 1024.0;
    float p_eye_z = eye_to_pos.z + normalize(eye_to_pos).z * scale * depth;
    gl_FragDepth = ((-far / (far - near)) * p_eye_z + (-far * near / (far - near))) / -p_eye_z;
#endif


#if SHOW_DEPTH
    gl_FragColor = vec4(depth, depth, depth, 1.0);
#else
    gl_FragColor = col;
#endif
}


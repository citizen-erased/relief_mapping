uniform sampler2D diffuse_map;
uniform sampler2D relief_map;

varying vec3 to_light;
varying vec3 to_eye;

varying vec3 position_tan;
varying vec3 light_position_tan;


// A - texture coordinate given by application
// B - texture coordinate at depth=1.0
float findDepth(vec2 A, vec2 B)
{
    //vec2 V = B - A;
    float depth;
    float a = 0.0;
    float b = 1.0;

#if 0
    float t = 0.0;
    for(int i = 0; i < 10; i++)
    {
        t += 0.1;
        float d = texture2D(relief_map, mix(A, B, t)).a;

        if(t > d)
        {
            b = t;
            a = t-0.1;
            break;
        }
    }
#endif

#if 1
    a = 0.0;
    b = 0.0;
    for(int i = 0; i < 20; i++)
    {
        a = b;
        b += 0.05;

        float d = texture2D(relief_map, mix(A, B, b)).a;

        if(b > d) break;
    }
#endif

    for(int i = 0; i < 5; i++)
    {
        //depth = a + (b - a) * 0.5;
        //float d = texture2D(relief_map, V*depth + A).a;
        depth = mix(a, b, 0.5);
        float d = texture2D(relief_map, mix(A, B, depth)).a;

        if(d > depth)
            a = depth;
        else
            b = depth;
    }

    return depth;
    //return texture2D(relief_map, V*depth + A).a;
    //return texture2D(relief_map, A).a;
}

void main()
{
    const float scale = 0.1;

    vec2 A = gl_TexCoord[0].st; //entry point of view ray in texture coordinates
    vec3 n_to_eye = normalize(to_eye);
    vec2 B = (vec3(A, 0.0) - n_to_eye * (1.0/n_to_eye.z) * scale).xy; //exit point of view ray in texture coordinates
    float depth = findDepth(A, B);

    vec2 r_tex_coord = mix(A, B, depth);

    // normal mapping normap
    vec3 norm = texture2D(relief_map, r_tex_coord).rgb;
    norm = normalize((norm - 0.5) * 2.0);

    //
    // use uncorrected light position. perhaps this artefact isn't noticable
    // and can be used as a speedup?
    //
    //vec3 r_to_light = to_light;

    //
    // correct light position
    //
    vec3 eyeB = -to_eye * (1.0/to_eye.z) * depth * scale;
    eyeB.z = 0.0;
    vec3 r_to_light = light_position_tan - eyeB;

    vec4 diffuse_col = texture2D(diffuse_map, r_tex_coord);
    //ambient
    vec4 col = diffuse_col * vec4(0.1, 0.1, 0.1, 1.0);

    float n_dot_l = max(dot(norm, normalize(r_to_light)), 0.0);
    if(n_dot_l > 0.0)
    {
        col += diffuse_col * n_dot_l;

        vec3 H = normalize(r_to_light + to_eye);
        col += vec4(0.5, 0.5, 0.5, 1.0) * pow(max(dot(norm,H),0.0), 64.0);
    }

#if 0
    vec3 D = vec3(r_tex_coord, depth);
    vec3 C = D + (vec3(light_position_tan.x, light_position_tan.y, -light_position_tan.z) - D);
    C *= depth / -C.z;

    //vec3 E = C + normalize(D-C);
    //vec3 E = C + (normalize(D-C).z / 1.0);
    vec3 E = D-C;
    E *= 1.0 / depth;

    float light_depth = findDepth(vec2(C), vec2(E));
    //light_depth = findDepth(vec2(A), vec2(B));
    if(light_depth < depth-0.05) //in shadow
    //if(light_depth < 0.95) //in shadow
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
        gl_FragColor = col;

    //gl_FragColor = vec4(light_depth, 0.0, 0.0, 1.0);
    //gl_FragColor = vec4(0.0, 0.0, E.z, 1.0);
    //gl_FragColor = texture2D(diffuse_map, vec2(E));
    //gl_FragColor = vec4(light_position_tan, 1.0);
#endif

    gl_FragColor = col;
    //gl_FragColor = vec4(depth, depth, depth, 1.0);
    //gl_FragColor = texture2D(diffuse_map, r_tex_coord)/2.0 + texture2D(relief_map, r_tex_coord)/2.0;
    //gl_FragColor = texture2D(diffuse_map, r_tex_coord);
    //gl_FragColor = texture2D(relief_map, r_tex_coord);
    //gl_FragColor = texture2D(relief_map, gl_TexCoord[0].st);
    //gl_FragColor = vec4(to_light, 1.0);
    //gl_FragColor = vec4(to_eye, 1.0);
}


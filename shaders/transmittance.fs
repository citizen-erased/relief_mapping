varying vec3 reflect_vec;
varying vec3 refract_vec;
varying vec3 refract_vec_r;
varying vec3 refract_vec_g;
varying vec3 refract_vec_b;
varying vec3 ratio;

uniform samplerCube cube_map;

void main()
{
    //gl_FragColor = textureCube(cube_map, reflect_vec);
    //gl_FragColor = textureCube(cube_map, refract_vec);
    //gl_FragColor = 0.2*textureCube(cube_map, reflect_vec) + 0.8*textureCube(cube_map, refract_vec);

    vec4 col_reflect = textureCube(cube_map, reflect_vec);

    vec4 col_refract;
    col_refract.r = textureCube(cube_map, refract_vec_r).r;
    col_refract.g = textureCube(cube_map, refract_vec_g).g;
    col_refract.b = textureCube(cube_map, refract_vec_b).b;
    col_refract.a = 1.0;

    gl_FragColor = mix(col_reflect, col_refract, vec4(ratio, 1.0));
}


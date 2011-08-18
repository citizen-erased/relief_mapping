uniform sampler2d diffuse_map;
uniform sampler2d normal_map;

varying vec3 to_light;
varying vec3 to_eye;

void main()
{
    vec3 norm = texture2d(normal_map, gl_texcoord[0].st).xyz;
    norm = normalize((norm - 0.5) * 2.0);

    float n_dot_l = max(dot(norm, normalize(to_light)), 0.0);

    vec4 col = vec4(0.0);
    if(n_dot_l > 0.0)
    {
        vec4 diffuse_col = texture2d(diffuse_map, gl_texcoord[0].st);

        col += diffuse_col * n_dot_l;


        vec3 h = normalize(to_light + to_eye);
        col += gl_frontlightproduct[0].specular * pow(max(dot(norm,h),0.0), 40.0f);
    }


    gl_fragcolor = col;
}


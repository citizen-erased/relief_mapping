varying vec3 reflect_vec;
varying vec3 refract_vec;
varying vec3 refract_vec_r;
varying vec3 refract_vec_g;
varying vec3 refract_vec_b;
//varying float ratio;
varying vec3 ratio;

uniform vec3 w_eye_position;
uniform mat4 m_world;
uniform float eta_r;
uniform float eta_g;
uniform float eta_b;
uniform float F;
uniform float Fr;
uniform float Fg;
uniform float Fb;

void main()
{
    vec3 w_position = vec3(m_world * gl_Vertex);
    vec3 w_normal = normalize(vec3(m_world * vec4(gl_Normal, 0.0)));
    vec3 w_incident = normalize(w_position - w_eye_position);
    
    reflect_vec = reflect(w_incident, w_normal);
    //refract_vec = refract(w_incident, w_normal, eta);

    refract_vec_r = refract(w_incident, w_normal, eta_r);
    refract_vec_g = refract(w_incident, w_normal, eta_g);
    refract_vec_b = refract(w_incident, w_normal, eta_b);

    //ratio = F + (1.0 - F) * pow((1.0 - dot(-w_incident, w_normal)), 5.0);
    ratio.r = Fr + (1.0 - Fr) * pow((1.0 - dot(-w_incident, w_normal)), 5.0);
    ratio.g = Fg + (1.0 - Fg) * pow((1.0 - dot(-w_incident, w_normal)), 5.0);
    ratio.b = Fb + (1.0 - Fb) * pow((1.0 - dot(-w_incident, w_normal)), 5.0);


    //doing computations in eye space
    //vec3 incident = normalize(gl_ModelViewMatrix * gl_Vertex).xyz;
    //vec3 normal   = normalize(gl_NormalMatrix * gl_Normal);

    //reflect_vec = reflect(incident, normal);
    //refract_vec = refract(incident, normal, eta);

    //ratio = F + (1.0 - F) * pow((1.0 - dot(-incident, normal)), 5.0);


    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}


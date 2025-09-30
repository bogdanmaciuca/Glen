precision mediump float;

varying vec2 o_tex_coord;
varying vec3 o_frag_pos;
varying vec3 o_normal;
uniform vec3 u_cam_pos;
uniform sampler2D u_sampler;

uniform vec3 u_light_color;
uniform vec3 u_color;
uniform int u_use_diffuse;
uniform int u_use_normal;

void main() {
    vec3 color;
    vec3 light_color;
    vec3 normal;

    // Light color
    if (u_light_color == vec3(0))
        light_color = vec3(1);

    // Diffuse
    if (u_use_diffuse == 0)
        color = u_color;
    else
        color = texture2D(u_sampler, o_tex_coord).xyz;

    // Normal map
    // ...

    vec3 light_dir = vec3(1);
    
    float shading = max(0.0, dot(normalize(o_normal), normalize(light_dir)));
    gl_FragColor = vec4(color * shading, 1.0);
}


precision mediump float;

varying vec2 o_tex_coord;
varying vec3 o_frag_pos;
varying vec3 o_normal;
uniform vec3 u_cam_pos;
uniform sampler2D u_sampler;

void main() {
    vec3 color = texture2D(u_sampler, o_tex_coord).xyz;
    vec3 light_dir = vec3(1);
    
    float shading = max(0.0, dot(normalize(o_normal), normalize(light_dir)));
    gl_FragColor = vec4(color * shading, 1.0);
}


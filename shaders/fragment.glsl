precision mediump float;

varying vec3 o_frag_pos;
varying vec3 o_normal;
uniform vec3 u_cam_pos;

void main() {
    vec3 color = vec3(1.0, 0.8, 0.6);
    vec3 light_dir = vec3(1);
    
    float shading = max(0.0, dot(normalize(o_normal), normalize(light_dir)));
    gl_FragColor = vec4(color * shading, 1.0);
}


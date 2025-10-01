precision mediump float;

varying vec2 o_uv;
varying vec3 o_frag_pos;
varying vec3 o_normal;
varying mat3 o_tbn;

uniform vec3 u_cam_pos;
uniform sampler2D u_diffuse;
uniform sampler2D u_normal;

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
        color = texture2D(u_diffuse, o_uv).xyz;

    // Normals
    if (u_use_normal == 0) {
        normal = normalize(o_normal);
    }
    else {
        normal = texture2D(u_normal, o_uv).rgb;
        normal = normal * 2.0 - 1.0;   
        normal = o_tbn * normal; 
    }

    vec3 light_dir = vec3(1);
    
    float shading = max(0.0, dot(normalize(normal), normalize(light_dir)));
    gl_FragColor = vec4(color * shading, 1.0);
}


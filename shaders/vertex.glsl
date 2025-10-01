attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec3 a_tangent;
attribute vec3 a_bitangent;

varying vec2 o_uv;
varying vec3 o_frag_pos;
varying vec3 o_normal;
varying mat3 o_tbn;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat3 u_normal_matrix;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
    o_frag_pos = a_position;
    o_uv = a_uv;
    o_normal = u_normal_matrix * a_normal;
    o_tbn = mat3(
        vec3(normalize(u_model * vec4(a_tangent, 0.0))),
        vec3(normalize(u_model * vec4(a_bitangent, 0.0))),
        vec3(normalize(u_model * vec4(a_normal, 0.0)))
    );
}


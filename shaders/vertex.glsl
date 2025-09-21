attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec3 a_bitangent;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
}


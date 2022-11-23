#extension GL_EXT_gpu_shader4 : require

attribute highp vec4 a_position;
attribute highp vec2 a_texcoord;
attribute highp vec3 a_normal;

uniform highp mat4 u_projection_matrix;
uniform highp mat4 u_view_matrix;
uniform highp mat4 u_model_matrix;

varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;

flat varying vec3 v_normal_flat;

void main(void) {
    mat4 mv_matrix = u_view_matrix * u_model_matrix;

    gl_Position = u_projection_matrix * mv_matrix * a_position;

    v_texcoord = a_texcoord;
    v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_normal_flat = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_position = mv_matrix * a_position;
}

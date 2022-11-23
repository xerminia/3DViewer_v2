#extension GL_EXT_gpu_shader4 : require

uniform bool flag_texture;
uniform int flag_type;
uniform sampler2D u_texture;
uniform highp vec4 u_light_position;
uniform highp float u_light_power;
uniform highp vec4 u_light_color;
uniform highp vec3 u_eye_position;
uniform highp vec4 u_base_color;

varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;

flat varying vec3 v_normal_flat;

void main(void) {
    vec4 diff_material_color;
    vec4 diff_color;

    vec4 result_color = u_base_color;
    vec4 eye_position = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 eye_vect = normalize(v_position.xyz - eye_position.xyz); // вектор направления взгляда(v_pos - вектор где находится наблюдатель) // eye vector направлен из точки 0 0 0 в точку которую сейчас обрабатывает фрагментный шейдер

    if (flag_texture) {
        diff_material_color = texture2D(u_texture, v_texcoord);
    } else {
        diff_material_color = result_color;
    }

    float len = length(v_position.xyz - eye_position.xyz); // расстояние от наблюдателя до рассматриваемой точки
    vec3 light_vect = normalize(v_position.xyz - u_light_position.xyz); // вектор света (из точки где находится источ света к вектору позиции)

    if (flag_type == 1) {
        diff_color = diff_material_color * u_light_power * max(0.0, dot(v_normal_flat, -light_vect));
    } else if (flag_type == 2) {
        vec3 reflect_light = normalize(reflect(light_vect, v_normal)); // отраженный свет(вектор из точки которую мы рассматриваем в направлении отражения) на вход вектор направления света и нормаль
        float specular_factor = 70.0; // отвечает за размер пятна блика
        float ambient_factor = 0.5; // отвечает за силу свечения материала
        diff_color = diff_material_color * u_light_power * max(0.0, dot(v_normal, -light_vect)) / (1.0 + 0.25 * pow(len, 2.0)); // дифьюзный цвет
        vec4 ambient_color = ambient_factor * diff_material_color; // амбиентное освещение(составляющая)
        vec4 specular_color = u_light_color * u_light_power * pow(max(0.0, dot(reflect_light, -eye_vect)), specular_factor) / (1.0 + 0.25 * pow(len, 2.0)); // блики(цвет бликов)
        result_color += ambient_color + specular_color;
    }

    result_color += diff_color * u_light_color;

    gl_FragColor = result_color;
}


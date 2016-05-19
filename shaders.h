#define SHADER_VERT2D \
"#version 150 \n"\
"in  vec3 position; "\
"in  vec2 texcoord;"\
"out vec2 Texcoord;"\
"void main(void) { "\
"    Texcoord = texcoord;"\
"    gl_Position = vec4(position, 1.0);"\
"}"

#define SHADER_VERT3D \
"#version 150 \n"\
"in  vec3 position; "\
"in  vec2 texcoord;"\
"out vec2 Texcoord;"\
"uniform mat4 model;"\
"uniform mat4 view;"\
"uniform mat4 proj;"\
"void main(void) { "\
"    Texcoord = texcoord;"\
"    gl_Position = proj * view * model * vec4(position, 1.0);"\
"}"

#define SHADER_VERT3D_COLOR \
"#version 150 \n"\
"in  vec3 position; "\
"in  vec3 v_color;"\
"out vec3 f_color; "\
"uniform mat4 model;"\
"uniform mat4 view;"\
"uniform mat4 proj;"\
"void main(void) { "\
"    gl_Position = proj * view * model * vec4(position, 1.0);"\
"    f_color = v_color;"\
"}"

#define SHADER_FRAG_COLOR \
"#version 150\n"\
"in vec3 f_color; "\
"out vec4 outColor; "\
"void main() "\
"{ "\
"    outColor = vec4(f_color,1.0); "\
"}"

#define SHADER_FRAG_PRETTY \
"#version 150 core\n"\
"in  vec2 Texcoord;"\
"out vec4 outColor;"\
"uniform sampler2D tex;"\
"void main()"\
"{"\
"    outColor = texture(tex, Texcoord) * vec4(Texcoord.x, Texcoord.y, 1.0, 1.0);"\
"}"

#define SHADER_FRAG_SIMPLE \
"#version 150 core\n"\
"in  vec2 Texcoord;"\
"out vec4 outColor;"\
"uniform sampler2D tex;"\
"void main()"\
"{"\
"    outColor = texture(tex, Texcoord);"\
"}"

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

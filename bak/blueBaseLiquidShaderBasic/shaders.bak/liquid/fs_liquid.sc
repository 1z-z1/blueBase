$input v_texcoord0, v_pos

#include <bgfx_shader.sh>

uniform vec4 u_time;  // Add this uniform

void main()
{
    vec2 p = v_pos.xz * 2.0;
    float t = u_time.x * 0.5;
    
    float wave1 = sin(p.x * 2.0 + t) * 0.5 + 0.5;
    float wave2 = sin(p.y * 1.5 + t * 0.8) * 0.5 + 0.5;
    float waves = wave1 * wave2;
    
    vec3 color1 = vec3(0.0, 0.4, 0.8);
    vec3 color2 = vec3(0.0, 0.6, 1.0);
    vec3 finalColor = mix(color1, color2, waves);
    
    float highlight = pow(waves, 3.0) * 0.3;
    finalColor += vec3(highlight);
    
    gl_FragColor = vec4(finalColor, 0.9);
}
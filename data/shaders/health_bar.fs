varying vec2 v_uv;

uniform vec4 u_color;
uniform float u_mask;

void main()
{
    vec3 final_color;
    if(v_uv.x > u_mask){
        final_color = vec3(0.1,0.1, 0.1);
    }else{
        final_color = mix(vec3(1.0,0.3,0.3), vec3(0.3,1.0,0.3), u_mask);
    }

    gl_FragColor = vec4(final_color*u_color.rgb, 0.5);
}
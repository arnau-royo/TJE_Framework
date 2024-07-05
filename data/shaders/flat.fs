
varying vec4 v_color;

uniform vec4 u_color;
uniform vec3 u_Kd;

void main()
{
	gl_FragColor = u_color * vec4(u_Kd, 1.0);
}

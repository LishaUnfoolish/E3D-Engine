#Vertex_Begin

#Attribute
{
	POSITION:position;
	COLOR:color;
	TEXTURECOORD:inputTextureCoordinate;
	NORMAL:attr_normal;
}

varying vec3 ReflectDir;
varying vec4 vertColor;

varying vec4 vPos;
varying vec3 vNrm;
varying vec3 eyePosition;
varying vec2 v_Coord;
varying float time;
varying vec4 v_InLightPos;

varying vec3 lightDir;
varying vec4 lightColor;

const float _WaveScale = 0.1;
vec4 _2StreamVelo = vec4(-12.5, 42.7,-15.8,-0.34);
const int n = 3;
const int m = 2;
vec2 thetas = vec2(0.38,1.42);
mat3 amplitudes = mat3(
	vec3(0.2, 0.2,0),
	vec3(0.3, 0.50,0),
	vec3(0.2, 0.6,0)
);
 const vec3 omegas = vec3(3.27,3.31,3.42);
 const vec3 waveNums = vec3(1.091,1.118,1.1935);

const  mat4 biasMatrix = mat4(0.5 , 0.0 , 0.0 , 0.0 ,
                       0.0 , 0.5 , 0.0 , 0.0 ,
                       0.0 , 0.0 , 0.5 , 0.0 ,
                       0.5 , 0.5 , 0.5 , 1.0 ) ;
					   
mat4 getRotateMatrix()
{
	mat4 rotate;
	rotate[0] = _e3d_matModel[0];
	rotate[1] = _e3d_matModel[1];
	rotate[2] = _e3d_matModel[2];
	rotate[3] = vec4(0.0,0.0,0.0,1.0);
	return rotate;
}

void main(void)
{
	mat4 rotateMatrix = getRotateMatrix();
	vec4 _normal = rotateMatrix * vec4(attr_normal.xyz, 1.0);	
	eyePosition = _e3d_CameraPos.xyz;
	vPos =  _e3d_matModel * vec4(position, 1.0);; // 顶点位置
	vNrm = normalize(_normal.xyz); // 顶点法线
	//vec3 posi = gerstner(position.x, position.y ,position.z);
	
#ifdef USING_DIRECTIONAL_LIGHT
	lightDir = _e3d_WorldSpaceLightDirection;
	lightColor = _e3d_WorldSpaceLightColor;
	v_InLightPos = _e3d_lightMatProj * _e3d_lightMatView * vPos;
	v_InLightPos = biasMatrix* (v_InLightPos / v_InLightPos.w );
#else
	lightDir = vec3(0.0,0.0,0.0);
#endif
	time = _Time;
	vec4 _pos = _e3d_matModel * vec4(position, 1.0);
	
	v_Coord = inputTextureCoordinate;// * _WaveScale + (_2StreamVelo * time).xy ;//vPos.xzxz * _WaveScale + (_2StreamVelo * time) * 0.1;
	vertColor = color;
    gl_Position = _e3d_getMVPMatrix() * vec4(position, 1.0);
}

#Vertex_End

#Framgent_Begin
#ifdef __GLES__
precision highp float;
#endif
varying vec3 ReflectDir;
uniform sampler2D waterTex;
uniform sampler2D waterNormal;
uniform samplerCube skybox;
varying vec4 vertColor;

varying vec3 lightDir;
varying vec4 lightColor;
varying vec4 v_InLightPos;

varying vec4 vPos;
varying vec3 vNrm;
varying vec3 eyePosition;
varying vec2 v_Coord;
varying float time;

uniform float fresnelBias;
uniform float etaRatio;

const vec3  ambient = vec3(0.8, 0.8, 0.8);		//环境光颜色
const float Ns = 5.0;			//高光系数
const float attenuation = 1.0;	//光线的衰减系数

const float _BumpDepth = 1.0;
const float _ReflScale = 3.598;
const float fresnelScale = 0.1;
const float fresnelPower = 5.0;

float near_clip = 1.0;
float ampify = 5.0;
float ks = 1.0;

vec4 lerp(vec4 a, vec4 b, float s)
{
	return vec4(a+(b-a)*s);
}


vec4 getLightColor(vec3 position, vec3 normal)
{
	//--- 光照 Blinn Phong 模型
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	vec3 V = normalize(eyePosition - position);
	vec3 H = normalize(V + L);
	vec3 diffuse = vec3((lightColor * max(dot(N, L), 0.0)).xyz);
	
	// vec3 R=reflect(L,N);
	// R=normalize(R);
	// float specularLight = pow(max(dot(V, R), 0), Ns);
	// vec3 specular = (attenuation * lightColor * specularLight).xyz;
	
	vec3 specular = vec3((lightColor * pow(max(dot(N, H), 0.0), Ns) * attenuation).xyz);

	return vec4(diffuse + specular + ambient, 1.0);
}


vec4 FresnelShading(vec3 normal)
{
	// 计算入射，反射，折射
	vec3 N = normalize(normal); // 法线
	vec3 I = normalize(vPos.xyz-eyePosition); // 入射
	vec3 R = reflect(I, N); // 反射
	vec3 T = refract(I, N, etaRatio); // 折射
	// 反射因子计算
	float fresnel = fresnelBias+fresnelScale*pow(min(0.0, 1.0-dot(I, N)), fresnelPower);
	// 获得反射环境色
	vec4 reflecColor = textureCube(skybox, R);
	reflecColor.a = 1.0;
	// 计算折射环境色
	vec4 refracColor;
	refracColor.rgb = textureCube(skybox, T).rgb;
	refracColor.a = 1.0;
	// 颜色合成
	vec4 cout = lerp(refracColor, reflecColor, fresnel);
	cout.a = fresnel*0.5+0.5;
	
	return cout;
}

float getShadowColor(vec4 pos, float bias)
{
	float shadowColor = 1.0;
#ifndef __GLES__
#ifdef __CREATE_SHADOW__
	vec2 textSize = 1.0/ textureSize(_e3d_lightDepthTex, 0);
 	float depth = texture2D(_e3d_lightDepthTex, pos.xy).r;
	
	if(pos.z - bias > depth) // 
	{
		shadowColor = 0.5;
	}	
	else
	{
		vec4 coord1 = vec4(clamp(pos.xy - textSize, 0.0, 1.0),pos.z, pos.w );		
		vec4 coord2 = vec4(clamp(pos.xy + textSize, 0.0, 1.0),pos.z, pos.w );
		vec4 coord3 = vec4(clamp(pos.x -  textSize.x, 0.0, 1.0), clamp(pos.y + textSize.y, 0.0, 1.0) ,pos.z, pos.w );
		vec4 coord4 = vec4(clamp(pos.x +  textSize.x, 0.0, 1.0), clamp(pos.y - textSize.y, 0.0, 1.0) ,pos.z, pos.w );
		
		float depth1 =  texture2D(_e3d_lightDepthTex,coord1.xy).r;
		float depth2 =  texture2D(_e3d_lightDepthTex,coord2.xy).r;
		float depth3 =  texture2D(_e3d_lightDepthTex,coord3.xy).r;
		float depth4 =  texture2D(_e3d_lightDepthTex,coord4.xy).r;
		
		depth1 = coord1.z - bias > depth1 ?  0.5 : 1.0; 
		depth2 = coord2.z - bias > depth2 ?  0.5 : 1.0; 
		depth3 = coord3.z - bias > depth3 ?  0.5 : 1.0; 
		depth4 = coord4.z - bias > depth4 ?  0.5 : 1.0; 
		
		float mulColor = depth1* depth2 * depth3 * depth4;
		if (mulColor < 1.0)
		{
			shadowColor = (depth1+ depth2 + depth3 + depth4 ) / 4.0 * 0.9;
		}
	}
#endif
#endif
	return shadowColor;
}

void main(void) 
{ 
	float yV = mod(v_Coord.y * 10.0 + time * 0.03, 10.0);
	float xV = mod( 10.0 + v_Coord.x * 10.0 - time * 0.02, 10.0);
	
	vec2 coord = vec2(v_Coord.x * 10.0, yV);
	vec2 coord1 = vec2(v_Coord.y * 10.0, xV);
	
	vec3 bump1 = texture2D( waterNormal, coord).rbg * 2.0 - 1.0;
	vec3 bump2 = texture2D( waterNormal, coord1).rbg * 2.0 - 1.0;
	
	vec3 normal =  normalize(vec3(bump1.x, _BumpDepth, bump1.z)); // 扰动法线
	vec3 normal2 =  normalize(vec3(bump2.x, _BumpDepth, bump2.z)); // 扰动法线
	
	vec3 normalVec = vec3(normal.x, normal.z, -normal.y); // 旋转法线到xz平面
	
	vec4 _lightColor = getLightColor(vPos.xyz, normalVec.xyz);
	vec4 freColor1 = FresnelShading(normalVec) * 0.5;
	vec4 freColor2 = FresnelShading(vNrm)  * 0.8;
	vec4 freColor3 = FresnelShading(vec3(normal2.x, normal2.z, -normal2.y)) * 0.5;	
	float sC = getShadowColor(v_InLightPos, 0.0);
	gl_FragColor = vec4(((freColor1  + freColor2 + freColor3) * _lightColor * vertColor * sC).rgb, 0.6) ;
}

#Framgent_End


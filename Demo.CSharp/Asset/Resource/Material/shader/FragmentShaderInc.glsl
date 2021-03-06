const float r1 = 1.0;
const float r2 = 1.0 / 256.0;
const float r3 = 1.0 / (256.0 * 256.0);
const float r4 = 1.0 / (256.0 * 256.0 * 256.0);

varying vec3 vlightDir;
varying vec4 vlightColor;
varying vec3 vCameraPos;
varying vec4 vShadow_Pos;

float unpackDepth(const in vec4 rgbaDepth) 
{
    const vec4 bitShift = vec4(r1, r2, r3, r4);
    float depth = dot(rgbaDepth, bitShift);
    return depth;
}

float texture2DCompare(sampler2D depths, vec2 uv, float compare, float bias)
{
    float depth = unpackDepth(texture2D(depths, uv));
    return step(compare,depth);
}

vec4 mixFogColor(vec4 _in_fragColor, vec4 _in_FogColor,vec3 position)
{
	float dist = abs(distance(vCameraPos, position));
	float maxDist = 1500.0;
	float minDist = 1.0;
    float fogFactor = (maxDist - dist) / (maxDist - minDist);  
    fogFactor = clamp( fogFactor, 0.0, 1.0 );  
	
    vec4 mColor = mix( _in_FogColor, _in_fragColor, fogFactor );  
  
    return mColor;
}

float texture2DShadowLerp(sampler2D depths, vec2 uv, float compare, float bias)
{
    float size = textureSize(depths, 0).x;
	vec2 texelSize = vec2(1.0) / vec2(size, size);
    vec2 centroidUV = floor(uv * size + 0.5) / size;
    vec2 f = fract(uv * size + 0.5);
    float lb = texture2DCompare(depths, centroidUV + texelSize * vec2(0.0, 0.0), compare ,bias);
    float lt = texture2DCompare(depths, centroidUV + texelSize * vec2(0.0, 1.0), compare ,bias);
    float rb = texture2DCompare(depths, centroidUV + texelSize * vec2(1.0, 0.0), compare ,bias);
    float rt = texture2DCompare(depths, centroidUV + texelSize * vec2(1.0, 1.0), compare ,bias);
    float a = mix(lb, lt, f.y);
    float b = mix(rb, rt, f.y);
    float c = mix(a, b, f.x);
    return c;
}

float PCFLerp(sampler2D depths, vec2 uv, float compare, float bias) 
{	
	vec2 textSize = 1.0/ textureSize(depths, 0);
    float result = 0.0;
    for(int x = -1; x <= 1; x++)
	{
        for(int y = -1; y <= 1; y++)
		{
            vec2 off = textSize * vec2(x,y);
            result += texture2DShadowLerp(depths, uv + off, compare, bias);
        }
    }
    return result / 9.0;
}

float getShadowColor(float bias)
{
	float shadowColor = 1.0;
#ifndef __GLES__
#ifdef __CREATE_SHADOW__
	shadowColor = clamp(PCFLerp(_e3d_lightDepthTex, vShadow_Pos.xy, vShadow_Pos.z + bias , bias) + 0.3, 0.3, 1.0);
#endif
#endif
	return shadowColor;
}

// vec4 getPointLightColor(vec3 position, vec3 normal, float ns)
// {
// 	vec4 lightColor = vec4(0.0,0.0,0.0,1.0);
	
// #ifdef USING_POINT_LIGHT
// 	vec3 N = normalize((vec4(normal, 1.0)).xyz);
// 	vec3 V = normalize(vCameraPos - position);
// 	for (int i = 0; i < _e3d_PointLightCount; i++)
// 	{
// 		if(distance(_e3d_PointLightPos[i] , position) < _e3d_PointLightRange[i])
// 		{
// 			float d = distance(_e3d_PointLightPos[i] , position);
// 			float attenuate = 1.0 /(Kc + Kl*d + KQ * d * d); //衰减因子（由公式计算）
// 			vec3 L = normalize(_e3d_PointLightPos[i] - position);
// 			vec3 H = normalize(V + L);
// 			vec3 diffuse = vec3((_e3d_PointLightColor[i] * max(dot(N, L), 0.0)).xyz);
// 			vec3 specular = (_e3d_PointLightColor[i] * pow(max(dot(N, H), 0.0), ns) * attenuate).xyz;
			
// 			lightColor = lightColor + vec4(clamp((diffuse + specular), 0.0, 1.0), 1.0);
// 		}
// 	}
// #endif
// 	return lightColor;
// }

vec4 getLightColor(vec3 position, vec3 normal, float ns, float shadowFactor)
{
    const vec4  ambient = vec4(0.3, 0.3, 0.3, 1.0);		//环境光颜色
	const float PI = 3.141592653;
#ifdef USING_DIRECTIONAL_LIGHT
	//--- 光照
	vec3 N = normalize(normal);
	vec3 L = normalize(vlightDir);
	vec3 V = normalize(vCameraPos - position);
	vec3 H = normalize(V + L);
	vec4 diffuse = vlightColor;
	vec4 specular = vlightColor * ((ns + 8.0) / (8 * PI)) * pow(max(dot(N, H), 0.0), ns);
	vec4 lightColor = (diffuse + specular) * max(dot(N, L), 0.0);//vec4(clamp((diffuse + specular), 0.0, 1.0), 1.0);
#endif
	return vec4((lightColor  * shadowFactor + ambient).rgb, 1.0);
}

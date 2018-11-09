
varying vec3 vPosition;
varying vec3 mCameraPos;

const vec4  ambient = vec4(0.8, 0.8, 0.8, 1.0);		//环境光颜色
const float Ns = 1.0;			//高光系数
const float attenuation = 1.0;	//光线的衰减系数

void initFogNeedVar(vec3 _pos)
{
	vPosition = (_e3d_matModel * vec4(_pos ,1.0)).xyz;
	mCameraPos = _e3d_CameraPos;
}

vec4 getPointLightColor(vec3 position, vec3 normal)
{
	vec4 lightColor = vec4(0.0,0.0,0.0,1.0);
	
#ifdef USING_POINT_LIGHT
	vec3 N = normalize((vec4(normal, 1.0)).xyz);
	vec3 V = normalize(_e3d_CameraPos - position);
	for (int i = 0; i < _e3d_PointLightCount; i++)
	{
		if(distance(_e3d_PointLightPos[i] , position) < _e3d_PointLightRange[i])
		{
			vec3 L = normalize(_e3d_PointLightPos[i] - position);
			vec3 H = normalize(V + L);
			vec3 diffuse = vec3((_e3d_PointLightColor[i] * max(dot(N, L), 0.0)).xyz);
			vec3 specular = vec3((_e3d_PointLightColor[i] * pow(max(dot(N, H), 0.0), Ns) * attenuation).xyz);
			
			lightColor = lightColor + vec4(clamp((diffuse + specular), 0.0, 1.0), 1.0);
		}
	}
#endif
	return lightColor;
}

vec4 getLightColor(vec3 position, vec3 normal)
{
	vec4 lightColor = vec4(0.0,0.0,0.0,1.0);
#ifdef USING_DIRECTIONAL_LIGHT
	//--- 光照
	vec3 N = normalize(normal);
	vec3 L = normalize(_e3d_WorldSpaceLightDirection);
	vec3 V = normalize(_e3d_CameraPos - position);
	vec3 H = normalize(V + L);
	vec3 diffuse = vec3((_e3d_WorldSpaceLightColor * max(dot(N, L), 0.0)).xyz);
	vec3 specular = vec3((_e3d_WorldSpaceLightColor * pow(max(dot(N, H), 0.0), Ns) * attenuation).xyz);
	lightColor = vec4(clamp((diffuse + specular), 0.0, 1.0), 1.0);
#endif
#ifdef USING_POINT_LIGHT
	lightColor = lightColor + getPointLightColor(position, normal);
#endif
	return (lightColor + ambient);
}


mat4 getRotateMatrix()
{
	mat4 rotate;
	rotate[0] = _e3d_matModel[0];
	rotate[1] = _e3d_matModel[1];
	rotate[2] = _e3d_matModel[2];
	rotate[3] = vec4(0.0,0.0,0.0,1.0);
	return rotate;
}

mat4 getBillboardMatrix(vec3 position, vec3 worldCameraPosition, vec3 worldCameraUp)
{
    vec3 forward = normalize(position - worldCameraPosition);
    vec3 side = cross( forward, worldCameraUp );
    
    mat4 result;
    result[0] = vec4( side, 0.0 );
    result[1] = vec4( worldCameraUp, 0.0 );
    result[2] = vec4( forward.x, 0.0, forward.z, 0.0 );
    result[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
    return result;
}

mat4 getTransformRotateMatrix(vec3 transformRotate)
{
	mat4 roateMatX	= mat4(1.0, 0.0,						0.0,					0.0,
						   0.0, cos(transformRotate.x),	sin(transformRotate.x),		0.0,
						   0.0, -sin(transformRotate.x),	cos(transformRotate.x),	0.0,
						   0.0, 0.0,						0.0, 1.0);
	
	mat4 roateMatY	= mat4(cos(transformRotate.y),	0.0, -sin(transformRotate.y),	0.0,
						   0.0,						1.0, 0.0,						0.0,
						   sin(transformRotate.y),	0.0, cos(transformRotate.y),	0.0,
						   0.0,						0.0, 0.0,						1.0);
	
	mat4 roateMatZ	= mat4(cos(transformRotate.z) ,	sin(transformRotate.z),	0.0, 0.0,
						   -sin(transformRotate.z),	cos(transformRotate.z),	0.0, 0.0,
						   0.0,						0.0,					1.0, 0.0,
						   0.0,						0.0,					0.0, 1.0);
	return roateMatX * roateMatY * roateMatZ;
}

mat4 getTransformMatrix(vec3 transformPosition, vec3 transformRotate, vec3 transformScale)
{
	mat4 translate	= mat4(1.0,					0.0,					0.0,					0.0,
						   0.0,					1.0,					0.0,					0.0,
						   0.0,					0.0,					1.0,					0.0,
						   transformPosition.x,	transformPosition.y,	transformPosition.z,	1.0);
	
	
	mat4 scaleMat	= mat4(transformScale.x	, 0.0			  ,		0.0				,	0.0,
						   0.0				, transformScale.y,		0.0				,	0.0,
						   0.0				, 0.0			  ,		transformScale.z,	0.0,
						   0.0				, 0.0			  ,		0.0				,	1.0
						   );
	
	mat4 roateMatrix = getTransformRotateMatrix(transformRotate);
	
	return translate * roateMatrix * scaleMat;
}
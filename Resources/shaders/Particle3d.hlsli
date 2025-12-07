
struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
	//xy : uv, z : instanceID
	float32_t3 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
    float32_t3 worldPosition : POSITION0;
    float32_t4 color : COLOR0;
};

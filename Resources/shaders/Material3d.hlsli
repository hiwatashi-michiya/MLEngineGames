
struct Material
{
	
    float32_t4 color;
    
    int32_t enableLighting;
    int32_t enableNormalMap;
    float32_t shininess;
    
    float32_t4x4 uvTransform;
    
};
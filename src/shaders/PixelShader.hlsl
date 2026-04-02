  struct PSInput {
      float4 pos    : SV_Position;
      float4 normal : NORMAL;
      float4 color  : COLOR;
  };

  float4 main(PSInput input) : SV_Target {
      float3 lightDir = normalize(float3(1.0, 1.0, -1.0));
      float3 normal   = normalize(input.normal.xyz);
      float  diffuse  = max(0.0, dot(normal, lightDir));
      float  ambient  = 0.1;
      return input.color * (ambient + diffuse);
  }

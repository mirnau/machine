struct VSInput {
  float4 pos   : POSITION;
  float4 color : COLOR;
};

struct VSOutput {
  float4 pos   : SV_Position;
  float4 color : COLOR;
};

VSOutput main(VSInput input) {
  VSOutput output;
  output.pos   = input.pos;
  output.color = input.color;
  return output;
}

struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t4 color : COLOR0; // 色（RGBA）
    float32_t2 texcoord : TEXCOORD0;
    uint layer : TEXCOORD1; // Texture2DArrayのレイヤー
    uint isTemp : TEXCOORD2; // 仮アトラスかどうか（0か1）
};
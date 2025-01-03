#include <metal_stdlib>
#include <metal_math>
#include <metal_texture>
using namespace metal;

#line 6 "tri.vert.slang"
struct Output_0
{
    float4 Color_0 [[user(TEXCOORD)]];
    float4 Position_0 [[position]];
};

[[vertex]] Output_0 start(uint VertexIndex_0 [[vertex_id]])
{
    thread Output_0 output_0;

#line 14
    float2 pos_0;

    if(VertexIndex_0 == 0U)
    {

#line 16
        float2 _S1 = float2(-1.0) ;


        (&output_0)->Color_0 = float4(1.0, 0.0, 0.0, 1.0);

#line 19
        pos_0 = _S1;

#line 16
    }
    else
    {

#line 23
        if(VertexIndex_0 == 1U)
        {
            float2 _S2 = float2(1.0, -1.0);
            (&output_0)->Color_0 = float4(0.0, 1.0, 0.0, 1.0);

#line 26
            pos_0 = _S2;

#line 23
        }
        else
        {

#line 30
            if(VertexIndex_0 == 2U)
            {
                float2 _S3 = float2(0.0, 1.0);
                (&output_0)->Color_0 = float4(0.0, 0.0, 1.0, 1.0);

#line 33
                pos_0 = _S3;

#line 30
            }

#line 23
        }

#line 16
    }

#line 37
    (&output_0)->Position_0 = float4(pos_0, 0.0, 1.0);
    return output_0;
}


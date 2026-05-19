; SPIR-V
; Version: 1.3
; Generator: Khronos Slang Compiler; 0
; Bound: 171
; Schema: 0
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint Vertex %vertexMain "main" %entryPointParam_vertexMain_position %entryPointParam_vertexMain_texCoord %entryPointParam_vertexMain_normal %entryPointParam_vertexMain_tangeant %entryPointParam_vertexMain_bitangeant %entryPointParam_vertexMain_texIndex %inVert_position %inVert_texCoord %inVert_normal %inVert_tangeant %inVert_bitangeant %inInst_modelMat %inInst_index
               OpSource Slang 1
               OpName %inVert_position "inVert.position"
               OpName %inVert_texCoord "inVert.texCoord"
               OpName %inVert_normal "inVert.normal"
               OpName %inVert_tangeant "inVert.tangeant"
               OpName %inVert_bitangeant "inVert.bitangeant"
               OpName %_MatrixStorage_float4x4_ColMajornatural "_MatrixStorage_float4x4_ColMajornatural"
               OpMemberName %_MatrixStorage_float4x4_ColMajornatural 0 "data"
               OpName %inInst_modelMat "inInst.modelMat"
               OpName %inInst_index "inInst.index"
               OpName %_MatrixStorage_float4x4_ColMajorstd140 "_MatrixStorage_float4x4_ColMajorstd140"
               OpMemberName %_MatrixStorage_float4x4_ColMajorstd140 0 "data"
               OpName %UniformBufferObject_std140 "UniformBufferObject_std140"
               OpMemberName %UniformBufferObject_std140 0 "viewMat"
               OpMemberName %UniformBufferObject_std140 1 "projMat"
               OpMemberName %UniformBufferObject_std140 2 "lightPos"
               OpMemberName %UniformBufferObject_std140 3 "padding"
               OpMemberName %UniformBufferObject_std140 4 "lightCol"
               OpMemberName %UniformBufferObject_std140 5 "padding2"
               OpName %ubo "ubo"
               OpName %entryPointParam_vertexMain_position "entryPointParam_vertexMain.position"
               OpName %entryPointParam_vertexMain_texCoord "entryPointParam_vertexMain.texCoord"
               OpName %entryPointParam_vertexMain_normal "entryPointParam_vertexMain.normal"
               OpName %entryPointParam_vertexMain_tangeant "entryPointParam_vertexMain.tangeant"
               OpName %entryPointParam_vertexMain_bitangeant "entryPointParam_vertexMain.bitangeant"
               OpName %entryPointParam_vertexMain_texIndex "entryPointParam_vertexMain.texIndex"
               OpName %vertexMain "vertexMain"
               OpDecorate %inVert_position Location 0
               OpDecorate %inVert_texCoord Location 1
               OpDecorate %inVert_normal Location 2
               OpDecorate %inVert_tangeant Location 3
               OpDecorate %inVert_bitangeant Location 4
               OpDecorate %_arr_v4float_int_4 ArrayStride 16
               OpMemberDecorate %_MatrixStorage_float4x4_ColMajornatural 0 Offset 0
               OpDecorate %inInst_modelMat Location 5
               OpDecorate %inInst_index Location 9
               OpMemberDecorate %_MatrixStorage_float4x4_ColMajorstd140 0 Offset 0
               OpDecorate %UniformBufferObject_std140 Block
               OpMemberDecorate %UniformBufferObject_std140 0 Offset 0
               OpMemberDecorate %UniformBufferObject_std140 1 Offset 64
               OpMemberDecorate %UniformBufferObject_std140 2 Offset 128
               OpMemberDecorate %UniformBufferObject_std140 3 Offset 140
               OpMemberDecorate %UniformBufferObject_std140 4 Offset 144
               OpMemberDecorate %UniformBufferObject_std140 5 Offset 156
               OpDecorate %ubo Binding 0
               OpDecorate %ubo DescriptorSet 0
               OpDecorate %entryPointParam_vertexMain_position Location 0
               OpDecorate %entryPointParam_vertexMain_texCoord Location 1
               OpDecorate %entryPointParam_vertexMain_normal Location 2
               OpDecorate %entryPointParam_vertexMain_tangeant Location 3
               OpDecorate %entryPointParam_vertexMain_bitangeant Location 4
               OpDecorate %entryPointParam_vertexMain_texIndex Location 5
       %void = OpTypeVoid
          %3 = OpTypeFunction %void
      %float = OpTypeFloat 32
    %v4float = OpTypeVector %float 4
    %v2float = OpTypeVector %float 2
    %v3float = OpTypeVector %float 3
        %int = OpTypeInt 32 1
%_ptr_Input_v3float = OpTypePointer Input %v3float
%_ptr_Input_v2float = OpTypePointer Input %v2float
      %int_4 = OpConstant %int 4
%_arr_v4float_int_4 = OpTypeArray %v4float %int_4
%_MatrixStorage_float4x4_ColMajornatural = OpTypeStruct %_arr_v4float_int_4
%_ptr_Input__MatrixStorage_float4x4_ColMajornatural = OpTypePointer Input %_MatrixStorage_float4x4_ColMajornatural
%mat4v4float = OpTypeMatrix %v4float 4
%_ptr_Input_int = OpTypePointer Input %int
%_MatrixStorage_float4x4_ColMajorstd140 = OpTypeStruct %_arr_v4float_int_4
%UniformBufferObject_std140 = OpTypeStruct %_MatrixStorage_float4x4_ColMajorstd140 %_MatrixStorage_float4x4_ColMajorstd140 %v3float %float %v3float %float
%_ptr_Uniform_UniformBufferObject_std140 = OpTypePointer Uniform %UniformBufferObject_std140
      %int_0 = OpConstant %int 0
%_ptr_Uniform__MatrixStorage_float4x4_ColMajorstd140 = OpTypePointer Uniform %_MatrixStorage_float4x4_ColMajorstd140
      %int_1 = OpConstant %int 1
    %float_1 = OpConstant %float 1
%_ptr_Output_v4float = OpTypePointer Output %v4float
%_ptr_Output_v2float = OpTypePointer Output %v2float
%_ptr_Output_v3float = OpTypePointer Output %v3float
%_ptr_Output_int = OpTypePointer Output %int
%inVert_position = OpVariable %_ptr_Input_v3float Input
%inVert_texCoord = OpVariable %_ptr_Input_v2float Input
%inVert_normal = OpVariable %_ptr_Input_v3float Input
%inVert_tangeant = OpVariable %_ptr_Input_v3float Input
%inVert_bitangeant = OpVariable %_ptr_Input_v3float Input
%inInst_modelMat = OpVariable %_ptr_Input__MatrixStorage_float4x4_ColMajornatural Input
%inInst_index = OpVariable %_ptr_Input_int Input
        %ubo = OpVariable %_ptr_Uniform_UniformBufferObject_std140 Uniform
%entryPointParam_vertexMain_position = OpVariable %_ptr_Output_v4float Output
%entryPointParam_vertexMain_texCoord = OpVariable %_ptr_Output_v2float Output
%entryPointParam_vertexMain_normal = OpVariable %_ptr_Output_v3float Output
%entryPointParam_vertexMain_tangeant = OpVariable %_ptr_Output_v3float Output
%entryPointParam_vertexMain_bitangeant = OpVariable %_ptr_Output_v3float Output
%entryPointParam_vertexMain_texIndex = OpVariable %_ptr_Output_int Output
 %vertexMain = OpFunction %void None %3
          %4 = OpLabel
         %13 = OpLoad %v3float %inVert_position
         %16 = OpLoad %v2float %inVert_texCoord
         %19 = OpLoad %v3float %inVert_normal
         %21 = OpLoad %v3float %inVert_tangeant
         %23 = OpLoad %v3float %inVert_bitangeant
         %28 = OpLoad %_MatrixStorage_float4x4_ColMajornatural %inInst_modelMat
         %31 = OpCompositeExtract %_arr_v4float_int_4 %28 0
         %32 = OpCompositeExtract %v4float %31 0
         %33 = OpCompositeExtract %float %32 0
         %34 = OpCompositeExtract %float %32 1
         %35 = OpCompositeExtract %float %32 2
         %36 = OpCompositeExtract %float %32 3
         %37 = OpCompositeExtract %v4float %31 1
         %38 = OpCompositeExtract %float %37 0
         %39 = OpCompositeExtract %float %37 1
         %40 = OpCompositeExtract %float %37 2
         %41 = OpCompositeExtract %float %37 3
         %42 = OpCompositeExtract %v4float %31 2
         %43 = OpCompositeExtract %float %42 0
         %44 = OpCompositeExtract %float %42 1
         %45 = OpCompositeExtract %float %42 2
         %46 = OpCompositeExtract %float %42 3
         %47 = OpCompositeExtract %v4float %31 3
         %48 = OpCompositeExtract %float %47 0
         %49 = OpCompositeExtract %float %47 1
         %50 = OpCompositeExtract %float %47 2
         %51 = OpCompositeExtract %float %47 3
         %52 = OpCompositeConstruct %v4float %33 %38 %43 %48
         %53 = OpCompositeConstruct %v4float %34 %39 %44 %49
         %54 = OpCompositeConstruct %v4float %35 %40 %45 %50
         %55 = OpCompositeConstruct %v4float %36 %41 %46 %51
         %57 = OpCompositeConstruct %mat4v4float %52 %53 %54 %55
         %58 = OpLoad %int %inInst_index
         %67 = OpAccessChain %_ptr_Uniform__MatrixStorage_float4x4_ColMajorstd140 %ubo %int_0
         %68 = OpLoad %_MatrixStorage_float4x4_ColMajorstd140 %67
         %69 = OpCompositeExtract %_arr_v4float_int_4 %68 0
         %70 = OpCompositeExtract %v4float %69 0
         %71 = OpCompositeExtract %float %70 0
         %72 = OpCompositeExtract %float %70 1
         %73 = OpCompositeExtract %float %70 2
         %74 = OpCompositeExtract %float %70 3
         %75 = OpCompositeExtract %v4float %69 1
         %76 = OpCompositeExtract %float %75 0
         %77 = OpCompositeExtract %float %75 1
         %78 = OpCompositeExtract %float %75 2
         %79 = OpCompositeExtract %float %75 3
         %80 = OpCompositeExtract %v4float %69 2
         %81 = OpCompositeExtract %float %80 0
         %82 = OpCompositeExtract %float %80 1
         %83 = OpCompositeExtract %float %80 2
         %84 = OpCompositeExtract %float %80 3
         %85 = OpCompositeExtract %v4float %69 3
         %86 = OpCompositeExtract %float %85 0
         %87 = OpCompositeExtract %float %85 1
         %88 = OpCompositeExtract %float %85 2
         %89 = OpCompositeExtract %float %85 3
         %90 = OpCompositeConstruct %v4float %71 %76 %81 %86
         %91 = OpCompositeConstruct %v4float %72 %77 %82 %87
         %92 = OpCompositeConstruct %v4float %73 %78 %83 %88
         %93 = OpCompositeConstruct %v4float %74 %79 %84 %89
         %94 = OpCompositeConstruct %mat4v4float %90 %91 %92 %93
         %95 = OpMatrixTimesMatrix %mat4v4float %94 %57
         %97 = OpAccessChain %_ptr_Uniform__MatrixStorage_float4x4_ColMajorstd140 %ubo %int_1
         %98 = OpLoad %_MatrixStorage_float4x4_ColMajorstd140 %97
         %99 = OpCompositeExtract %_arr_v4float_int_4 %98 0
        %100 = OpCompositeExtract %v4float %99 0
        %101 = OpCompositeExtract %float %100 0
        %102 = OpCompositeExtract %float %100 1
        %103 = OpCompositeExtract %float %100 2
        %104 = OpCompositeExtract %float %100 3
        %105 = OpCompositeExtract %v4float %99 1
        %106 = OpCompositeExtract %float %105 0
        %107 = OpCompositeExtract %float %105 1
        %108 = OpCompositeExtract %float %105 2
        %109 = OpCompositeExtract %float %105 3
        %110 = OpCompositeExtract %v4float %99 2
        %111 = OpCompositeExtract %float %110 0
        %112 = OpCompositeExtract %float %110 1
        %113 = OpCompositeExtract %float %110 2
        %114 = OpCompositeExtract %float %110 3
        %115 = OpCompositeExtract %v4float %99 3
        %116 = OpCompositeExtract %float %115 0
        %117 = OpCompositeExtract %float %115 1
        %118 = OpCompositeExtract %float %115 2
        %119 = OpCompositeExtract %float %115 3
        %120 = OpCompositeConstruct %v4float %101 %106 %111 %116
        %121 = OpCompositeConstruct %v4float %102 %107 %112 %117
        %122 = OpCompositeConstruct %v4float %103 %108 %113 %118
        %123 = OpCompositeConstruct %v4float %104 %109 %114 %119
        %124 = OpCompositeConstruct %mat4v4float %120 %121 %122 %123
        %125 = OpMatrixTimesMatrix %mat4v4float %124 %95
        %128 = OpCompositeConstruct %v4float %13 %float_1
        %130 = OpMatrixTimesVector %v4float %125 %128
        %134 = OpVectorShuffle %v2float %16 %16 0 1
               OpStore %entryPointParam_vertexMain_position %130
               OpStore %entryPointParam_vertexMain_texCoord %134
               OpStore %entryPointParam_vertexMain_normal %19
               OpStore %entryPointParam_vertexMain_tangeant %21
               OpStore %entryPointParam_vertexMain_bitangeant %23
               OpStore %entryPointParam_vertexMain_texIndex %58
               OpReturn
               OpFunctionEnd

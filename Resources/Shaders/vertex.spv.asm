; SPIR-V
; Version: 1.3
; Generator: Khronos Slang Compiler; 0
; Bound: 164
; Schema: 0
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint Vertex %vertexMain "main" %entryPointParam_vertexMain_position %entryPointParam_vertexMain_texCoord %entryPointParam_vertexMain_tangeant %entryPointParam_vertexMain_bitangeant %entryPointParam_vertexMain_texIndex %inVert_position %inVert_texCoord %inInst_modelMat %inInst_index
               OpSource Slang 1
               OpName %inVert_position "inVert.position"
               OpName %inVert_texCoord "inVert.texCoord"
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
               OpName %entryPointParam_vertexMain_tangeant "entryPointParam_vertexMain.tangeant"
               OpName %entryPointParam_vertexMain_bitangeant "entryPointParam_vertexMain.bitangeant"
               OpName %entryPointParam_vertexMain_texIndex "entryPointParam_vertexMain.texIndex"
               OpName %vertexMain "vertexMain"
               OpDecorate %inVert_position Location 8
               OpDecorate %inVert_texCoord Location 9
               OpDecorate %_arr_v4float_int_4 ArrayStride 16
               OpMemberDecorate %_MatrixStorage_float4x4_ColMajornatural 0 Offset 0
               OpDecorate %inInst_modelMat Location 3
               OpDecorate %inInst_index Location 7
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
               OpDecorate %entryPointParam_vertexMain_tangeant Location 2
               OpDecorate %entryPointParam_vertexMain_bitangeant Location 3
               OpDecorate %entryPointParam_vertexMain_texIndex Location 4
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
%inInst_modelMat = OpVariable %_ptr_Input__MatrixStorage_float4x4_ColMajornatural Input
%inInst_index = OpVariable %_ptr_Input_int Input
        %ubo = OpVariable %_ptr_Uniform_UniformBufferObject_std140 Uniform
%entryPointParam_vertexMain_position = OpVariable %_ptr_Output_v4float Output
%entryPointParam_vertexMain_texCoord = OpVariable %_ptr_Output_v2float Output
%entryPointParam_vertexMain_tangeant = OpVariable %_ptr_Output_v3float Output
%entryPointParam_vertexMain_bitangeant = OpVariable %_ptr_Output_v3float Output
%entryPointParam_vertexMain_texIndex = OpVariable %_ptr_Output_int Output
%_ptr_Function_v3float = OpTypePointer Function %v3float
 %vertexMain = OpFunction %void None %3
          %4 = OpLabel
        %158 = OpVariable %_ptr_Function_v3float Function
        %157 = OpVariable %_ptr_Function_v3float Function
         %13 = OpLoad %v3float %inVert_position
         %16 = OpLoad %v2float %inVert_texCoord
         %22 = OpLoad %_MatrixStorage_float4x4_ColMajornatural %inInst_modelMat
         %25 = OpCompositeExtract %_arr_v4float_int_4 %22 0
         %26 = OpCompositeExtract %v4float %25 0
         %27 = OpCompositeExtract %float %26 0
         %28 = OpCompositeExtract %float %26 1
         %29 = OpCompositeExtract %float %26 2
         %30 = OpCompositeExtract %float %26 3
         %31 = OpCompositeExtract %v4float %25 1
         %32 = OpCompositeExtract %float %31 0
         %33 = OpCompositeExtract %float %31 1
         %34 = OpCompositeExtract %float %31 2
         %35 = OpCompositeExtract %float %31 3
         %36 = OpCompositeExtract %v4float %25 2
         %37 = OpCompositeExtract %float %36 0
         %38 = OpCompositeExtract %float %36 1
         %39 = OpCompositeExtract %float %36 2
         %40 = OpCompositeExtract %float %36 3
         %41 = OpCompositeExtract %v4float %25 3
         %42 = OpCompositeExtract %float %41 0
         %43 = OpCompositeExtract %float %41 1
         %44 = OpCompositeExtract %float %41 2
         %45 = OpCompositeExtract %float %41 3
         %46 = OpCompositeConstruct %v4float %27 %32 %37 %42
         %47 = OpCompositeConstruct %v4float %28 %33 %38 %43
         %48 = OpCompositeConstruct %v4float %29 %34 %39 %44
         %49 = OpCompositeConstruct %v4float %30 %35 %40 %45
         %51 = OpCompositeConstruct %mat4v4float %46 %47 %48 %49
         %52 = OpLoad %int %inInst_index
         %61 = OpAccessChain %_ptr_Uniform__MatrixStorage_float4x4_ColMajorstd140 %ubo %int_0
         %62 = OpLoad %_MatrixStorage_float4x4_ColMajorstd140 %61
         %63 = OpCompositeExtract %_arr_v4float_int_4 %62 0
         %64 = OpCompositeExtract %v4float %63 0
         %65 = OpCompositeExtract %float %64 0
         %66 = OpCompositeExtract %float %64 1
         %67 = OpCompositeExtract %float %64 2
         %68 = OpCompositeExtract %float %64 3
         %69 = OpCompositeExtract %v4float %63 1
         %70 = OpCompositeExtract %float %69 0
         %71 = OpCompositeExtract %float %69 1
         %72 = OpCompositeExtract %float %69 2
         %73 = OpCompositeExtract %float %69 3
         %74 = OpCompositeExtract %v4float %63 2
         %75 = OpCompositeExtract %float %74 0
         %76 = OpCompositeExtract %float %74 1
         %77 = OpCompositeExtract %float %74 2
         %78 = OpCompositeExtract %float %74 3
         %79 = OpCompositeExtract %v4float %63 3
         %80 = OpCompositeExtract %float %79 0
         %81 = OpCompositeExtract %float %79 1
         %82 = OpCompositeExtract %float %79 2
         %83 = OpCompositeExtract %float %79 3
         %84 = OpCompositeConstruct %v4float %65 %70 %75 %80
         %85 = OpCompositeConstruct %v4float %66 %71 %76 %81
         %86 = OpCompositeConstruct %v4float %67 %72 %77 %82
         %87 = OpCompositeConstruct %v4float %68 %73 %78 %83
         %88 = OpCompositeConstruct %mat4v4float %84 %85 %86 %87
         %89 = OpMatrixTimesMatrix %mat4v4float %88 %51
         %91 = OpAccessChain %_ptr_Uniform__MatrixStorage_float4x4_ColMajorstd140 %ubo %int_1
         %92 = OpLoad %_MatrixStorage_float4x4_ColMajorstd140 %91
         %93 = OpCompositeExtract %_arr_v4float_int_4 %92 0
         %94 = OpCompositeExtract %v4float %93 0
         %95 = OpCompositeExtract %float %94 0
         %96 = OpCompositeExtract %float %94 1
         %97 = OpCompositeExtract %float %94 2
         %98 = OpCompositeExtract %float %94 3
         %99 = OpCompositeExtract %v4float %93 1
        %100 = OpCompositeExtract %float %99 0
        %101 = OpCompositeExtract %float %99 1
        %102 = OpCompositeExtract %float %99 2
        %103 = OpCompositeExtract %float %99 3
        %104 = OpCompositeExtract %v4float %93 2
        %105 = OpCompositeExtract %float %104 0
        %106 = OpCompositeExtract %float %104 1
        %107 = OpCompositeExtract %float %104 2
        %108 = OpCompositeExtract %float %104 3
        %109 = OpCompositeExtract %v4float %93 3
        %110 = OpCompositeExtract %float %109 0
        %111 = OpCompositeExtract %float %109 1
        %112 = OpCompositeExtract %float %109 2
        %113 = OpCompositeExtract %float %109 3
        %114 = OpCompositeConstruct %v4float %95 %100 %105 %110
        %115 = OpCompositeConstruct %v4float %96 %101 %106 %111
        %116 = OpCompositeConstruct %v4float %97 %102 %107 %112
        %117 = OpCompositeConstruct %v4float %98 %103 %108 %113
        %118 = OpCompositeConstruct %mat4v4float %114 %115 %116 %117
        %119 = OpMatrixTimesMatrix %mat4v4float %118 %89
        %122 = OpCompositeConstruct %v4float %13 %float_1
        %124 = OpMatrixTimesVector %v4float %119 %122
        %128 = OpVectorShuffle %v2float %16 %16 0 1
        %163 = OpLoad %v3float %158
        %162 = OpLoad %v3float %157
               OpStore %entryPointParam_vertexMain_position %124
               OpStore %entryPointParam_vertexMain_texCoord %128
               OpStore %entryPointParam_vertexMain_tangeant %162
               OpStore %entryPointParam_vertexMain_bitangeant %163
               OpStore %entryPointParam_vertexMain_texIndex %52
               OpReturn
               OpFunctionEnd

#include "pipeline.h"
#include "util.h"
#include "math/types.h"
#include "core/logger.h"
#include <vulkan/vulkan_core.h>

extern VulkanContext vulkan_context;

bool vulkan_graphics_pipeline_create(VulkanRenderpass *renderpass,
  unsigned int attribute_count, VkVertexInputAttributeDescription *attributes,
  unsigned int descriptor_set_layout_count,
  VkDescriptorSetLayout *descriptor_set_layouts, unsigned int stage_count,
  VkPipelineShaderStageCreateInfo *stages, VkViewport viewport,
  VkRect2D scissor, bool is_wireframe, VulkanPipeline *out_pipeline) {

    // viewport state
    VkPipelineViewportStateCreateInfo viewport_state = {
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;

    // rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    rasterizer_create_info.depthClampEnable = VK_FALSE;
    rasterizer_create_info.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_create_info.polygonMode =
      is_wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
    rasterizer_create_info.lineWidth = 1.0f;
    rasterizer_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer_create_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer_create_info.depthBiasEnable = VK_FALSE;
    rasterizer_create_info.depthBiasConstantFactor = 0.0f;
    rasterizer_create_info.depthBiasClamp = 0.0f;
    rasterizer_create_info.depthBiasSlopeFactor = 0.0f;

    // multisampling
    VkPipelineMultisampleStateCreateInfo ms_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    ms_create_info.sampleShadingEnable = VK_FALSE;
    ms_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    ms_create_info.minSampleShading = 1.0f;
    ms_create_info.pSampleMask = nullptr;
    ms_create_info.alphaToCoverageEnable = VK_FALSE;
    ms_create_info.alphaToOneEnable = VK_FALSE;

    // depth and stencil testing
    VkPipelineDepthStencilStateCreateInfo depth_stencil = {
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
    depth_stencil.depthTestEnable = VK_TRUE;
    depth_stencil.depthWriteEnable = VK_TRUE;
    depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depth_stencil.depthBoundsTestEnable = VK_FALSE;
    depth_stencil.stencilTestEnable = VK_FALSE;

    // color blending
    VkPipelineColorBlendAttachmentState color_blend_attachment = {};
    color_blend_attachment.blendEnable = VK_TRUE;
    color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_blend_attachment.dstColorBlendFactor =
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_blend_attachment.dstAlphaBlendFactor =
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

    color_blend_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo cb_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    cb_create_info.logicOpEnable = VK_FALSE;
    cb_create_info.logicOp = VK_LOGIC_OP_COPY;
    cb_create_info.attachmentCount = 1;
    cb_create_info.pAttachments = &color_blend_attachment;

    // dynamic state
    constexpr unsigned int dynamic_state_count = 3;
    VkDynamicState dynamic_states[dynamic_state_count] = {
      VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
      VK_DYNAMIC_STATE_LINE_WIDTH};

    // dynamic state create info
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
    dynamic_state_create_info.dynamicStateCount = dynamic_state_count;
    dynamic_state_create_info.pDynamicStates = dynamic_states;

    // vertex input
    VkVertexInputBindingDescription binding_description;
    binding_description.binding = 0;
    binding_description.stride = sizeof(Vertex3D);
    binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // attrributes
    VkPipelineVertexInputStateCreateInfo vertex_input_info = {
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.pVertexBindingDescriptions = &binding_description;
    vertex_input_info.vertexAttributeDescriptionCount = attribute_count;
    vertex_input_info.pVertexAttributeDescriptions = attributes;

    // input assembly
    VkPipelineInputAssemblyStateCreateInfo input_assembly = {
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable = VK_FALSE;

    // pipelin layout
    VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};

    // descriptor set layouts
    pipeline_layout_create_info.setLayoutCount = descriptor_set_layout_count;
    pipeline_layout_create_info.pSetLayouts = descriptor_set_layouts;

    // create the pipeline layout
    vk_check(vkCreatePipelineLayout(vulkan_context.device.logical_device,
      &pipeline_layout_create_info, vulkan_context.allocator,
      &out_pipeline->layout));

    // create the pipeline
    VkGraphicsPipelineCreateInfo pipeline_create_info = {
      VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    pipeline_create_info.stageCount = stage_count;
    pipeline_create_info.pStages = stages;
    pipeline_create_info.pVertexInputState = &vertex_input_info;
    pipeline_create_info.pInputAssemblyState = &input_assembly;

    pipeline_create_info.pViewportState = &viewport_state;
    pipeline_create_info.pRasterizationState = &rasterizer_create_info;
    pipeline_create_info.pMultisampleState = &ms_create_info;
    pipeline_create_info.pDepthStencilState = &depth_stencil;
    pipeline_create_info.pColorBlendState = &cb_create_info;
    pipeline_create_info.pDynamicState = &dynamic_state_create_info;
    pipeline_create_info.pTessellationState = nullptr;

    pipeline_create_info.layout = out_pipeline->layout;
    pipeline_create_info.renderPass = renderpass->handle;
    pipeline_create_info.subpass = 0;
    pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    pipeline_create_info.basePipelineIndex = -1;

    VkResult result = vkCreateGraphicsPipelines(
      vulkan_context.device.logical_device, VK_NULL_HANDLE, 1,
      &pipeline_create_info, vulkan_context.allocator, &out_pipeline->handle);

    if (vulkan_result_is_success(result)) {
        ilog("successfully created graphics pipeline");
    } else {
        elog("failed to create graphics pipeline");
        return false;
    }
    return true;
}

void vulkan_pipeline_destroy(VulkanPipeline *pipeline) {
    if (pipeline) {
        if (pipeline->handle) {
            vkDestroyPipeline(vulkan_context.device.logical_device,
              pipeline->handle, vulkan_context.allocator);
            pipeline->handle = VK_NULL_HANDLE;
        }

        if (pipeline->layout) {
            vkDestroyPipelineLayout(vulkan_context.device.logical_device,
              pipeline->layout, vulkan_context.allocator);
            pipeline->layout = VK_NULL_HANDLE;
        }
    }
}

void vulkan_pipeline_bind(VulkanCommandBuffer *command_buffer,
  VkPipelineBindPoint bind_point, VulkanPipeline *pipeline) {
    vkCmdBindPipeline(command_buffer->handle, bind_point, pipeline->handle);
}

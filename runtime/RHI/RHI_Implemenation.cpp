/*
Copyright(c) 2016-2024 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//= INCLUDES ==================
#include "pch.h"
#include "RHI_Implementation.h"
//=============================

//= NAMESPACES =====
using namespace std;
//==================

namespace Spartan
{
    // api specific
#if defined(API_GRAPHICS_D3D12)
    RHI_Api_Type  RHI_Context::api_type     = RHI_Api_Type::D3d12;
    string        RHI_Context::api_type_str = "D3D12";
    ID3D12Device* RHI_Context::device       = nullptr;
#elif defined(API_GRAPHICS_VULKAN)
    RHI_Api_Type     RHI_Context::api_type        = RHI_Api_Type::Vulkan;
    string           RHI_Context::api_type_str    = "Vulkan";
    VkInstance       RHI_Context::instance        = nullptr;
    VkPhysicalDevice RHI_Context::device_physical = nullptr;
    VkDevice         RHI_Context::device          = nullptr;

    vector<VkValidationFeatureEnableEXT> RHI_Context::validation_extensions;
    vector<const char*> RHI_Context::extensions_instance = { "VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_swapchain_colorspace" };
    vector<const char*> RHI_Context::validation_layers   = { "VK_LAYER_KHRONOS_validation" };
    vector<const char*> RHI_Context::extensions_device   = { "VK_KHR_swapchain", "VK_EXT_memory_budget", "VK_EXT_extended_dynamic_state" };
    // hardware capability viewer: https://vulkan.gpuinfo.org/
#endif

   // api agnostic
    string RHI_Context::api_version_str;
}

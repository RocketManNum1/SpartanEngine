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

#pragma once

//= INCLUDES ===============
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix.h"
#include "Color.h"
//==========================

namespace Spartan
{
    // low frequency - updates once per frame
    struct Cb_Frame
    {
        Math::Matrix view;
        Math::Matrix projection;
        Math::Matrix view_projection;
        Math::Matrix view_projection_inv;
        Math::Matrix view_projection_ortho;
        Math::Matrix view_projection_unjittered;
        Math::Matrix view_projection_previous;

        Math::Vector2 resolution_render;
        Math::Vector2 resolution_output;

        Math::Vector2 taa_jitter_current;
        Math::Vector2 taa_jitter_previous;

        float time;
        float delta_time;
        uint32_t frame;
        uint32_t options;

        Math::Vector3 camera_position;
        float camera_near;

        Math::Vector3 camera_direction;
        float camera_far;

        float gamma;
        float camera_last_movement_time;
        Math::Vector2 padding;

        Math::Vector3 camera_position_previous;
        uint32_t material_index;

        void set_bit(const bool set, const uint32_t bit)
        {
            options = set ? (options |= bit) : (options & ~bit);
        }

        bool operator==(const Cb_Frame& rhs) const
        {
            return
                view                       == rhs.view                       &&
                projection                 == rhs.projection                 &&
                view_projection            == rhs.view_projection            &&
                view_projection_inv        == rhs.view_projection_inv        &&
                view_projection_ortho      == rhs.view_projection_ortho      &&
                view_projection_unjittered == rhs.view_projection_unjittered &&
                view_projection_previous   == rhs.view_projection_previous   &&
                time                       == rhs.time                       &&
                delta_time                 == rhs.delta_time                 &&
                frame                      == rhs.frame                      &&
                camera_near                == rhs.camera_near                &&
                camera_far                 == rhs.camera_far                 &&
                camera_position            == rhs.camera_position            &&
                camera_position_previous   == rhs.camera_position_previous   &&
                camera_direction           == rhs.camera_direction           &&
                camera_last_movement_time  == rhs.camera_last_movement_time  &&
                gamma                      == rhs.gamma                      &&
                resolution_output          == rhs.resolution_output          &&
                resolution_render          == rhs.resolution_render          &&
                taa_jitter_current         == rhs.taa_jitter_current         &&
                taa_jitter_previous        == rhs.taa_jitter_previous        &&
                material_index             == rhs.material_index             &&
                options                    == rhs.options;
        }

        bool operator!=(const Cb_Frame& rhs) const { return !(*this == rhs); }
    };

    // 128 byte push constant buffer - updates per pass/draw
    struct Pcb_Pass
    {
        Math::Matrix transform = Math::Matrix::Identity;
        Math::Matrix m_value   = Math::Matrix::Identity;

        void set_transform_previous(const Math::Matrix& transform_previous)
        {
            m_value = transform_previous;
        }

        void set_resolution_in(const RHI_Texture* texture)
        {
            m_value.m03 = static_cast<float>(texture->GetWidth());
            m_value.m22 = static_cast<float>(texture->GetHeight());
        };

        void set_resolution_in(const Math::Vector2& resolution)
        {
            m_value.m03 = resolution.x;
            m_value.m22 = resolution.y;
        };

        void set_resolution_out(const RHI_Texture* texture)
        {
            m_value.m23 = static_cast<float>(texture->GetWidth());
            m_value.m30 = static_cast<float>(texture->GetHeight());
        };

        void set_resolution_out(const Math::Vector2& resolution)
        {
            m_value.m23 = resolution.x;
            m_value.m30 = resolution.y;
        };

        void set_f3_value(const Math::Vector3& value)
        {
            m_value.m00 = value.x;
            m_value.m01 = value.y;
            m_value.m02 = value.z;
        };

        void set_f3_value(const float x, const float y = 0.0f, const float z = 0.0f)
        {
            m_value.m00 = x;
            m_value.m01 = y;
            m_value.m02 = z;
        };

        void set_f3_value2(const Math::Vector3& value)
        {
            m_value.m20 = value.x;
            m_value.m21 = value.y;
            m_value.m31 = value.z;
        };

        void set_f3_value2(const float x, const float y, const float z)
        {
            m_value.m20 = x;
            m_value.m21 = y;
            m_value.m31 = z;
        };

        void set_f4_value(const Color& color)
        {
            m_value.m10 = color.r;
            m_value.m11 = color.g;
            m_value.m12 = color.b;
            m_value.m13 = color.a;
        };

        void set_f4_value(const float x, const float y, const float z, const float w)
        {
            m_value.m10 = x;
            m_value.m11 = y;
            m_value.m12 = z;
            m_value.m13 = w;
        };

        void set_material_index(const uint32_t index)
        {
            m_value.m32 = static_cast<float>(index);
        }

        void set_is_transparent(const bool is_transparent)
        {
            m_value.m33 = is_transparent ? 1.0f : 0.0f;
        }

        bool operator==(const Pcb_Pass& rhs) const
        {
            return transform == rhs.transform && m_value == rhs.m_value;
        }

        bool operator!=(const Pcb_Pass& rhs) const { return !(*this == rhs); }
    };

    struct Sb_Material
    {
        Math::Vector4 color = Math::Vector4::Zero;

        Math::Vector2 tiling_uv = Math::Vector2::Zero;
        Math::Vector2 offset_uv = Math::Vector2::Zero;

        float roughness_mul = 0.0f;
        float metallic_mul  = 0.0f;
        float normal_mul    = 0.0f;
        float height_mul    = 0.0f;

        uint32_t flags           = 0;
        float world_space_height = 0.0f;
        float ior                = 1.0f;
        float subsurface_scattering;

        float sheen;
        Math::Vector3 sheen_tint;

        float anisotropic;
        float anisotropic_rotation;
        float clearcoat;
        float clearcoat_roughness;
    };

    struct Sb_Light
    {
        Math::Matrix view_projection[6];

        float intensity;
        float range;
        float angle;
        float bias;

        Color color;

        Math::Vector3 position;
        float normal_bias;

        Math::Vector3 direction;
        uint32_t flags;

        bool operator==(const Sb_Light& rhs)
        {
            return
                view_projection[0] == rhs.view_projection[0] &&
                view_projection[1] == rhs.view_projection[1] &&
                view_projection[2] == rhs.view_projection[2] &&
                view_projection[3] == rhs.view_projection[3] &&
                view_projection[4] == rhs.view_projection[4] &&
                view_projection[5] == rhs.view_projection[5] &&
                intensity          == rhs.intensity          &&
                range              == rhs.range              &&
                angle              == rhs.angle              &&
                bias               == rhs.bias               &&
                normal_bias        == rhs.normal_bias        &&
                color              == rhs.color              &&
                position           == rhs.position           &&
                direction          == rhs.direction          &&
                flags              == rhs.flags;
        }
    };
}

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

//= INCLUDES ===============
#include "pch.h"
#include "RHI_CommandList.h"
#include "RHI_Fence.h"
//==========================

//= NAMESPACES =====
using namespace std;
//==================

namespace Spartan
{
    void RHI_CommandList::WaitForExecution()
    {
        SP_ASSERT_MSG(m_state == RHI_CommandListState::Submitted, "The command list hasn't been submitted, can't wait for it.");

        // wait for execution to finish
        if (IsExecuting())
        {
            SP_ASSERT_MSG(m_proccessed_fence->Wait(), "Timed out while waiting for the fence");
        }

        // reset fence
        if (m_proccessed_fence->GetStateCpu() == RHI_Sync_State::Submitted)
        {
            m_proccessed_fence->Reset();
        }

        m_state = RHI_CommandListState::Idle;
    }

    bool RHI_CommandList::IsExecuting()
    {
        return
            m_state == RHI_CommandListState::Submitted && // it has been submitted
            !m_proccessed_fence->IsSignaled();            // and the fence is not signaled yet
    }
}

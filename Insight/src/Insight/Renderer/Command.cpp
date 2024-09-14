#include "Command.h"

#include "Context.h"
#include "SynchronizationFrame.h"

namespace Insight::Renderer {
    void Command::Initialize()
    {

    }

    void Command::Dispose()
    {

    }

    CommandBuffer& Command::BeginFrame()
    {
        auto &frame = Context::Instance().AcquireNextFrame();

        Context::Instance().AcquireNextImage(frame.GetImageAvailableSemaphore());

        frame.GetInFlightFence().Reset();

        frame.ResetResources();

        auto &commandBuffer = frame.GetCommandBuffer();

        commandBuffer.Reset();

        commandBuffer.Begin();

        return commandBuffer;
    }

    void Command::CompositeFrame(const bool enableFinalRender, const bool enableUi)
    {
        auto &frame = Context::Instance().GetCurrentFrame();

        auto &commandBuffer = frame.GetCommandBuffer();

        Context::Instance().GetFinalCompositorPass().Run({
            .CommandBuffer = commandBuffer,
            .EnableFinalRender = enableFinalRender,
            .EnableUi = enableUi,
        });
    }

    void Command::EndFrame()
    {
        auto &frame = Context::Instance().GetCurrentFrame();

        auto &commandBuffer = frame.GetCommandBuffer();

        commandBuffer.End();

        Context::Instance().SubmitFrame(frame);

        Context::Instance().PresentFrame(frame);
    }


}

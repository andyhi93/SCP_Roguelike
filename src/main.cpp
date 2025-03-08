#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    //context->SetWindowSize(1920, 1080);
    App app;

    while (!context->GetExit()) {
        context->Setup();
        switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start();
                break;

            case App::State::UPDATE:
                std::cout << "main.update" << std::endl;
                app.Update();
                break;

            case App::State::END:
                std::cout << "end.update" << std::endl;
                app.End();
                context->SetExit(true);
                break;
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        context->Update();
    }
    return 0;
}

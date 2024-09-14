#include <iostream>

#include "EditorApplication.h"

int main(int argc, char **argv)
{
    Insight::vector<Insight::string> args(argv, argv + argc);

    Insight::ApplicationInfo info = {
        .Name = "Editor",
        .Arguments = args
    };

    auto &app = Insight::Application::Create<Portent::EditorApplication>(info);

    app.Run();
}

#include "Application.h"

int main(int, char**)
{
    Application app;
    Camera camera;
    app.SetCamera(camera);
    app.Run();

    return 0;
}

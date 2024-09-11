#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

b2World *world;

struct AppContext
{
    SDL_Window *window;
    SDL_GLContext glContext;
    SDL_AppResult appQuit = SDL_APP_CONTINUE;
};

const char *vertexShaderSource =
    "attribute vec2 aPosition;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
    "}\n";

const char *fragmentShaderSource =
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(0.2, 0.7, 0.3, 1.0);\n"
    "}\n";

// Helper function for creating shaders
GLuint createShader(const char *shaderSource, int shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        glDeleteShader(shader); // Don't leak the shader
        std::cout << &(errorLog[0]) << std::endl;
        std::cout << shaderSource << std::endl;
    }
    return shader;
}

// Helper function for creating a shader program
GLuint createShaderProgram()
{
    GLuint program = glCreateProgram();
    GLuint vShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glUseProgram(program);

    return program;
}

// Load a triangle to the video card
void initVertexBuffers(GLuint program)
{
    float vertPositions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.f, 0.5f
    };
    GLuint vertPosBuffer;
    glGenBuffers(1, &vertPosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    int amount = sizeof(vertPositions) / sizeof(vertPositions[0]);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(GLfloat),
        vertPositions, GL_STATIC_DRAW);
    GLint aPositionLocation = glGetAttribLocation(program, "aPosition");
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(aPositionLocation);
}

SDL_AppResult SDL_Fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    // Init the library, here we make a window so we only need the Video capabilities.
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        return SDL_Fail();
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // Enable MULTISAMPLE
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2); // can be 2, 4, 8 or 16

    // Create a window
    SDL_Window *window = SDL_CreateWindow("SDL3, OpenGL 2.1, C++", 400, 400,
        SDL_WINDOW_OPENGL); // | SDL_WINDOW_RESIZABLE
    if (!window)
    {
        return SDL_Fail();
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        return SDL_Fail();
    }

    if (!gladLoadGL())
    {
        std::cout << "Failed to load OpenGL functions" << std::endl;
        return SDL_APP_FAILURE;
    }

    GLuint program = createShaderProgram();
    initVertexBuffers(program);

    b2Vec2 gravity(0.f, -9.8f);
    world = new b2World(gravity);
    float gx = world->GetGravity().x;
    float gy = world->GetGravity().y;
    std::cout << "gravity = (" << gx << ", " << gy << ")" << std::endl;

    SDL_ShowWindow(window);

    // Set up the application data
    *appstate = new AppContext {
        window,
        glContext,
    };

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, const SDL_Event *event)
{
    auto *app = (AppContext *)appstate;

    switch (event->type)
    {
        case SDL_EVENT_QUIT:
        {
            app->appQuit = SDL_APP_SUCCESS;
            break;
        }
        default:
        {
            break;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto *app = (AppContext *)appstate;

    glClearColor(0.188f, 0.22f, 0.255f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    SDL_GL_SwapWindow(app->window);

    return app->appQuit;
}

void SDL_AppQuit(void *appstate)
{
    auto *app = (AppContext *)appstate;
    if (app)
    {
        SDL_GL_DestroyContext(app->glContext);
        SDL_DestroyWindow(app->window);
        delete app;
    }

    delete world;
    SDL_Quit();
}

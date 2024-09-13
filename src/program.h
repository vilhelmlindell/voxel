#pragma once

class Program {
  public:
    Program();
    ~Program();

    void Update();
    void Render();
private:
    void InitializeWindow();
    void InitializeImGui();
    void InitializeBuffers();
    void LoadTexture();
    void Cleanup();
    void ProcessInput();
};

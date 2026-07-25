#pragma once

class ProjectorResizer
{
public:
    static void Execute();

    static int GetTargetWidth() { return m_width; }
    static int GetTargetHeight() { return m_height; }
    static void SetTargetSize(int width, int height);

    static void SaveConfig();
    static void LoadConfig();

private:
    static void ResizeActiveProjectors(int width, int height);

    static int m_width;
    static int m_height;
};

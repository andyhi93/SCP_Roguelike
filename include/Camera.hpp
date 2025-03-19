#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>
#include <iostream>

class Camera {
public:
    static Camera& GetInstance() {
        static Camera instance;
        return instance;
    }
    void Update(){}
    glm::vec2 GetCameraWorldCoord() { return CameraWorldCoord; }
    void SetCameraWorldCoord(glm::vec2 pos) { CameraWorldCoord = pos; }

    bool isActive = true;
private:
    glm::vec2 CameraWorldCoord = { 0,0 };

    Camera() { } // �p���غc�l�A����~���Ы�
    ~Camera() {}

    // �T������P���
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
};

#endif
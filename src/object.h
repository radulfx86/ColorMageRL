#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "types.h"
#include <map>
#include <unordered_map>

class Object2D : public Drawable
{
public:
    Object2D() {}
    GLuint attribPos;
    GLuint vao;
    GLuint vertexBuffer;
    GLuint tex;
    GLuint program;
    GLuint texOffset;
    Animation animation;
    Vec2 pos;
    virtual void updateAnimation(float delta_s);
    virtual void draw() override;
    virtual void setPosition(Vec2 pos) override;
    virtual void updateCamera(float view[16], float proj[16]) override;
    void setAnimation(Direction_t animDir);
    friend bool operator<(const Object2D &a, const Object2D &b)
    {
        return a < b;
    }
};

class InstancedObject2D : public Object2D
{
public:
    int numInstances;
    Vec2 pos;
    Vec2 size;
    std::vector<Animation> animations;
    /// @note might not be needed
    std::unordered_map<Vec2i, int> instancePositions;
    virtual void draw() override;
    virtual void updateAnimation(float delta_s) override;
    void updateInstance(int instance, bool enabled, Vec2 pos, Vec2 texPos, Vec2 texSize);
    void updateInstanceType(int instance, bool enabled, Vec2 texPos);
    void updateInstanceTypePos(int instance, bool enabled, Vec2 pos, Vec2 texPos);
};


class MultiInstancedObject2D : public Object2D
{
public:
    int numInstances;
    Vec2 pos;
    Vec2 size;
    std::vector<InstancedObject2D*> subInstances;
    virtual void draw() override;
    virtual void updateAnimation(float delta_s) override;
    virtual void setPosition(Vec2 pos) override;
    virtual void updateCamera(float view[16], float proj[16]) override;
    void updateInstance(int instance, bool enabled, Vec2 pos, Vec2 texPos, Vec2 texSize);
    void updateInstanceType(int instance, bool enabled, Vec2 texPos);
    void updateInstanceTypePos(int instance, bool enabled, Vec2 pos, Vec2 texPos);
};

class Text2D : public InstancedObject2D
{
public:
    void setText(std::string text);
    void setCharacterSize(Vec2 size, Vec2 displayDistance);
    void setColor(float color[3]);
    virtual void draw() override { InstancedObject2D::draw(); }
    virtual void updateAnimation(float delta_s) override { InstancedObject2D::updateAnimation(delta_s); }
    Vec2 characterSize;
    Vec2 characterDisplayDistance;
    std::map<char, int> textIndex;
    void setTextIndex(char c, int i) { textIndex[c] = i; }
    int textureColumns;
};

class Path2D : public Drawable
{
public:
    GLuint attribPos;
    GLuint program;
    GLuint vao;
    GLuint vertexBuffer;
    std::vector<float> vertexData;
    Path2D(std::vector<Vec2> elements, float color[3]);
    
    void setPath(std::vector<Vec2> elements);
    void setColor(float color[3]);
    virtual void draw() override;
    virtual void setPosition(Vec2 pos) override;
    virtual void updateCamera(float view[16], float proj[16]) override;
};


#endif // _OBJECT_H_
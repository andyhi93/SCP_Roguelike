#ifndef UITEXT_HPP
#define UITEXT_HPP

#include "Core/Object.hpp"
#include "Util/Text.hpp"

class UIText : public Object {
public:
    UIText() = default;

    UIText(std::string font, const int size)
        : m_Font(std::move(font)),
        m_Size(size) {}

    ~UIText() override = default;

    void Start();

    void Update();
    std::shared_ptr<Util::Text> m_Text;

private:
    std::string m_Font;
    int m_Size;
};

#endif

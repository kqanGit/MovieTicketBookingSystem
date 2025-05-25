#ifndef ROUNDEDRECTANGLESHAPE_H
#define ROUNDEDRECTANGLESHAPE_H

#include <SFML/Graphics.hpp>
#include <cmath>

class RoundedRectangleShape : public sf::Shape
{
public:
    explicit RoundedRectangleShape(const sf::Vector2f& size = {0,0}, float radius = 0, std::size_t cornerPointCount = 10)
        : m_size(size), m_radius(radius), m_cornerPointCount(cornerPointCount)
    {
        update();
    }

    void setSize(const sf::Vector2f& size)
    {
        m_size = size;
        update();
    }

    const sf::Vector2f& getSize() const
    {
        return m_size;
    }

    void setCornersRadius(float radius)
    {
        m_radius = radius;
        update();
    }

    float getCornersRadius() const
    {
        return m_radius;
    }

    void setCornerPointCount(std::size_t count)
    {
        m_cornerPointCount = count;
        update();
    }

    std::size_t getPointCount() const override
    {
        return m_cornerPointCount * 4;
    }

    sf::Vector2f getPoint(std::size_t index) const override
    {
        std::size_t corner = index / m_cornerPointCount;
        float theta = (index % m_cornerPointCount) * 90.f / (m_cornerPointCount - 1);
        float rad = theta * 3.141592654f / 180.f;

        sf::Vector2f center;
        switch (corner)
        {
            case 0: center = {m_size.x - m_radius, m_radius}; break;          // top-right
            case 1: center = {m_radius, m_radius}; break;                    // top-left
            case 2: center = {m_radius, m_size.y - m_radius}; break;         // bottom-left
            case 3: center = {m_size.x - m_radius, m_size.y - m_radius}; break; // bottom-right
        }

        // Góc tròn theo chiều ngược kim đồng hồ bắt đầu từ góc trên phải
        float x = center.x + m_radius * std::cos(rad + corner * 3.141592654f / 2);
        float y = center.y + m_radius * std::sin(rad + corner * 3.141592654f / 2);

        return {x, y};
    }

private:
    sf::Vector2f m_size;
    float m_radius;
    std::size_t m_cornerPointCount;
};

#endif // ROUNDEDRECTANGLESHAPE_H

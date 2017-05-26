#include "objects.hpp"
#include <NGAME/renderer2d.hpp>
#include <glm/trigonometric.hpp>
#include "../sc_master.hpp"

void Wall::render(const Renderer2d& renderer) const
{
    renderer.render(pos, size, glm::ivec4(), nullptr, 0.f, glm::vec2(), glm::vec4(1.f, 0.7f, 0.f, 0.3f));
}

void Brick::render(const Renderer2d& renderer) const
{
    renderer.render(pos, size, tex_coords, texture, 0.f, glm::vec2(), glm::vec4(1.f, 1.f, 1.f, 1.f));
}

void Life_bar::update(float dt)
{
    time += dt;
    auto coeff = glm::sin(time) * 0.5f + 0.8f;
    color.a = coeff;
}

void Life_bar::render(const Renderer2d& renderer) const
{
    auto pos = this->pos;
    for(auto i = 0; i < lifes; ++i)
    {
        renderer.render(pos, glm::vec2(quad_size), glm::ivec4(0, 0, texture->get_size()), texture, 0.f, glm::vec2(), color);
        pos.x += quad_size + spacing;
    }
}

void Ball::spawn(const Paddle& paddle)
{
    pos.y = paddle.pos.y - 2 * radius;
    pos.x = paddle.pos.x + paddle.size.x / 2.f - radius;
    is_stuck = true;

    // set velocity direction to 45*
    auto len = glm::length(vel);
    auto tan_pow_2 = glm::pow(glm::tan(glm::pi<float>() / 4.f), 2.f);
    vel.x = len / glm::sqrt(tan_pow_2 + 1.f);

    vel.y = -glm::sqrt(glm::pow(len, 2.f) - glm::pow(vel.x, 2.f));

    std::uniform_int_distribution<int> d(0, 1);
    auto right_dir = d(Sc_master::handle->rn_eng);
    if(right_dir)
        vel.x *= -1.f;
}

void Ball::render(const Renderer2d& renderer) const
{
    renderer.render(pos, glm::vec2(radius * 2.f), glm::ivec4(0, 0, texture->get_size()), texture, 0.f, glm::vec2(),
                    glm::vec4(1.f, 1.f, 1.f, 0.5f));
}

void Paddle::render(const Renderer2d& renderer) const
{
    renderer.render(pos, size, glm::ivec4(), nullptr, 0.f, glm::vec2(), glm::vec4(1.f, 1.f, 1.f, 0.6f));
}

void Paddle::update(float dt, Ball& ball)
{
    pos.x += vel * dt;
    if(ball.is_stuck)
        ball.pos.x += vel * dt;
}

void Ball::update(float dt)
{
    if(is_stuck)
        return;

    pos += vel * dt;
}
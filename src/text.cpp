#include <NGAME/text.hpp>
#include <NGAME/font.hpp>
#include <algorithm>
#include <vector>

Text::Text(const Font& font):
    font(&font)
{}

glm::vec4 Text::get_bbox() const
{
    if(text.size() == 0)
        return glm::vec4();

    auto num_lines = std::count(text.begin(), text.end(), '\n') + 1;
    std::vector<float> widths;
    widths.reserve(num_lines);
    widths.emplace_back();
    auto height = (font->get_ascent() - font->get_descent()) * scale;

    for(auto c: text)
    {
        if(c == '\n')
        {
            widths.push_back(0.f);
            height += font->get_linespace() * scale;
        }
        else
        {
            auto& glyph = font->get_glyph(c);
            widths.back() += glyph.advance * scale;
        }
    }
    auto max_width = std::max_element(widths.begin(), widths.end());
    return glm::vec4(pos.x, pos.y - font->get_ascent() * scale, *max_width, height);
}

void Text::set_pixel_size(int size)
{
    scale = static_cast<float>(size) / font->get_size_px();
}

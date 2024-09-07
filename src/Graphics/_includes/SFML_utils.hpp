#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <utility>

namespace SFML_utils {
    using std::unique_ptr, std::shared_ptr;
    using SpritePtr = unique_ptr<sf::Sprite>;
    using TexturePtr = unique_ptr<sf::Texture>;
    using SpriteTexturePair = std::pair<SpritePtr, TexturePtr>;
    using RenderWindowPtr = unique_ptr<sf::RenderWindow>;

    template <typename... Args>
    SpriteTexturePair createSprite(Args... args) {
        std::unique_ptr<sf::Texture> texture{new sf::Texture{}};
        texture->loadFromFile(std::forward<Args>(args)...);
        std::unique_ptr<sf::Sprite> sprite{new sf::Sprite{*texture}};
        sprite->setOrigin(texture->getSize().x / 2., texture->getSize().y / 2.);
        return SpriteTexturePair{std::move(sprite), std::move(texture)};
    }

}  // namespace SFML_utils

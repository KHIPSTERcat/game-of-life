#include <SFML/Graphics.hpp>
#include <vector>
#include <random>


const sf::Vector2f cell_offset(0.5f, 0.5f);
const sf::Vector2f cell_size (4.f,4.f);
const size_t width = 100;
const size_t height = 100;
const float camera_speed = 300;
const float camera_zoom = 1.001f;
const float camera_default_zoom = 0.45f;
const float camera_max_zoom = 2.f;
const float camera_min_zoom = 0.1f;
const float fps = 1.f/8.f;
const float builder_speed = 1.f/8.f;
const sf::Color background_color(50, 50, 50);
const sf::Color cell_color(255, 255, 255);
const sf::Color grid_color(150, 150, 150);
const sf::Color builder_color(255, 0, 0, 100);

using namespace std;

uniform_int_distribution<size_t> dist(0,1);
random_device rd;

sf::Vector2f Normalize(const sf::Vector2f &vec) {
    float length = vec.x * vec.x + vec.y * vec.y;
    if (length < 1e-5) return vec;
    return vec / sqrt(length);
}


int main()
{
    vector<vector<bool>> map(height,vector<bool>(width));
    vector<vector<bool>> buff_map(height,vector<bool>(width));


    

    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Game Of Life", sf::Style::Fullscreen);
    sf::View view(window.getView());
    view.setCenter(sf::Vector2f(width * (cell_size.x + cell_offset.x) + cell_offset.x, height * (cell_size.y + cell_offset.y) + cell_offset.y) / 2.f);
    window.setView(view);

    sf::Clock clock;
    sf::Clock fps_clock;
    sf::Clock builder_clock;
    
    sf::Vector2i builder_position;

    bool grid_visible = true;
    bool game_paused = true;
    bool key_g_is_released = true;
    bool key_space_is_released = true;
    bool key_p_is_released = true;
    bool key_c_is_released = true;
    bool key_r_is_released = true;


    float camera_current_zoom = camera_default_zoom;
    view.zoom(camera_default_zoom);

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Vector2f direction;


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
        if (sf::Joystick::isButtonPressed(0, sf::Joystick::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            direction += sf::Vector2f(-1.f, 0.f);
        }
        if (sf::Joystick::isButtonPressed(0, sf::Joystick::Y) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            direction += sf::Vector2f(1.f, 0.f);
        }
        if (sf::Joystick::isButtonPressed(0, sf::Joystick::R) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            direction += sf::Vector2f(0.f, -1.f);
        }
        if (sf::Joystick::isButtonPressed(0,sf::Joystick::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            direction += sf::Vector2f(0.f, 1.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (builder_position.x > 0 && builder_clock.getElapsedTime().asSeconds() > builder_speed) {
                builder_position += sf::Vector2i(-1, 0); 
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (builder_position.x + 1 < width && builder_clock.getElapsedTime().asSeconds() > builder_speed) {
                builder_position += sf::Vector2i(1, 0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (builder_position.y > 0 && builder_clock.getElapsedTime().asSeconds() > builder_speed) {
                builder_position += sf::Vector2i(0, -1);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            if (builder_position.y + 1 < height && builder_clock.getElapsedTime().asSeconds() > builder_speed) {
                builder_position += sf::Vector2i(0, 1);
            }
        }
        if (builder_clock.getElapsedTime().asSeconds() > builder_speed) {
            builder_clock.restart();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Joystick::isButtonPressed(0, sf::Joystick::U)) {
            if (camera_current_zoom > camera_min_zoom) {
                view.zoom(1 / camera_zoom);
                camera_current_zoom /= camera_zoom;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) || sf::Joystick::isButtonPressed(0, sf::Joystick::V)) {
            if (camera_current_zoom < camera_max_zoom) {
                view.zoom(camera_zoom);
                camera_current_zoom *= camera_zoom;
            }
        }
        if (key_g_is_released) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                grid_visible = !grid_visible;
                key_g_is_released = false;
            }
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
            key_g_is_released = true;
        }

        if (key_p_is_released) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                game_paused = !game_paused;
                key_p_is_released = false;
            }
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            key_p_is_released = true;
        }

        if (key_space_is_released) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                map[builder_position.y][builder_position.x] = !map[builder_position.y][builder_position.x];
                key_space_is_released = false;
            }
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            key_space_is_released = true;
        }

        if (key_c_is_released) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                map = vector<vector<bool>>(height, vector<bool>(width));
                key_c_is_released = false;
            }
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            key_c_is_released = true;
        }

        if (key_r_is_released) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                map = vector<vector<bool>>(height, vector<bool>(width));
                for (size_t i = 0; i < map.size(); i++) {
                    for (size_t j = 0; j < map[i].size(); j++) {
                        map[i][j] = dist(rd);
                    }
                }
                key_r_is_released = false;
            }
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            key_r_is_released = true;
        }

        direction = Normalize(direction);
        

        view.setCenter(view.getCenter() + direction * dt * camera_speed * camera_current_zoom);
        window.setView(view);

        window.clear(background_color);

        sf::RectangleShape builder_rect(cell_size);
        builder_rect.setFillColor(builder_color);
        sf::RectangleShape cell_rect(cell_size);
        cell_rect.setFillColor(cell_color);
        sf::RectangleShape v_rect(sf::Vector2f(cell_offset.x, height * (cell_size.y + cell_offset.y) + cell_offset.y));
        v_rect.setFillColor(grid_color);
        sf::RectangleShape h_rect(sf::Vector2f(width * (cell_size.x + cell_offset.x) + cell_offset.x, cell_offset.y));
        h_rect.setFillColor(grid_color);

        if (grid_visible) {
            for (size_t i = 0; i < width + 1; i++) {
                v_rect.setPosition(i * (cell_size.x + cell_offset.x), 0.f);
                window.draw(v_rect);
            }

            for (size_t i = 0; i < height + 1; i++) {
                h_rect.setPosition(0.f, i * (cell_size.y + cell_offset.y));
                window.draw(h_rect);
            }
        }
        

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                if (map[i][j]){
                    cell_rect.setPosition(j * (cell_size.x + cell_offset.x) + cell_offset.x, i * (cell_size.y + cell_offset.y) + cell_offset.y);
                    window.draw(cell_rect);
                }
            }
        }


        builder_rect.setPosition(builder_position.x * (cell_size.x + cell_offset.x) + cell_offset.x,
            builder_position.y * (cell_size.y + cell_offset.y) + cell_offset.y);
        window.draw(builder_rect);


        if (fps_clock.getElapsedTime().asSeconds() < fps) {
            window.display();
            continue;
        }
        fps_clock.restart();

        if (!game_paused) {
            for (size_t i = 0; i < map.size(); i++) {
                for (size_t j = 0; j < map[i].size(); j++) {
                    size_t count = map[(i + 1) % height][(j - 1 + width) % width]
                        + map[(i + 1) % height][j] 
                        + map[(i + 1) % height][(j + 1) % width] 
                        + map[i][(j - 1 + width) % width] +
                        map[i][(j + 1) % width] +
                        map[(i - 1 + height) % height][(j - 1 + width) % width] +
                        map[(i - 1 + height) % height][j] +
                        map[(i - 1 + height) % height][(j + 1) % width];
                    buff_map[i][j] = map[i][j];
                    if (map[i][j]) {
                        if (count < 2 || count > 3) {
                            buff_map[i][j] = false;
                        }
                    }
                    else {
                        if (count == 3) {
                            buff_map[i][j] = true;
                        }
                    }
                }
            }
            map.swap(buff_map);
        }
        

        

        window.display();
    }

    return 0;
}
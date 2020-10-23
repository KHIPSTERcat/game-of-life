#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include "world.pb.h"

using boost::asio::ip::tcp;

class session
    
{
public:
    session(tcp::socket socket,const std::vector<std::unique_ptr<session>>* players, size_t player_id)
        : socket_(std::move(socket)), player_id(player_id)
    {
        data_.assign(1024,' ');
        this->g_players = players;
    }

    void start()
    {
        do_write();
    }

    bool IsDisconnected() const {
        return is_disconnected;
    }

private:
    World create_world(size_t curent_player_id) const {
        World new_world;
        for (const std::unique_ptr<session>& player_session : *g_players) {
            if (player_session->is_disconnected) continue;
            Player* new_player = new_world.add_players();
            new_player->set_x(player_session->player_x);
            new_player->set_y(player_session->player_y);
            new_player->set_id(player_session->player_id);
            new_player->set_is_player(player_session->player_id == curent_player_id); 
        }
        return new_world;
    }

    void do_read()
    {

        socket_.async_read_some(boost::asio::buffer(data_),
            [this](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    Player player;
                    player.ParseFromArray(data_.data(), length);
                    player_x = player.x();
                    player_y = player.y();
                    do_write();
                } else {
                    this->is_disconnected = true;
                }
            });
    }

    void do_write()
    {
        


        for (const std::unique_ptr<session> &player_session : *g_players) {
            if (player_session->is_disconnected) continue;
            boost::asio::async_write(player_session->socket_,
            boost::asio::buffer(create_world(player_session->player_id).SerializeAsString()),
                [this](boost::system::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        do_read();
                    } else {
                        this->is_disconnected = true;
                    }
                });
        }
        
    }

    const std::vector<std::unique_ptr<session>>* g_players;

    size_t player_x = 0;
    size_t player_y = 0;
    size_t player_id = 0;

    bool is_disconnected = false;


    tcp::socket socket_;
    std::string data_;
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
    }

    

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    for (size_t i = 0; i < g_players.size();) {
                        if (g_players[i]->IsDisconnected()) {
                            std::swap(g_players[i], g_players.back());
                            g_players.pop_back();
                            
                        } else {
                            i++;
                        }
                    }
                    g_players.push_back(std::make_unique<session>(std::move(socket), &g_players, last_id++));
                    g_players.back()->start();
                }

                do_accept();
            });
    }

    size_t last_id = 0;
    std::vector<std::unique_ptr<session>> g_players;
    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
    const int default_port = 5000;
    int port = default_port;
    try
    {
        if (argc >= 2){
            port = std::atoi(argv[1]);
        }

        boost::asio::io_context io_context;

        server s(io_context, port);

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
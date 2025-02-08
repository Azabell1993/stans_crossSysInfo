#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <memory>
#include <vector>
#include <fstream>

using boost::asio::ip::tcp;

class Server
{
public:
    explicit Server(unsigned short port) : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {}

    void start()
    {
        acceptConnection();
        io_context_.run();
    }

private:
    void acceptConnection()
    {
        auto socket = std::make_shared<tcp::socket>(io_context_);
        acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code &ec)
                               {
                                   if (!ec)
                                   {
                                       std::cout << "Server connected: " << socket->remote_endpoint() << std::endl;
                                       receiveData(socket);
                                   }
                                   acceptConnection(); // 다음 연결을 수락
                               });
    }

    void receiveData(const std::shared_ptr<tcp::socket> &socket)
    {
        try
        {
            uint32_t data_size_network_order = 0;
            boost::asio::read(*socket, boost::asio::buffer(&data_size_network_order, sizeof(data_size_network_order)));
            uint32_t data_size = ntohl(data_size_network_order);

            std::cout << "[Client] Expected data size: " << data_size << " bytes" << std::endl;

            if (data_size == 0)
            {
                std::cerr << "[Client] Error: Received data size is zero. Client may not have sent data." << std::endl;
                return;
            }

            std::vector<char> data_buffer(data_size);
            size_t total_read = 0;
            while (total_read < data_size)
            {
                size_t bytes_read = boost::asio::read(*socket, boost::asio::buffer(data_buffer.data() + total_read, data_size - total_read));
                total_read += bytes_read;
                std::cout << "[Client] Read " << bytes_read << " bytes, total: " << total_read << "/" << data_size << " bytes" << std::endl;
            }

            std::string received_message(data_buffer.begin(), data_buffer.end());

            // 파일에 저장
            std::ofstream result_file("result.txt", std::ios::app);
            if (result_file)
            {
                result_file << received_message << std::endl;
                result_file.close();
                std::cout << "[Client] Received message saved to result.txt: " << received_message << std::endl;
            }
            else
            {
                std::cerr << "[Client] Failed to save received message to result.txt" << std::endl;
            }

            sendResponse(socket, "Acknowledged: " + received_message);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[Client] Error in receiveData: " << e.what() << std::endl;
        }
    }

    void sendResponse(const std::shared_ptr<tcp::socket> &socket, const std::string &message)
    {
        boost::asio::async_write(*socket, boost::asio::buffer(message), [socket](const boost::system::error_code &ec, std::size_t)
                                 {
            if (!ec) {
                std::cout << "Response sent." << std::endl;
            } });
    }

    boost::asio::io_context io_context_;
    tcp::acceptor acceptor_;
};

int main()
{
    try
    {
        unsigned short port = 12345;
        Server server(port);
        std::cout << "Running this Server Progream... " << std::endl;
        std::cout << "need the clientPort : " << port << std::endl;
        server.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}

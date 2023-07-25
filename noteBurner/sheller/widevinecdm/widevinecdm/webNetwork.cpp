#include "webNetwork.h"
#include <condition_variable>
#include <mutex>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

using namespace websocketpp;

// ����WebSocket�ͻ�������
typedef client<config::asio_client> client_t;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// �������������ͻ���������ͬ���ȴ��������ظ�
std::condition_variable cv;
std::mutex mutex;
std::string server_response;
bool got_response = false;
client_t cli;
client_t::connection_ptr con;

// ���崦�������Ϣ�Ļص�����
void on_message(client_t* c, websocketpp::connection_hdl hdl, client_t::message_ptr msg) {
    std::cout << "���յ���Ϣ��" << msg->get_payload() << std::endl;

    // �����ﴦ����յ�����Ϣ�������Ը�����Ҫ�����߼�����
    // ...

    // ���÷������ظ�����֪ͨ�ȴ����߳�
    std::unique_lock<std::mutex> lock(mutex);
    server_response = msg->get_payload();
    got_response = true;
    cv.notify_all();
}

// ���������ӷ������������Ƿ����ӳɹ�
bool connectToServer(const std::string& serverAddress, uint16_t port) {

    try {
        // ����ǰ���÷���ͨ��
        cli.clear_access_channels(websocketpp::log::alevel::all);
        cli.set_access_channels(websocketpp::log::alevel::connect);
        cli.set_access_channels(websocketpp::log::alevel::disconnect);
        cli.set_access_channels(websocketpp::log::alevel::app);

        // ��ʼ���ͻ��˲�����Ϣ�ص�����
        cli.init_asio();
        cli.set_message_handler(bind(&on_message, &cli, ::_1, ::_2));

        // ���ӵ�������
        websocketpp::lib::error_code ec;
        con = cli.get_connection("ws://" + serverAddress + ":" + std::to_string(port), ec);
        if (ec) {
            std::cout << "�޷��������ӣ�" << ec.message() << std::endl;
            return false;
        }

        // ���ӵ�������
        cli.connect(con);

        // �ȴ��������
        cli.run();

    }
    catch (websocketpp::exception const& e) {
        std::cout << "����" << e.what() << std::endl;
        return false;
    }

    return true;
}

// ������������Ϣ���ȴ�������Ϣ
std::string sendMessageAndWaitForResponse(const std::string& message) {
    std::unique_lock<std::mutex> lock(mutex);
    server_response = "";
    got_response = false;

    // �������������Ϣ
    // ������Ҫ���ݾ����WebSocket�ͻ��˿���������Ϣ��ʾ����ʡ�Դ˲��ִ���
    cli.send(con, message, websocketpp::frame::opcode::text);
    // �ȴ��ظ�
    cv.wait(lock, [] { return got_response; });

    return server_response;
}

// ������������Ϣ���ȴ�������Ϣ
void sendMessage(const std::string& message) {
    std::unique_lock<std::mutex> lock(mutex);
    // �������������Ϣ
    // ������Ҫ���ݾ����WebSocket�ͻ��˿���������Ϣ��ʾ����ʡ�Դ˲��ִ���
    cli.send(con, message, websocketpp::frame::opcode::text);
  

}

void closeClient() {
    cli.close(con, websocketpp::close::status::normal, "Client shutting down");
}
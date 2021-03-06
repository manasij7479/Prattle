/**

    Prattle/Server/Server.hpp
    =========================

    The server which hosts the chat process.

    The server is responsible for:
     > Communication between clients (through packets)
     > User login and registration
     > Controlling what goes and what does not go into the user database.

    The clients communicate through the server, i.e., all packets
    shared between clients are first received by the server and
    then sent to the intended receiver.

    So, intuitively, the chat process can be visualized as below:

    *******************************************************************


                                ============
                                | CLIENT 2 |
                                ============
                                     |
                                     |
                                     |
                                     |
                                     |
                                     |
      ============               ==========               ============
      | CLIENT 1 | ------------- | SERVER | ------------- | CLIENT 3 |
      ============               ==========               ============
                                     |
                                     |
                                     |
                                     |
                                     |
                                     |
                                ============
                                | CLIENT 4 |
                                ============


    *******************************************************************

    The entire communication process is made using a set of five protocols



    Protocols
    =========


    I. Login Protocols
    ==================

    1.1 Packet sent to Server          : LOGIN username SERVER password
    1.2 Packet sent to Client as reply : LOGIN-SUCCESS SERVER username user-details
    1.3 Packet sent to Client as reply : LOGIN-FAILURE SERVER username details


    II. Signup Protocols
    ====================

    2.1 Packet sent to Server          : SIGNUP username SERVER password
    2.2 Packet sent to Client as reply : SIGNUP-SUCCESS SERVER username
    2.3 Packet sent to Client as reply : SIGNUP-FAILURE SERVER username details


    III. Packet exchange between two(or more) Clients Protocols
    ===========================================================

    3.1 Packet sent to Server                    : SENDMSG sender receiver data
    3.2 Packet sent to Client(receiver) as reply : SENDMSG SERVER sender receiver data
    3.3 Packet sent to Client(sender) as reply   : SENDMSG-SUCCESS SERVER sender
    3.4 Packet sent to Client(sender) as reply   : SENDMSG-FAILURE SERVER sender details


    IV. Searching Database Protocols
    ================================

    4.1 Packet sent to Server              : SEARCH-USER username SEVER name
    4.2 Packet sent to Client as reply     : SEARCH-USER-RESULT SERVER username NumOfResults name1 name2 ...


    V. Adding friends Protocols
    ============================

    5.1 Packet sent to Server              : ADD-FRIEND username SERVER name
    5.2 Packet sent to Client as reply     : ADD-FRIEND-SUCCESS SERVER username
    5.3 Packet sent to Client as reply     : ADD-FRIEND-FAILURE SERVER username details

*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <memory>
#include <SFML/Network.hpp>
#include "../include/System.hpp"
#include "../include/UserDatabase.hpp"

namespace prattle
{
    /* Protocols */

    #define SERVER               "server"

    #define LOGIN                "login"
    #define LOGIN_SUCCESS        "login_success"
    #define LOGIN_FAILURE        "login_failure"

    #define SIGNUP               "signup"
    #define SIGNUP_SUCCESS       "signup_success"
    #define SIGNUP_FAILURE       "signup_failure"

    #define SEND_MSG             "send_msg"
    #define SEND_MSG_SUCCESS     "send_msg_success"
    #define SEND_MSG_FAILURE     "send_msg_failure"

    #define SEARCH_USER          "search_user"
    #define SEARCH_USER_RESULTS  "search_user_results"

    #define ADD_FRIEND           "add_friend"
    #define ADD_FRIEND_SUCCESS   "add_friend_success"
    #define ADD_FRIEND_FAILURE   "add_friend_failure"

    class Server
    {
        public:
            Server();

            void run();

            bool isRunning();                   // Returns true if 'm_running' is true.
            bool wait();                        // Causes 'm_selector' to wait until one or more sockets are ready to receive.
                                                // Returns true if a socket is ready.
            bool newConnectionRequest();        // Listens for new connections until a socket is ready to accept a new connection.
            void addNewClient();                // Adds a connected client (a sf::TcpSocket) to the list of currently connected
                                                // clients 'm_clients'.
            bool send(const sf::Packet& packet);// sends a packet (an instance of sf::Packet) to a connected
                                                // client (an instance of sf::TcpSocket)
            void receive();                     // Receive packets from all clients who are either connected or trying to connect.
            void shutdown();                    // Stop listening to incoming connections
            //void searchDatabase(const std::string& username);
                                                // Search the database for 'username'

        private:
            private:
            sf::TcpListener m_listener;         // Listens to incoming connections at port OPEN_PORT.
            sf::SocketSelector m_selector;      // Selector class for interacting with multiple sockets.
            std::map<std::string, std::unique_ptr<sf::TcpSocket>> m_clients;
                                                // The TcpSockets that act as clients.
            bool m_running;                     // Boolean to indicate the running state of the server.
            UserDatabase db;                    // The user database object that handles or database related tasks.
            sf::Time timeOut;                   // The timeout limit.
            std::multimap<std::string, std::pair<std::string, sf::Packet>> m_messages;
                                                // A map to store the messages being sent by online clients to offline clients.
            std::list<std::unique_ptr<sf::TcpSocket>> newConnections;
                                                // List to store the clients who have established connection
                                                // with the server but are not ready to send/receive messages yet.
    };
}

#endif // SERVER_HPP
